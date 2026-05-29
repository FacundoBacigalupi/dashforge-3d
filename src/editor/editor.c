#include "editor.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

static const float s_deco_palette[8][3] = {
    {0.55f, 0.35f, 0.80f},  /* 0 purple */
    {0.20f, 0.85f, 0.95f},  /* 1 cyan   */
    {1.00f, 0.55f, 0.10f},  /* 2 orange */
    {0.95f, 0.30f, 0.75f},  /* 3 pink   */
    {0.20f, 0.90f, 0.40f},  /* 4 green  */
    {1.00f, 0.95f, 0.10f},  /* 5 yellow */
    {0.95f, 0.20f, 0.20f},  /* 6 red    */
    {0.90f, 0.90f, 0.90f},  /* 7 white  */
};

void editor_init(Editor* e, Level* level, Camera* camera) {
    memset(e, 0, sizeof(*e));
    e->level            = level;
    e->camera           = camera;
    e->selected_type    = LEVEL_OBJECT_BLOCK;
    e->snap_size        = 1;
    e->camera_offset_x  = 5.0f;
    e->camera_offset_y  = 1.5f;
    e->is_new_level     = 1;
    strcpy(e->current_file, "assets/levels/custom/new_level.dash");
    grid_init(&e->grid, 1.0f, 64, 16);
}

void editor_shutdown(Editor* e) { grid_destroy(&e->grid); }

static void world_from_screen(Editor* e, Renderer* r,
                               double sx, double sy, float* wx, float* wy) {
    float ndc_x = (float)(sx / r->fb_width)  * 2.0f - 1.0f;
    float ndc_y = 1.0f - (float)(sy / r->fb_height) * 2.0f;
    mat4 proj, view, vp, inv_vp;
    camera_get_view(e->camera, view);
    camera_get_projection(e->camera, r->aspect, proj);
    glm_mat4_mul(proj, view, vp);
    glm_mat4_inv(vp, inv_vp);
    vec4 near_ndc = {ndc_x, ndc_y, -1.0f, 1.0f};
    vec4 far_ndc  = {ndc_x, ndc_y,  1.0f, 1.0f};
    vec4 nw, fw;
    glm_mat4_mulv(inv_vp, near_ndc, nw);
    glm_mat4_mulv(inv_vp, far_ndc,  fw);
    if (fabsf(nw[3]) > 1e-5f) { nw[0]/=nw[3]; nw[1]/=nw[3]; nw[2]/=nw[3]; }
    if (fabsf(fw[3]) > 1e-5f) { fw[0]/=fw[3]; fw[1]/=fw[3]; fw[2]/=fw[3]; }
    float dz = fw[2] - nw[2];
    if (fabsf(dz) < 1e-4f) { *wx = nw[0]; *wy = nw[1]; return; }
    float t = -nw[2] / dz;
    *wx = nw[0] + t * (fw[0] - nw[0]);
    *wy = nw[1] + t * (fw[1] - nw[1]);
}

static int snap(float v, int size) { return (int)floorf(v / size) * size; }

/* Generate a safe filename from a display name */
static void name_to_filename(const char* name, char* out, int out_size) {
    snprintf(out, (size_t)out_size, "assets/levels/custom/");
    int base = (int)strlen(out);
    int j = base;
    for (int i = 0; name[i] && j < out_size - 6; i++) {
        char c = (char)tolower((unsigned char)name[i]);
        if (c == ' ' || c == '-') c = '_';
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
            out[j++] = c;
    }
    if (j == base) { /* empty name fallback */
        strcpy(out + j, "custom");
        j += 6;
    }
    strcpy(out + j, ".dash");
}

/* ---- Save dialog input ---- */
static void save_dialog_update(Editor* e, const Input* input, Renderer* r) {
    /* Append typed characters — reset conflict flag on any change */
    for (int i = 0; i < input->text_input_len; i++) {
        char c = input->text_input[i];
        if (e->name_buf_len < 48 && (isalnum((unsigned char)c) || c == ' ' || c == '_' || c == '-')) {
            e->name_buf[e->name_buf_len++] = c;
            e->name_buf[e->name_buf_len]   = '\0';
            e->save_name_conflict = 0;
        }
    }
    /* Backspace */
    if (input_just_pressed(input, KEY_BACKSPACE) && e->name_buf_len > 0) {
        e->name_buf[--e->name_buf_len] = '\0';
        e->save_name_conflict = 0;
    }

    /* Helper: try to confirm save; returns 1 on conflict, 0 on success */
    /* Confirm with Enter */
    if (input_just_pressed(input, KEY_ENTER) && e->name_buf_len > 0) {
        char path[256];
        name_to_filename(e->name_buf, path, sizeof(path));
        FILE* chk = fopen(path, "r");
        if (chk) { fclose(chk); e->save_name_conflict = 1; return; }
        strncpy(e->current_file, path, sizeof(e->current_file)-1);
        e->is_new_level    = 0;
        e->save_dialog_open = 0;
        editor_save(e);
        e->level_saved = 1;
        return;
    }
    /* Cancel */
    if (input_just_pressed(input, KEY_ESCAPE)) {
        e->save_dialog_open = 0;
        return;
    }
    /* Mouse click on SAVE / CANCEL buttons */
    if (input_mouse_just_pressed(input, MOUSE_LEFT) && r->fb_width > 0) {
        double mx, my;
        input_get_mouse_pos(input, &mx, &my);
        float nx = (float)(mx / r->fb_width)  * 2.0f - 1.0f;
        float ny = 1.0f - (float)(my / r->fb_height) * 2.0f;
        /* SAVE: rect(-0.25, -0.14, 0.42w, 0.13h) → x:[-0.46,-0.04] y:[-0.205,-0.075] */
        int on_save   = (nx>=-0.46f && nx<=-0.04f && ny>=-0.205f && ny<=-0.075f);
        /* CANCEL: rect(0.25, -0.14, 0.42w, 0.13h) → x:[0.04,0.46] */
        int on_cancel = (nx>= 0.04f && nx<= 0.46f && ny>=-0.205f && ny<=-0.075f);
        if (on_save && e->name_buf_len > 0) {
            char path[256];
            name_to_filename(e->name_buf, path, sizeof(path));
            FILE* chk = fopen(path, "r");
            if (chk) { fclose(chk); e->save_name_conflict = 1; }
            else {
                strncpy(e->current_file, path, sizeof(e->current_file)-1);
                e->is_new_level     = 0;
                e->save_dialog_open = 0;
                e->save_name_conflict = 0;
                editor_save(e);
                e->level_saved = 1;
            }
        }
        if (on_cancel)
            e->save_dialog_open = 0;
    }
}

void editor_update(Editor* e, const Input* input, Renderer* r, float dt) {
    (void)dt;

    /* Save dialog intercepts all input when open */
    if (e->save_dialog_open) {
        save_dialog_update(e, input, r);
        return;
    }

    /* Camera panning */
    float scroll = (float)input_get_scroll(input);
    if (input_shift(input)) e->camera_offset_y += scroll * 0.5f;
    else                    e->camera_offset_x += scroll * 0.8f;

    {
        double dx, dy;
        input_get_mouse_delta(input, &dx, &dy);
        if (input_mouse_pressed(input, MOUSE_MIDDLE)) {
            e->camera_offset_x -= (float)dx * 0.025f;
            e->camera_offset_y += (float)dy * 0.025f;
        }
    }
    e->camera->position[0] = e->camera_offset_x;
    e->camera->position[1] = e->camera_offset_y;
    e->camera->position[2] = 15.0f;
    e->camera->target[0]   = e->camera_offset_x;
    e->camera->target[1]   = e->camera_offset_y;
    e->camera->target[2]   = 0.0f;

    /* Type selection: 1=Block 2=Spike 3=Deco 4=Start 5=End */
    if (input_just_pressed(input, KEY_1)) e->selected_type = LEVEL_OBJECT_BLOCK;
    if (input_just_pressed(input, KEY_2)) e->selected_type = LEVEL_OBJECT_SPIKE;
    if (input_just_pressed(input, KEY_3)) e->selected_type = LEVEL_OBJECT_DECORATION;
    if (input_just_pressed(input, KEY_4)) e->selected_type = LEVEL_OBJECT_START;
    if (input_just_pressed(input, KEY_5)) e->selected_type = LEVEL_OBJECT_END;

    /* Decoration color cycle with Left/Right */
    if (e->selected_type == LEVEL_OBJECT_DECORATION) {
        if (input_just_pressed(input, KEY_LEFT))
            e->deco_color_idx = (e->deco_color_idx + 7) % 8;
        if (input_just_pressed(input, KEY_RIGHT))
            e->deco_color_idx = (e->deco_color_idx + 1) % 8;
    }

    /* Save: open dialog on first save, direct save otherwise */
    if (input_ctrl(input) && input_just_pressed(input, KEY_S)) {
        if (e->is_new_level) {
            memset(e->name_buf, 0, sizeof(e->name_buf));
            e->name_buf_len    = 0;
            e->save_dialog_open = 1;
        } else {
            editor_save(e);
            e->level_saved = 1;
        }
    }

    /* Cursor world position */
    double mx, my;
    input_get_mouse_pos(input, &mx, &my);
    float wx, wy;
    world_from_screen(e, r, mx, my, &wx, &wy);
    e->cursor_grid_x = snap(wx, e->snap_size);
    e->cursor_grid_y = snap(wy, e->snap_size);
    e->cursor_x = (float)e->cursor_grid_x;
    e->cursor_y = (float)e->cursor_grid_y;

    /* Place */
    if (input_mouse_just_pressed(input, MOUSE_LEFT)) {
        LevelObject obj = {0};
        obj.type = e->selected_type;

        /* Decorations render behind the track */
        float z = (obj.type == LEVEL_OBJECT_DECORATION) ? -2.0f : 0.0f;
        glm_vec3_copy((vec3){e->cursor_x, e->cursor_y, z}, obj.transform.position);
        glm_vec3_copy((vec3){0,0,0}, obj.transform.rotation_euler);
        glm_vec3_copy((vec3){1,1,1}, obj.transform.scale);
        obj.solid     = (obj.type == LEVEL_OBJECT_BLOCK);   /* deco is NOT solid */
        obj.lethal    = (obj.type == LEVEL_OBJECT_SPIKE);
        obj.is_end    = (obj.type == LEVEL_OBJECT_END);
        obj.color_idx = (obj.type == LEVEL_OBJECT_DECORATION) ? e->deco_color_idx : 0;

        /* START: only one per map — remove any previous, update spawn */
        if (obj.type == LEVEL_OBJECT_START) {
            for (int i = e->level->object_count - 1; i >= 0; i--) {
                if (e->level->objects[i].type == LEVEL_OBJECT_START)
                    level_remove_object_at(e->level, i);
            }
            glm_vec3_copy((vec3){e->cursor_x, e->cursor_y, 0.0f}, e->level->player_start);
        }

        /* END: only one per map — remove any previous, update level end X */
        if (obj.type == LEVEL_OBJECT_END) {
            for (int i = e->level->object_count - 1; i >= 0; i--) {
                if (e->level->objects[i].type == LEVEL_OBJECT_END)
                    level_remove_object_at(e->level, i);
            }
            e->level->level_end[0] = e->cursor_x;
            e->level->level_end[1] = e->cursor_y;
            e->level->level_end[2] = 0.0f;
        }

        level_object_rebuild_collider(&obj);
        level_add_object(e->level, obj);
        e->unsaved = 1;
    }

    /* Erase */
    if (input_mouse_just_pressed(input, MOUSE_RIGHT)) {
        float tx = e->cursor_x, ty = e->cursor_y;
        for (int i = e->level->object_count - 1; i >= 0; i--) {
            float ox = e->level->objects[i].transform.position[0];
            float oy = e->level->objects[i].transform.position[1];
            if (fabsf(ox - tx) < 0.5f && fabsf(oy - ty) < 0.5f) {
                level_remove_object_at(e->level, i);
                e->unsaved = 1;
                break;
            }
        }
    }
}

static void draw_save_dialog(Editor* e, Renderer* r) {
    renderer_draw_rect(r, 0.0f, 0.0f, 2.0f, 2.0f, (vec4){0.0f,0.0f,0.0f,0.72f});
    renderer_draw_rect(r, 0.0f, 0.08f, 1.15f, 0.62f, (vec4){0.08f,0.09f,0.18f,0.97f});

    renderer_draw_rect(r, 0.0f, 0.30f, 1.0f, 0.16f, (vec4){0.15f,0.25f,0.55f,0.9f});
    renderer_draw_text_ndc(r, 0.0f, 0.30f, 2.5f, "NAME YOUR LEVEL",
                           (vec4){0.85f,0.92f,1.0f,1.0f});

    /* Input field */
    renderer_draw_rect(r, 0.0f, 0.10f, 1.0f, 0.13f, (vec4){0.04f,0.04f,0.10f,1.0f});
    char display[68];
    snprintf(display, sizeof(display), "%s_", e->name_buf);
    renderer_draw_text_ndc(r, 0.0f, 0.10f, 2.0f, display, (vec4){1,1,1,1});

    if (e->save_name_conflict)
        renderer_draw_text_ndc(r, 0.0f, -0.06f, 1.5f, "Name already taken! Choose another.",
                               (vec4){1.0f, 0.30f, 0.30f, 1.0f});
    else
        renderer_draw_text_ndc(r, 0.0f, -0.06f, 1.5f, "Letters, digits, spaces. ENTER=confirm",
                               (vec4){0.5f, 0.5f, 0.5f, 1.0f});

    /* SAVE button */
    vec4 save_c;
    glm_vec4_copy((vec4){0.10f, 0.70f, 0.20f, 1.0f}, save_c);
    renderer_draw_rect(r, -0.25f, -0.14f, 0.42f, 0.13f, save_c);
    renderer_draw_text_ndc(r, -0.25f, -0.14f, 2.0f, "SAVE", (vec4){1,1,1,1});

    /* CANCEL button */
    vec4 cancel_c;
    glm_vec4_copy((vec4){0.65f, 0.12f, 0.12f, 1.0f}, cancel_c);
    renderer_draw_rect(r, 0.25f, -0.14f, 0.42f, 0.13f, cancel_c);
    renderer_draw_text_ndc(r, 0.25f, -0.14f, 2.0f, "CANCEL", (vec4){1,1,1,1});
}

void editor_render(Editor* e, Renderer* r) {
    glDisable(GL_DEPTH_TEST);
    grid_draw(&e->grid, r);
    glEnable(GL_DEPTH_TEST);

    /* Level objects */
    for (int i = 0; i < e->level->object_count; i++) {
        LevelObject* obj = &e->level->objects[i];
        Material mat; mat.diffuse = NULL;
        switch (obj->type) {
        case LEVEL_OBJECT_BLOCK:      glm_vec4_copy((vec4){0.5f,0.5f,0.55f,1}, mat.color); break;
        case LEVEL_OBJECT_SPIKE:      glm_vec4_copy((vec4){1.0f,0.3f,0.2f,1},  mat.color); break;
        case LEVEL_OBJECT_END:        glm_vec4_copy((vec4){0.2f,1.0f,0.3f,1},  mat.color); break;
        case LEVEL_OBJECT_DECORATION: {
            int ci = obj->color_idx & 7;
            mat.color[0] = s_deco_palette[ci][0];
            mat.color[1] = s_deco_palette[ci][1];
            mat.color[2] = s_deco_palette[ci][2];
            mat.color[3] = 1.0f;
            break;
        }
        default:                      glm_vec4_copy((vec4){0.8f,0.8f,0.2f,1},  mat.color); break;
        }
        Mesh* mesh = (obj->type == LEVEL_OBJECT_SPIKE) ? &r->spike_mesh : &r->cube_mesh;
        renderer_draw_mesh(r, mesh, &mat, obj->transform);
    }

    /* Cursor preview */
    Transform ct = transform_default();
    glm_vec3_copy((vec3){e->cursor_x, e->cursor_y, 0.0f}, ct.position);
    Material cur; cur.diffuse = NULL;
    glm_vec4_copy((vec4){1,1,0,0.5f}, cur.color);
    glEnable(GL_BLEND);
    Mesh* preview = (e->selected_type == LEVEL_OBJECT_SPIKE) ? &r->spike_mesh : &r->cube_mesh;
    renderer_draw_mesh(r, preview, &cur, ct);

    /* Type selector HUD — displayed in KEY order (1-5) */
    static const LevelObjectType hud_order[] = {
        LEVEL_OBJECT_BLOCK, LEVEL_OBJECT_SPIKE, LEVEL_OBJECT_DECORATION,
        LEVEL_OBJECT_START, LEVEL_OBJECT_END
    };
    static const char* hud_labels[] = {
        "[1]BLOCK", "[2]SPIKE", "[3]DECO", "[4]START", "[5]END"
    };
    static const char* hud_desc[] = {
        "Solid — stops player",
        "Spike — instant death",
        "Visual only, behind track.  L/R arrow keys = change color",
        "Set player spawn (one per map)",
        "Level end trigger (one per map)"
    };
    float bx = -0.72f;
    const char* cur_desc = "";
    for (int i = 0; i < 5; i++) {
        int sel = (e->selected_type == hud_order[i]);
        if (sel) cur_desc = hud_desc[i];
        vec4 bc;
        if (sel) glm_vec4_copy((vec4){0.2f,0.5f,0.9f,0.9f}, bc);
        else     glm_vec4_copy((vec4){0.08f,0.08f,0.18f,0.75f}, bc);
        renderer_draw_rect(r, bx, -0.88f, 0.33f, 0.10f, bc);
        renderer_draw_text_ndc(r, bx, -0.88f, 1.4f, hud_labels[i], (vec4){1,1,1,1});
        bx += 0.36f;
    }
    /* Description of current type */
    renderer_draw_text_ndc(r, 0.0f, -0.97f, 1.5f, cur_desc,
                           (vec4){0.7f, 0.8f, 1.0f, 0.9f});

    /* Ctrl+S hint */
    const char* save_hint = e->is_new_level ? "Ctrl+S: Save as..." : "Ctrl+S: Save";
    renderer_draw_text_ndc(r, 0.75f, 0.92f, 1.4f, save_hint, (vec4){0.6f,0.6f,0.6f,1});

    /* Show selected deco color swatch when deco tool active */
    if (e->selected_type == LEVEL_OBJECT_DECORATION) {
        int ci = e->deco_color_idx & 7;
        vec4 sw = {s_deco_palette[ci][0], s_deco_palette[ci][1], s_deco_palette[ci][2], 1.0f};
        renderer_draw_rect(r, -0.85f, 0.92f, 0.08f, 0.07f, sw);
        renderer_draw_text_ndc(r, -0.68f, 0.92f, 1.4f, "col ←/→", (vec4){0.8f,0.8f,0.8f,1});
    }

    /* Save dialog overlay */
    if (e->save_dialog_open)
        draw_save_dialog(e, r);
}

void editor_save(Editor* e) {
    if (level_save(e->level, e->current_file)) {
        printf("Editor: saved '%s'\n", e->current_file);
        e->unsaved = 0;
    }
}

void editor_load(Editor* e, const char* path) {
    strncpy(e->current_file, path, sizeof(e->current_file)-1);
    level_load(e->level, path);
    e->unsaved     = 0;
    e->is_new_level = 0;
}
