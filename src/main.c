#include "core/window.h"
#include "core/input.h"
#include "core/time.h"
#include "game/game.h"
#include "editor/editor.h"
#include <stdio.h>
#include <string.h>

#define WINDOW_W 1280
#define WINDOW_H  720
#define FIXED_DT_MAIN (1.0f / 60.0f)

int main(void) {
    Window window;
    if (!window_init(&window, WINDOW_W, WINDOW_H, "DashForge 3D")) return 1;
    window_set_icon(&window, "assets/icon.png");

    Input    input;
    GameTime time;
    Game     game;
    Editor   editor;

    input_init(&input, window_handle(&window));
    time_init(&time);

    if (!game_init(&game, window.fb_width, window.fb_height)) {
        fprintf(stderr, "Failed to init game\n");
        window_shutdown(&window);
        return 1;
    }

    /* Set up editor with game's level and camera */
    editor_init(&editor, &game.level, &game.camera);
    strcpy(editor.current_file, game.level_path);

#ifndef NDEBUG
    printf("DashForge 3D initialized.\n");
    printf("Controls:\n");
    printf("  SPACE  - Jump\n");
    printf("  R      - Restart level\n");
    printf("  TAB    - Toggle editor\n");
    printf("  F1/F2/F3 - Camera mode (side/3rd-person/orbit)\n");
    printf("  ESC    - Pause\n");
    printf("  Editor: LClick=place, RClick=erase, 1-4=type, Ctrl+S=save\n");
#endif

    while (!window_should_close(&window) && !game.should_quit) {
        window_poll_events();
        input_update(&input);
        time_update(&time);

        float dt = time_get_delta_seconds(&time);

        /* Route editor vs game updates */
        if (game.mode == GAME_MODE_EDITOR) {
            game.camera.mode = CAMERA_MODE_SIDE;
            editor.level     = &game.level;
            editor.camera    = &game.camera;
            editor_update(&editor, &input, &game.renderer, dt);
            /* TAB: test the level; ESC: back to main menu */
            if (input_just_pressed(&input, KEY_TAB) && !editor.save_dialog_open) {
                game.mode = GAME_MODE_PLAYING;
                game_restart(&game);
            }
            if (input_just_pressed(&input, KEY_ESCAPE) && !editor.save_dialog_open)
                game.mode = GAME_MODE_MAIN_MENU;
            /* Rescan level list if a new custom level was just saved */
            if (editor.level_saved) {
                editor.level_saved = 0;
                game_scan_levels(&game);
            }
        } else {
            game_update(&game, &input, dt);
            /* Level select requested a new blank level */
            if (game.wants_new_level) {
                game.wants_new_level         = 0;
                game.current_level_is_custom = 1;
                game.death_count             = 0;
                level_init(&game.level);
                editor_shutdown(&editor);
                editor_init(&editor, &game.level, &game.camera);
                game.mode = GAME_MODE_EDITOR;
            }
        }

        /* Update game_resize if window changed */
        game_resize(&game, window.fb_width, window.fb_height);

        /* Render */
        if (game.mode == GAME_MODE_EDITOR) {
            renderer_begin(&game.renderer, window.fb_width, window.fb_height);
            renderer_set_camera(&game.renderer, &game.camera);
            editor_render(&editor, &game.renderer);
            renderer_end(&game.renderer);
        } else {
            renderer_set_camera(&game.renderer, &game.camera);
            game_render(&game);
        }

#ifndef NDEBUG
        /* Update window title with debug info */
        static int title_tick = 0;
        if (++title_tick >= 30) {
            title_tick = 0;
            char title[256];
            const char* mode_str;
            switch (game.mode) {
            case GAME_MODE_MAIN_MENU:      mode_str = "MAIN MENU [SPACE]";     break;
            case GAME_MODE_PLAYING:        mode_str = "PLAYING";                break;
            case GAME_MODE_EDITOR:         mode_str = "EDITOR [1-4 type]";     break;
            case GAME_MODE_PAUSED:         mode_str = "PAUSED [ESC]";          break;
            case GAME_MODE_GAME_OVER:      mode_str = "DEAD [R]";              break;
            case GAME_MODE_LEVEL_COMPLETE: mode_str = "COMPLETE! [R]";         break;
            default:                       mode_str = "...";                   break;
            }
            const char* cam_str;
            switch (game.camera.mode) {
            case CAMERA_MODE_SIDE:         cam_str = "Side";  break;
            case CAMERA_MODE_THIRD_PERSON: cam_str = "3rd";   break;
            case CAMERA_MODE_ORBIT:        cam_str = "Orbit"; break;
            default:                       cam_str = "?";     break;
            }
            if (game.mode == GAME_MODE_GAME_OVER && game.last_death_info[0]) {
                snprintf(title, sizeof(title), "DashForge 3D | DEAD | %s",
                         game.last_death_info);
            } else {
                snprintf(title, sizeof(title),
                         "DashForge 3D | %s | Cam:%s | FPS:%.0f | x=%.1f",
                         mode_str, cam_str, time_get_fps(&time),
                         game.player.position[0]);
            }
            window_set_title(&window, title);
        }
#endif

        window_swap_buffers(&window);
    }

    editor_shutdown(&editor);
    game_shutdown(&game);
    window_shutdown(&window);
    return 0;
}
