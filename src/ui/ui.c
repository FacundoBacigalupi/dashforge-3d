#include "ui.h"
#include "game/game.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void ui_init(UI* ui) {
    ui->menu_selection = 0;
}

/* ---- helpers ---- */

static int point_in_rect(float px, float py, float cx, float cy, float w, float h) {
    return px >= cx - w * 0.5f && px <= cx + w * 0.5f &&
           py >= cy - h * 0.5f && py <= cy + h * 0.5f;
}

/* Draw a full-width horizontal rule */
static void draw_rule(Renderer* r, float ndc_y) {
    renderer_draw_rect(r, 0.0f, ndc_y, 1.8f, 0.005f, (vec4){0.3f, 0.3f, 0.4f, 0.8f});
}

/* ---- Main Menu ---- */

void ui_update_menu(UI* ui, const Input* input, Renderer* r, int* out_play, int* out_quit) {
    *out_play = 0;
    *out_quit = 0;

    if (input_just_pressed(input, KEY_UP)   || input_just_pressed(input, KEY_W)) ui->menu_selection = 0;
    if (input_just_pressed(input, KEY_DOWN) || input_just_pressed(input, KEY_S)) ui->menu_selection = 1;

    double mx, my;
    input_get_mouse_pos(input, &mx, &my);
    float ndcx = (r->fb_width  > 0) ? (float)(mx / r->fb_width)  * 2.0f - 1.0f : 0.0f;
    float ndcy = (r->fb_height > 0) ? 1.0f - (float)(my / r->fb_height) * 2.0f : 0.0f;
    if (point_in_rect(ndcx, ndcy, 0.0f,  0.12f, 0.55f, 0.14f)) ui->menu_selection = 0;
    if (point_in_rect(ndcx, ndcy, 0.0f, -0.12f, 0.55f, 0.14f)) ui->menu_selection = 1;

    int confirm = input_just_pressed(input, KEY_ENTER)
               || input_just_pressed(input, KEY_SPACE)
               || input_mouse_just_pressed(input, MOUSE_LEFT);
    if (confirm) {
        if (ui->menu_selection == 0) *out_play = 1;
        else                         *out_quit = 1;
    }
}

void ui_draw_main_menu(UI* ui, Renderer* r) {
    renderer_draw_rect(r, 0.0f, 0.0f, 2.0f, 2.0f, (vec4){0.0f, 0.0f, 0.0f, 0.60f});

    /* Title bar */
    renderer_draw_rect(r, 0.0f, 0.55f, 0.80f, 0.18f, (vec4){0.15f, 0.25f, 0.55f, 0.9f});
    renderer_draw_text_ndc(r, 0.0f, 0.55f, 3.5f, "DASHFORGE 3D",
                           (vec4){0.85f, 0.92f, 1.0f, 1.0f});

    /* PLAY */
    float play_w = (ui->menu_selection == 0) ? 0.55f : 0.50f;
    vec4 play_c;
    if (ui->menu_selection == 0) glm_vec4_copy((vec4){0.10f, 0.85f, 0.20f, 1.00f}, play_c);
    else                         glm_vec4_copy((vec4){0.05f, 0.32f, 0.10f, 0.85f}, play_c);
    renderer_draw_rect(r, 0.0f, 0.12f, play_w, 0.14f, play_c);
    renderer_draw_text_ndc(r, 0.0f, 0.12f, 3.0f, "PLAY",
                           (vec4){1.0f, 1.0f, 1.0f, 1.0f});

    /* QUIT */
    float quit_w = (ui->menu_selection == 1) ? 0.55f : 0.50f;
    vec4 quit_c;
    if (ui->menu_selection == 1) glm_vec4_copy((vec4){0.90f, 0.12f, 0.12f, 1.00f}, quit_c);
    else                         glm_vec4_copy((vec4){0.35f, 0.05f, 0.05f, 0.85f}, quit_c);
    renderer_draw_rect(r, 0.0f, -0.12f, quit_w, 0.14f, quit_c);
    renderer_draw_text_ndc(r, 0.0f, -0.12f, 3.0f, "QUIT",
                           (vec4){1.0f, 1.0f, 1.0f, 1.0f});
}

/* ---- How To Play ---- */

void ui_draw_how_to_play(UI* ui, Renderer* r) {
    renderer_draw_rect(r, 0.0f, 0.0f, 2.0f, 2.0f, (vec4){0.0f, 0.0f, 0.05f, 0.92f});

    renderer_draw_rect(r, 0.0f, 0.78f, 1.0f, 0.18f, (vec4){0.15f, 0.25f, 0.55f, 0.9f});
    renderer_draw_text_ndc(r, 0.0f, 0.78f, 3.0f, "HOW TO PLAY",
                           (vec4){0.85f, 0.92f, 1.0f, 1.0f});

    draw_rule(r, 0.62f);

    static const char* keys[]  = { "SPACE",    "R",       "ESC",              "TAB",         "F1/F2/F3"      };
    static const char* descs[] = { "Jump",     "Restart", "Pause / Menu",     "Level editor","Camera modes"  };
    float y = 0.50f;
    for (int i = 0; i < 5; i++) {
        char line[64];
        snprintf(line, sizeof(line), "%-9s  %s", keys[i], descs[i]);
        renderer_draw_text_ndc(r, 0.0f, y, 2.0f, line, (vec4){0.9f, 0.9f, 0.9f, 1.0f});
        y -= 0.20f;
    }

    draw_rule(r, -0.50f);

    /* BACK button */
    vec4 back_c, cont_c;
    if (ui->htp_selection == 0) glm_vec4_copy((vec4){0.70f, 0.12f, 0.12f, 1.0f}, back_c);
    else                         glm_vec4_copy((vec4){0.28f, 0.05f, 0.05f, 0.85f}, back_c);
    renderer_draw_rect(r, -0.30f, -0.70f, 0.46f, 0.14f, back_c);
    renderer_draw_text_ndc(r, -0.30f, -0.70f, 2.2f, "BACK", (vec4){1,1,1,1});

    /* CONTINUE button */
    if (ui->htp_selection == 1) glm_vec4_copy((vec4){0.10f, 0.80f, 0.22f, 1.0f}, cont_c);
    else                         glm_vec4_copy((vec4){0.04f, 0.30f, 0.08f, 0.85f}, cont_c);
    renderer_draw_rect(r, 0.30f, -0.70f, 0.52f, 0.14f, cont_c);
    renderer_draw_text_ndc(r, 0.30f, -0.70f, 2.2f, "CONTINUE", (vec4){1,1,1,1});
}

void ui_update_how_to_play(UI* ui, const Input* input, Renderer* r,
                            int* out_back, int* out_continue) {
    *out_back = *out_continue = 0;

    /* Keyboard L/R to switch, ENTER/SPACE to confirm */
    if (input_just_pressed(input, KEY_LEFT)  || input_just_pressed(input, KEY_A))
        ui->htp_selection = 0;
    if (input_just_pressed(input, KEY_RIGHT) || input_just_pressed(input, KEY_D))
        ui->htp_selection = 1;

    /* Mouse hover */
    if (r->fb_width > 0 && r->fb_height > 0) {
        double mx, my;
        input_get_mouse_pos(input, &mx, &my);
        float ndcx = (float)(mx / r->fb_width)  * 2.0f - 1.0f;
        float ndcy = 1.0f - (float)(my / r->fb_height) * 2.0f;
        if (point_in_rect(ndcx, ndcy, -0.30f, -0.70f, 0.46f, 0.14f)) {
            ui->htp_selection = 0;
            if (input_mouse_just_pressed(input, MOUSE_LEFT)) { *out_back = 1; return; }
        }
        if (point_in_rect(ndcx, ndcy,  0.30f, -0.70f, 0.52f, 0.14f)) {
            ui->htp_selection = 1;
            if (input_mouse_just_pressed(input, MOUSE_LEFT)) { *out_continue = 1; return; }
        }
    }

    if (input_just_pressed(input, KEY_ENTER) || input_just_pressed(input, KEY_SPACE)) {
        if (ui->htp_selection == 0) *out_back    = 1;
        else                         *out_continue = 1;
    }
    if (input_just_pressed(input, KEY_ESCAPE)) *out_back = 1;
}

/* ---- Level Select ---- */

#define LS_VISIBLE 6

void ui_draw_level_select(UI* ui, Renderer* r, struct Game* g) {
    (void)ui;
    renderer_draw_rect(r, 0.0f, 0.0f, 2.0f, 2.0f, (vec4){0.0f, 0.0f, 0.05f, 0.93f});

    renderer_draw_rect(r, 0.0f, 0.82f, 1.0f, 0.16f, (vec4){0.15f, 0.25f, 0.55f, 0.9f});
    renderer_draw_text_ndc(r, 0.0f, 0.82f, 3.0f, "SELECT LEVEL",
                           (vec4){0.85f, 0.92f, 1.0f, 1.0f});

    if (g->level_count == 0) {
        renderer_draw_text_ndc(r, 0.0f, 0.0f, 2.0f, "No levels found.",
                               (vec4){0.8f, 0.4f, 0.4f, 1.0f});
    }

    float slot_h   = 0.17f;
    float list_top = 0.56f;   /* lowered from 0.64 — creates gap below title for scroll arrow */
    int   start    = g->level_select_scroll;
    int   end      = start + LS_VISIBLE;
    if (end > g->level_count) end = g->level_count;

    /* List items first */
    for (int i = start; i < end; i++) {
        int   slot     = i - start;
        float cy       = list_top - slot * slot_h;
        int   selected = (i == g->level_select_cursor);

        vec4 bg;
        if (selected && g->level_list[i].is_custom)
            glm_vec4_copy((vec4){0.50f, 0.38f, 0.05f, 1.0f}, bg);
        else if (selected)
            glm_vec4_copy((vec4){0.08f, 0.28f, 0.60f, 1.0f}, bg);
        else if (g->level_list[i].is_custom)
            glm_vec4_copy((vec4){0.18f, 0.13f, 0.04f, 0.9f}, bg);
        else
            glm_vec4_copy((vec4){0.05f, 0.10f, 0.22f, 0.9f}, bg);

        renderer_draw_rect(r, 0.0f, cy, 1.65f, slot_h * 0.82f, bg);

        char label[160];
        const char* tag = g->level_list[i].is_custom ? "[C] " : "[O] ";
        snprintf(label, sizeof(label), "%s%s", tag, g->level_list[i].display_name);

        vec4 tc;
        if (selected) glm_vec4_copy((vec4){1.0f, 1.0f, 1.0f, 1.0f}, tc);
        else          glm_vec4_copy((vec4){0.70f, 0.70f, 0.70f, 1.0f}, tc);
        renderer_draw_text_ndc(r, 0.0f, cy, 1.8f, label, tc);
    }

    /* Scroll arrows drawn AFTER list items so they render on top of item edges */
    if (start > 0) {
        /* Gap between title bottom (~0.74) and first item top (~0.63) is ~0.11 — put arrow at 0.69 */
        renderer_draw_rect(r, 0.0f, 0.70f, 0.50f, 0.07f, (vec4){0.0f, 0.0f, 0.05f, 0.93f});
        renderer_draw_text_ndc(r, 0.0f, 0.70f, 1.8f, "/\\  scroll up",
                               (vec4){0.70f, 0.70f, 0.70f, 1.0f});
    }
    if (end < g->level_count)
        renderer_draw_text_ndc(r, 0.0f, list_top - LS_VISIBLE * slot_h + 0.06f, 1.8f, "\\/  scroll down",
                               (vec4){0.55f, 0.55f, 0.55f, 1.0f});

    /* Action buttons: NEW LEVEL  |  OPEN FOLDER  (side by side) */
    renderer_draw_rect(r, -0.33f, -0.52f, 0.56f, 0.11f,
                       (vec4){0.10f, 0.55f, 0.18f, 0.90f});
    renderer_draw_text_ndc(r, -0.33f, -0.52f, 1.7f, "+ NEW LEVEL",
                           (vec4){1.0f, 1.0f, 1.0f, 1.0f});

    renderer_draw_rect(r, 0.33f, -0.52f, 0.56f, 0.11f,
                       (vec4){0.10f, 0.30f, 0.60f, 0.90f});
    renderer_draw_text_ndc(r, 0.33f, -0.52f, 1.7f, "OPEN FOLDER",
                           (vec4){1.0f, 1.0f, 1.0f, 1.0f});

    draw_rule(r, -0.67f);
    renderer_draw_text_ndc(r, 0.0f, -0.78f, 1.5f,
                           "UP/DOWN Navigate   ENTER Play   ESC Back",
                           (vec4){0.45f, 0.45f, 0.45f, 1.0f});
    renderer_draw_text_ndc(r, 0.0f, -0.90f, 1.4f,
                           "To share: put .dash files in the custom folder, then OPEN FOLDER",
                           (vec4){0.35f, 0.35f, 0.35f, 1.0f});
}

void ui_update_level_select(UI* ui, const Input* input, struct Game* g,
                             int* out_selected, int* out_back,
                             int* out_new_level, int* out_open_folder) {
    (void)ui;
    *out_selected   = -1;
    *out_back       = 0;
    *out_new_level  = 0;
    *out_open_folder = 0;

    if (g->level_count == 0) {
        if (input_just_pressed(input, KEY_ESCAPE)) *out_back = 1;
        return;
    }

    /* Keyboard navigation */
    if (input_just_pressed(input, KEY_UP)   || input_just_pressed(input, KEY_W))
        { if (g->level_select_cursor > 0) g->level_select_cursor--; }
    if (input_just_pressed(input, KEY_DOWN) || input_just_pressed(input, KEY_S))
        { if (g->level_select_cursor < g->level_count - 1) g->level_select_cursor++; }

    /* Mouse scroll */
    float scroll = (float)input_get_scroll(input);
    if (scroll > 0 && g->level_select_scroll > 0)             g->level_select_scroll--;
    if (scroll < 0 && g->level_select_scroll + LS_VISIBLE < g->level_count) g->level_select_scroll++;

    /* Mouse hover + click */
    if (g->renderer.fb_width > 0 && g->renderer.fb_height > 0) {
        double mx, my;
        input_get_mouse_pos(input, &mx, &my);
        float ndcx = (float)(mx / g->renderer.fb_width)  * 2.0f - 1.0f;
        float ndcy = 1.0f - (float)(my / g->renderer.fb_height) * 2.0f;

        int start = g->level_select_scroll;
        int end   = start + LS_VISIBLE; if (end > g->level_count) end = g->level_count;
        for (int i = start; i < end; i++) {
            int   slot = i - start;
            float cy   = 0.56f - slot * 0.17f;
            if (point_in_rect(ndcx, ndcy, 0.0f, cy, 1.65f, 0.17f * 0.82f)) {
                g->level_select_cursor = i;
                if (input_mouse_just_pressed(input, MOUSE_LEFT))
                    *out_selected = i;
            }
        }
    }

    /* Action button clicks */
    if (g->renderer.fb_width > 0 && g->renderer.fb_height > 0) {
        double mx, my;
        input_get_mouse_pos(input, &mx, &my);
        float ndcx = (float)(mx / g->renderer.fb_width)  * 2.0f - 1.0f;
        float ndcy = 1.0f - (float)(my / g->renderer.fb_height) * 2.0f;
        if (point_in_rect(ndcx, ndcy, -0.33f, -0.52f, 0.56f, 0.11f)) {
            if (input_mouse_just_pressed(input, MOUSE_LEFT))
                *out_new_level = 1;
        }
        if (point_in_rect(ndcx, ndcy, 0.33f, -0.52f, 0.56f, 0.11f)) {
            if (input_mouse_just_pressed(input, MOUSE_LEFT))
                *out_open_folder = 1;
        }
    }

    /* Keep cursor in visible window */
    if (g->level_select_cursor < g->level_select_scroll)
        g->level_select_scroll = g->level_select_cursor;
    if (g->level_select_cursor >= g->level_select_scroll + LS_VISIBLE)
        g->level_select_scroll = g->level_select_cursor - LS_VISIBLE + 1;

    if (input_just_pressed(input, KEY_ENTER) || input_just_pressed(input, KEY_SPACE))
        *out_selected = g->level_select_cursor;
    if (input_just_pressed(input, KEY_ESCAPE))
        *out_back = 1;
}

/* ---- Level Complete ---- */

/* label/x-center for each option.  show_next=1: 3 opts; show_next=0: 2 opts */
static void lc_layout(int show_next, int n_opts_out[1],
                       const char* labels_out[], float cx_out[], float bw_out[1]) {
    if (show_next) {
        *n_opts_out = 3;
        labels_out[0] = "RETRY";       cx_out[0] = -0.43f;
        labels_out[1] = "NEXT LEVEL";  cx_out[1] =  0.00f;
        labels_out[2] = "EXIT";        cx_out[2] =  0.43f;
        *bw_out = 0.42f;
    } else {
        *n_opts_out = 2;
        labels_out[0] = "RETRY";  cx_out[0] = -0.32f;
        labels_out[1] = "EXIT";   cx_out[1] =  0.32f;
        *bw_out = 0.63f;
    }
}

void ui_draw_level_complete(UI* ui, Renderer* r, int show_next) {
    /* Background panel — light mint, semi-transparent */
    renderer_draw_rect(r, 0.0f, 0.12f, 1.30f, 0.58f, (vec4){0.78f, 0.92f, 0.84f, 0.52f});

    renderer_draw_text_ndc(r, 0.0f, 0.30f, 3.2f, "LEVEL COMPLETE!",
                           (vec4){0.2f, 1.0f, 0.4f, 1.0f});

    int n; const char* lbl[3]; float cx[3]; float bw;
    lc_layout(show_next, &n, lbl, cx, &bw);

    for (int i = 0; i < n; i++) {
        vec4 c;
        if (ui->lc_selection == i)
            glm_vec4_copy((vec4){0.15f, 0.70f, 0.25f, 1.0f}, c);
        else
            glm_vec4_copy((vec4){0.12f, 0.20f, 0.12f, 0.85f}, c);
        renderer_draw_rect(r, cx[i], -0.05f, bw, 0.13f, c);
        renderer_draw_text_ndc(r, cx[i], -0.05f, 2.0f, lbl[i], (vec4){1,1,1,1});
    }
}

void ui_update_level_complete(UI* ui, const Input* input, Renderer* r,
                               int show_next,
                               int* out_retry, int* out_next, int* out_exit) {
    *out_retry = *out_next = *out_exit = 0;

    int n; const char* lbl[3]; float cx[3]; float bw;
    lc_layout(show_next, &n, lbl, cx, &bw);

    /* Clamp selection */
    if (ui->lc_selection < 0)     ui->lc_selection = 0;
    if (ui->lc_selection >= n)    ui->lc_selection = n - 1;

    /* Keyboard L/R */
    if (input_just_pressed(input, KEY_LEFT) || input_just_pressed(input, KEY_A))
        { if (ui->lc_selection > 0) ui->lc_selection--; }
    if (input_just_pressed(input, KEY_RIGHT) || input_just_pressed(input, KEY_D))
        { if (ui->lc_selection < n - 1) ui->lc_selection++; }

    /* Mouse hover */
    if (r->fb_width > 0 && r->fb_height > 0) {
        double mx, my;
        input_get_mouse_pos(input, &mx, &my);
        float ndcx = (float)(mx / r->fb_width)  * 2.0f - 1.0f;
        float ndcy = 1.0f - (float)(my / r->fb_height) * 2.0f;
        for (int i = 0; i < n; i++) {
            if (point_in_rect(ndcx, ndcy, cx[i], -0.05f, bw, 0.13f)) {
                ui->lc_selection = i;
                if (input_mouse_just_pressed(input, MOUSE_LEFT)) {
                    /* fall through to confirm below */
                    goto confirm;
                }
            }
        }
    }

    if (!input_just_pressed(input, KEY_ENTER) && !input_just_pressed(input, KEY_SPACE))
        return;

confirm:;
    /* Map selection index to output */
    if (show_next) {
        if (ui->lc_selection == 0) *out_retry = 1;
        else if (ui->lc_selection == 1) *out_next  = 1;
        else                            *out_exit  = 1;
    } else {
        if (ui->lc_selection == 0) *out_retry = 1;
        else                        *out_exit  = 1;
    }
}

/* ---- Pause Menu ---- */

static const char* pause_labels[] = { "RESUME", "RESTART", "MAIN MENU", "QUIT" };
static const float pause_cy[]     = { 0.30f,    0.08f,    -0.14f,      -0.36f };

void ui_draw_pause_menu(UI* ui, Renderer* r) {
    renderer_draw_rect(r, 0.0f, 0.0f, 2.0f, 2.0f, (vec4){0.0f,0.0f,0.0f,0.55f});
    renderer_draw_rect(r, 0.0f, 0.52f, 0.7f, 0.16f, (vec4){0.15f,0.25f,0.55f,0.9f});
    renderer_draw_text_ndc(r, 0.0f, 0.52f, 3.0f, "PAUSED", (vec4){0.85f,0.92f,1.0f,1.0f});

    static vec4 colors[4] = {
        {0.10f,0.75f,0.20f,1.0f},  /* Resume  - green */
        {0.15f,0.35f,0.75f,1.0f},  /* Restart - blue  */
        {0.65f,0.38f,0.05f,1.0f},  /* Main    - orange*/
        {0.80f,0.10f,0.10f,1.0f},  /* Quit    - red   */
    };
    for (int i = 0; i < 4; i++) {
        float w = (ui->pause_selection == i) ? 0.60f : 0.55f;
        vec4 c;
        if (ui->pause_selection == i) {
            glm_vec4_copy(colors[i], c);
        } else {
            c[0]=colors[i][0]*0.5f; c[1]=colors[i][1]*0.5f;
            c[2]=colors[i][2]*0.5f; c[3]=0.85f;
        }
        renderer_draw_rect(r, 0.0f, pause_cy[i], w, 0.13f, c);
        renderer_draw_text_ndc(r, 0.0f, pause_cy[i], 2.5f, pause_labels[i],
                               (vec4){1,1,1,1});
    }
}

void ui_update_pause_menu(UI* ui, const Input* input, Renderer* r,
                           int* out_resume, int* out_restart,
                           int* out_main_menu, int* out_quit) {
    *out_resume = *out_restart = *out_main_menu = *out_quit = 0;

    if (input_just_pressed(input, KEY_UP)   || input_just_pressed(input, KEY_W))
        { if (ui->pause_selection > 0) ui->pause_selection--; }
    if (input_just_pressed(input, KEY_DOWN) || input_just_pressed(input, KEY_S))
        { if (ui->pause_selection < 3) ui->pause_selection++; }

    /* Mouse hover */
    double mx, my;
    input_get_mouse_pos(input, &mx, &my);
    float ndcx = (r->fb_width  > 0) ? (float)(mx/r->fb_width)  * 2.0f - 1.0f : 0.0f;
    float ndcy = (r->fb_height > 0) ? 1.0f - (float)(my/r->fb_height) * 2.0f : 0.0f;
    for (int i = 0; i < 4; i++) {
        if (point_in_rect(ndcx, ndcy, 0.0f, pause_cy[i], 0.60f, 0.13f))
            ui->pause_selection = i;
    }

    int confirm = input_just_pressed(input, KEY_ENTER)
               || input_just_pressed(input, KEY_SPACE)
               || input_mouse_just_pressed(input, MOUSE_LEFT);
    if (confirm) {
        switch (ui->pause_selection) {
        case 0: *out_resume    = 1; break;
        case 1: *out_restart   = 1; break;
        case 2: *out_main_menu = 1; break;
        case 3: *out_quit      = 1; break;
        }
    }
}

/* ---- HUD (unused, title bar handles it) ---- */
void ui_render_hud(UI* ui, struct Game* g) {
    (void)ui;
    (void)g;
}
