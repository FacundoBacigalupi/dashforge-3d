#include "game.h"
#include "editor/editor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>

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

/* --- forward declarations --- */
static void resolve_collisions(Game* g);
static void render_level(Game* g);

void game_scan_levels(Game* g) {
    g->level_count = 0;

    static const char* o_paths[] = {
        "assets/levels/official/level01.dash",
        "assets/levels/official/level02.dash",
        "assets/levels/official/level03.dash",
        "assets/levels/official/level04.dash",
        "assets/levels/official/level05.dash",
        NULL
    };
    static const char* o_names[] = {
        "01 - Tutorial", "02 - Easy", "03 - Medium", "04 - Hard", "05 - Extreme", NULL
    };
    for (int i = 0; o_paths[i] && g->level_count < MAX_LEVELS; i++) {
        FILE* f = fopen(o_paths[i], "r");
        if (!f) continue;
        fclose(f);
        strncpy(g->level_list[g->level_count].path, o_paths[i], 255);
        strncpy(g->level_list[g->level_count].display_name, o_names[i], 127);
        g->level_list[g->level_count].is_custom = 0;
        g->level_count++;
    }

    DIR* dir = opendir("assets/levels/custom");
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL && g->level_count < MAX_LEVELS) {
            size_t len = strlen(ent->d_name);
            if (len < 6 || strcmp(ent->d_name + len - 5, ".dash") != 0) continue;
            char path[256];
            snprintf(path, 255, "assets/levels/custom/%s", ent->d_name);
            strncpy(g->level_list[g->level_count].path, path, 255);
            strncpy(g->level_list[g->level_count].display_name, ent->d_name, 127);
            size_t nl = strlen(g->level_list[g->level_count].display_name);
            if (nl >= 5) g->level_list[g->level_count].display_name[nl - 5] = '\0';
            g->level_list[g->level_count].is_custom = 1;
            g->level_count++;
        }
        closedir(dir);
    }
}

int game_init(Game* g, int fb_w, int fb_h) {
    memset(g, 0, sizeof(*g));

    if (!renderer_init(&g->renderer, fb_w, fb_h)) return 0;

    camera_init(&g->camera);
    renderer_set_camera(&g->renderer, &g->camera);

    particles_init(&g->particles);
    ui_init(&g->ui);

    /* Load default skin */
    skin_load(&g->skins[0], "assets/skins/default_cube", &g->renderer.cube_mesh);
    g->skin_count  = 1;
    g->active_skin = 0;

    /* Scan available levels */
    game_scan_levels(g);

    /* Load first level as default */
    const char* first = (g->level_count > 0)
        ? g->level_list[0].path
        : "assets/levels/official/level01.dash";
    game_load_level(g, first);
    g->current_level_index = 0;
    g->level_select_cursor = 0;
    g->level_select_scroll = 0;
    g->current_level_is_custom = (g->level_count > 0) ? g->level_list[0].is_custom : 0;

    player_init(&g->player, g->level.player_start);
    player_set_skin(&g->player, &g->skins[0]);

    g->mode        = GAME_MODE_MAIN_MENU;
    g->accumulator = 0.0f;

    return 1;
}

void game_shutdown(Game* g) {
    for (int i = 0; i < g->skin_count; i++) skin_destroy(&g->skins[i]);
    renderer_shutdown(&g->renderer);
}

void game_load_level(Game* g, const char* path) {
    strncpy(g->level_path, path, sizeof(g->level_path)-1);
    level_load(&g->level, path);
    g->death_count = 0;
}

void game_start_playing(Game* g) {
    player_respawn(&g->player);
    glm_vec3_copy(g->level.player_start, g->player.spawn_position);
    player_respawn(&g->player);
    particles_reset(&g->particles);
    g->mode = GAME_MODE_PLAYING;
    g->state_timer = 0.0f;
    g->was_grounded = 0;
    g->ui.lc_selection = 0;
}

void game_restart(Game* g) {
    game_start_playing(g);
}

void game_on_player_death(Game* g) {
    particles_emit_death(&g->particles, g->player.position);
    g->player.alive = 0;
    g->death_count++;
    g->mode = GAME_MODE_GAME_OVER;
    g->state_timer = 0.0f;
}

void game_on_level_complete(Game* g) {
    g->mode = GAME_MODE_LEVEL_COMPLETE;
    g->state_timer = 0.0f;
}

void game_resize(Game* g, int fb_w, int fb_h) {
    g->renderer.fb_width  = fb_w;
    g->renderer.fb_height = fb_h;
    g->renderer.aspect    = (fb_h > 0) ? (float)fb_w / (float)fb_h : 1.0f;
}

void game_update(Game* g, const Input* input, float frame_dt) {
    g->state_timer += frame_dt;

    /* Camera mode switch */
    if (input_just_pressed(input, KEY_F1)) g->camera.mode = CAMERA_MODE_SIDE;
    if (input_just_pressed(input, KEY_F2)) g->camera.mode = CAMERA_MODE_THIRD_PERSON;
    if (input_just_pressed(input, KEY_F3)) g->camera.mode = CAMERA_MODE_ORBIT;

    /* TAB enters editor only when playing a custom level */
    if (input_just_pressed(input, KEY_TAB) && g->current_level_is_custom) {
        if (g->mode == GAME_MODE_PLAYING || g->mode == GAME_MODE_PAUSED) {
            g->mode = GAME_MODE_EDITOR;
            return;
        }
    }

    switch (g->mode) {
    case GAME_MODE_MAIN_MENU: {
        int do_play = 0, do_quit = 0;
        ui_update_menu(&g->ui, input, &g->renderer, &do_play, &do_quit);
        if (do_play) g->mode = GAME_MODE_HOW_TO_PLAY;
        if (do_quit) g->should_quit = 1;
        break;
    }

    case GAME_MODE_HOW_TO_PLAY: {
        int htp_back=0, htp_cont=0;
        ui_update_how_to_play(&g->ui, input, &g->renderer, &htp_back, &htp_cont);
        if (htp_cont) g->mode = GAME_MODE_LEVEL_SELECT;
        if (htp_back) g->mode = GAME_MODE_MAIN_MENU;
        break;
    }

    case GAME_MODE_LEVEL_SELECT: {
        int sel = -1, back = 0, new_level = 0, open_folder = 0;
        ui_update_level_select(&g->ui, input, g, &sel, &back, &new_level, &open_folder);
        if (sel >= 0) {
            g->current_level_index = sel;
            g->current_level_is_custom = g->level_list[sel].is_custom;
            game_load_level(g, g->level_list[sel].path);
            game_start_playing(g);
        }
        if (new_level) g->wants_new_level = 1;
        if (open_folder) {
            game_scan_levels(g);          /* pick up any newly dropped files */
#ifdef _WIN32
            system("explorer assets\\levels\\custom");
#endif
        }
        if (back) g->mode = GAME_MODE_MAIN_MENU;
        break;
    }

    case GAME_MODE_PLAYING: {
        /* Capture jump at frame rate — never missed on high-refresh displays */
        /* Set jump buffer:
           - Any fresh press: always works (just_pressed).
           - Held while airborne: keeps buffer alive for auto-jump on landing.
           - Held while grounded: ignored (prevents auto-fire when entering level
             with SPACE already held from level-select confirm). */
        if (input_just_pressed(input, KEY_SPACE) ||
            (input_pressed(input, KEY_SPACE) && !g->player.grounded))
            g->player.jump_buffer = JUMP_BUFFER_TIME;

        /* Fixed-step physics accumulator */
        g->accumulator += frame_dt;
        while (g->accumulator >= FIXED_DT && g->mode == GAME_MODE_PLAYING) {
            game_fixed_update(g, input, FIXED_DT);
            g->accumulator -= FIXED_DT;
        }
        /* Visual rotation (non-physics, frame-rate) */
        player_update_visual(&g->player, frame_dt);
        particles_update(&g->particles, frame_dt);

        /* Camera orbit control in orbit mode */
        if (g->camera.mode == CAMERA_MODE_ORBIT) {
            double dx, dy;
            input_get_mouse_delta(input, &dx, &dy);
            if (input_mouse_pressed(input, MOUSE_LEFT))
                camera_update_orbit(&g->camera, (float)dx * 0.01f, (float)dy * 0.01f, 0.0f);
            camera_update_orbit(&g->camera, 0, 0, -(float)input_get_scroll(input) * 0.5f);
        }
        camera_snap_to_player(&g->camera, g->player.position);

        /* Restart key */
        if (input_just_pressed(input, KEY_R)) game_restart(g);
        if (input_just_pressed(input, KEY_ESCAPE)) g->mode = GAME_MODE_PAUSED;
        break;
    }

    case GAME_MODE_GAME_OVER:
        particles_update(&g->particles, frame_dt);
        if (g->state_timer > 1.5f)
            game_restart(g);
        break;

    case GAME_MODE_LEVEL_COMPLETE: {
        int retry=0, next_lv=0, exitmenu=0;
        int is_last   = (g->current_level_index + 1 >= g->level_count);
        int show_next = !g->current_level_is_custom && !is_last;
        ui_update_level_complete(&g->ui, input, &g->renderer,
                                 show_next, &retry, &next_lv, &exitmenu);
        if (retry) {
            game_restart(g);
        } else if (next_lv) {
            int nx = g->current_level_index + 1;
            g->current_level_index       = nx;
            g->current_level_is_custom   = g->level_list[nx].is_custom;
            game_load_level(g, g->level_list[nx].path);
            game_start_playing(g);
        } else if (exitmenu) {
            g->mode = GAME_MODE_MAIN_MENU;
        }
        break;
    }

    case GAME_MODE_PAUSED: {
        int resume=0, restart=0, main_menu=0, quit=0;
        ui_update_pause_menu(&g->ui, input, &g->renderer, &resume, &restart, &main_menu, &quit);
        if (resume || input_just_pressed(input, KEY_ESCAPE))
            g->mode = GAME_MODE_PLAYING;
        if (restart)   game_restart(g);
        if (main_menu) g->mode = GAME_MODE_MAIN_MENU;
        if (quit)      g->should_quit = 1;
        break;
    }

    case GAME_MODE_EDITOR:
        /* Handled by editor module in main */
        break;

    default: break;
    }
}

void game_fixed_update(Game* g, const Input* input, float dt) {
    if (!g->player.alive) return;

    int prev_grounded = g->player.grounded;
    player_fixed_update(&g->player, input, dt);
    resolve_collisions(g);
    if (!g->player.alive) return;

    /* Update level progress */
    float level_len = level_get_length(&g->level);
    if (level_len > 0.0f)
        g->player.level_pct = g->player.position[0] / level_len;

    /* Emit particles on state changes */
    if (!prev_grounded && g->player.grounded) {
        particles_emit_land(&g->particles, g->player.position);
    }
    if (!g->was_grounded && prev_grounded && !g->player.grounded) {
        /* just jumped */
        particles_emit_jump(&g->particles, g->player.position);
    }
    g->was_grounded = g->player.grounded;

    /* Trail */
    static int trail_tick = 0;
    if (++trail_tick >= 3) {
        particles_emit_trail(&g->particles, g->player.position);
        trail_tick = 0;
    }

    /* Level completion: player past end X */
    if (g->player.position[0] >= g->level.level_end[0]) {
        game_on_level_complete(g);
    }
}

static void resolve_collisions(Game* g) {
    Player* p = &g->player;
    p->grounded = 0;

    for (int i = 0; i < g->level.object_count; i++) {
        LevelObject* obj = &g->level.objects[i];
        if (!obj->solid && !obj->lethal && !obj->is_end) continue;

        if (!aabb_intersects(&p->collider, &obj->collider)) continue;

        if (obj->lethal) {
            snprintf(g->last_death_info, sizeof(g->last_death_info),
                "SPIKE @ x=%.2f y=%.2f  |  spike_box y=[%.2f,%.2f] x=[%.2f,%.2f]  |  player_box y=[%.2f,%.2f] x=[%.2f,%.2f]  |  player_vel_y=%.2f",
                obj->transform.position[0], obj->transform.position[1],
                obj->collider.min[1], obj->collider.max[1],
                obj->collider.min[0], obj->collider.max[0],
                p->collider.min[1], p->collider.max[1],
                p->collider.min[0], p->collider.max[0],
                p->velocity[1]);
            g->last_death_reason = DEATH_SPIKE;
            printf("[DEATH] %s\n", g->last_death_info);
            fflush(stdout);
            game_on_player_death(g);
            return;
        }
        if (obj->is_end) {
            game_on_level_complete(g);
            return;
        }

        /* Solid block: SAT resolution on Y then X */
        float overlap_x_pos = obj->collider.max[0] - p->collider.min[0];
        float overlap_x_neg = p->collider.max[0] - obj->collider.min[0];
        float overlap_y_pos = obj->collider.max[1] - p->collider.min[1];
        float overlap_y_neg = p->collider.max[1] - obj->collider.min[1];

        float ox = (overlap_x_pos < overlap_x_neg) ? overlap_x_pos : -overlap_x_neg;
        float oy = (overlap_y_pos < overlap_y_neg) ? overlap_y_pos : -overlap_y_neg;

        if (fabsf(oy) < fabsf(ox)) {
            /* resolve Y (floor / ceiling) */
            p->position[1] += oy;
            if (oy > 0 && p->velocity[1] < 0) { p->velocity[1] = 0; p->grounded = 1; }
            else if (oy < 0 && p->velocity[1] > 0) { p->velocity[1] = 0; }
        } else {
            /* X axis has smaller overlap.
               Only a genuine wall hit if the player is moving TOWARD the block face.
               ox > 0 means "push player right" (block is to the left of player);
               ox < 0 means "push player left" (block is to the right of player).
               Wall kill when velocity opposes resolution: vel_x * ox < 0.
               If vel_x * ox >= 0, player is at a block corner sliding off the top —
               treat as Y resolution to avoid phantom deaths on floor edges. */
            if (p->velocity[0] * ox < -0.001f) {
                snprintf(g->last_death_info, sizeof(g->last_death_info),
                    "WALL @ x=%.2f y=%.2f  |  ox=%.3f oy=%.3f  |  player x=%.2f y=%.2f  vel_x=%.2f",
                    obj->transform.position[0], obj->transform.position[1],
                    ox, oy, p->position[0], p->position[1], p->velocity[0]);
                g->last_death_reason = DEATH_WALL;
                printf("[DEATH] %s\n", g->last_death_info);
                fflush(stdout);
                game_on_player_death(g);
                return;
            }
            /* Corner/edge case: resolve Y instead */
            p->position[1] += oy;
            if (oy > 0 && p->velocity[1] < 0) { p->velocity[1] = 0; p->grounded = 1; }
            else if (oy < 0 && p->velocity[1] > 0) { p->velocity[1] = 0; }
        }
        player_update_collider(p);
    }

    /* Kill player if fallen below the world */
    if (p->alive && p->position[1] < -10.0f) {
        snprintf(g->last_death_info, sizeof(g->last_death_info),
            "FELL below world  |  player_y=%.2f  vel_y=%.2f",
            p->position[1], p->velocity[1]);
        g->last_death_reason = DEATH_FALL;
        printf("[DEATH] %s\n", g->last_death_info);
        fflush(stdout);
        game_on_player_death(g);
    }
}

static void render_level_objects(Game* g) {
    for (int i = 0; i < g->level.object_count; i++) {
        LevelObject* obj = &g->level.objects[i];
        Material mat;
        mat.diffuse = NULL;

        switch (obj->type) {
        case LEVEL_OBJECT_BLOCK:
            glm_vec4_copy((vec4){0.5f, 0.5f, 0.55f, 1.0f}, mat.color);
            renderer_draw_mesh(&g->renderer, &g->renderer.cube_mesh, &mat, obj->transform);
            break;
        case LEVEL_OBJECT_SPIKE:
            glm_vec4_copy((vec4){1.0f, 0.3f, 0.2f, 1.0f}, mat.color);
            renderer_draw_mesh(&g->renderer, &g->renderer.spike_mesh, &mat, obj->transform);
            break;
        case LEVEL_OBJECT_END:
            glm_vec4_copy((vec4){0.2f, 1.0f, 0.3f, 0.7f}, mat.color);
            renderer_draw_mesh(&g->renderer, &g->renderer.cube_mesh, &mat, obj->transform);
            break;
        case LEVEL_OBJECT_DECORATION: {
            int ci = obj->color_idx & 7;
            mat.color[0] = s_deco_palette[ci][0];
            mat.color[1] = s_deco_palette[ci][1];
            mat.color[2] = s_deco_palette[ci][2];
            mat.color[3] = 1.0f;
            renderer_draw_mesh(&g->renderer, &g->renderer.cube_mesh, &mat, obj->transform);
            break;
        }
        default: break;
        }
    }
}

void game_render(Game* g) {
    renderer_begin(&g->renderer, g->renderer.fb_width, g->renderer.fb_height);

    switch (g->mode) {
    case GAME_MODE_MAIN_MENU:
        render_level_objects(g);
        ui_draw_main_menu(&g->ui, &g->renderer);
        break;

    case GAME_MODE_HOW_TO_PLAY:
        ui_draw_how_to_play(&g->ui, &g->renderer);
        break;

    case GAME_MODE_LEVEL_SELECT:
        ui_draw_level_select(&g->ui, &g->renderer, g);
        break;

    case GAME_MODE_PAUSED:
        render_level_objects(g);
        player_render(&g->player, &g->renderer);
        ui_draw_pause_menu(&g->ui, &g->renderer);
        break;

    case GAME_MODE_LEVEL_COMPLETE: {
        render_level_objects(g);
        player_render(&g->player, &g->renderer);
        particles_render(&g->particles, &g->renderer);
        int is_last   = (g->current_level_index + 1 >= g->level_count);
        int show_next = !g->current_level_is_custom && !is_last;
        ui_draw_level_complete(&g->ui, &g->renderer, show_next);
        break;
    }

    case GAME_MODE_PLAYING: {
        render_level_objects(g);
        player_render(&g->player, &g->renderer);
        particles_render(&g->particles, &g->renderer);

        /* Death counter — top-left */
        char death_str[32];
        snprintf(death_str, sizeof(death_str), "Deaths: %d", g->death_count);
        renderer_draw_text(&g->renderer, 12.0f, 12.0f, 2.0f, death_str,
                           (vec4){1.0f, 0.45f, 0.45f, 1.0f});

        /* Progress bar — bottom of screen */
        float pct = g->player.level_pct;
        if (pct < 0.0f) pct = 0.0f;
        if (pct > 1.0f) pct = 1.0f;
        /* background */
        renderer_draw_rect(&g->renderer, 0.0f, -0.93f, 1.80f, 0.045f,
                           (vec4){0.10f, 0.10f, 0.10f, 0.70f});
        /* fill */
        if (pct > 0.001f) {
            float fw = 1.80f * pct;
            renderer_draw_rect(&g->renderer, -0.90f + fw * 0.5f, -0.93f, fw, 0.045f,
                               (vec4){0.20f, 0.90f, 0.30f, 0.90f});
        }
        break;
    }

    default:
        render_level_objects(g);
        player_render(&g->player, &g->renderer);
        particles_render(&g->particles, &g->renderer);
        break;
    }

    renderer_end(&g->renderer);
}
