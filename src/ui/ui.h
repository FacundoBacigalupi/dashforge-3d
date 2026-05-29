#pragma once
#include "renderer/renderer.h"
#include "core/input.h"

struct Game;

typedef struct UI {
    int menu_selection;   /* main menu: 0=Play, 1=Quit */
    int pause_selection;  /* pause menu: 0=Resume 1=Restart 2=MainMenu 3=Quit */
    int lc_selection;     /* level complete: index into visible option list */
    int htp_selection;    /* how-to-play: 0=Back, 1=Continue */
} UI;

void ui_init(UI* ui);
void ui_render_hud(UI* ui, struct Game* g);

/* Main menu */
void ui_update_menu(UI* ui, const Input* input, Renderer* r, int* out_play, int* out_quit);
void ui_draw_main_menu(UI* ui, Renderer* r);

/* How to play */
void ui_draw_how_to_play(UI* ui, Renderer* r);
void ui_update_how_to_play(UI* ui, const Input* input, Renderer* r,
                            int* out_back, int* out_continue);

/* Level select */
void ui_draw_level_select(UI* ui, Renderer* r, struct Game* g);
void ui_update_level_select(UI* ui, const Input* input, struct Game* g,
                             int* out_selected, int* out_back,
                             int* out_new_level, int* out_open_folder);

/* Level complete overlay.
   show_next = 1 when it's an official level that's not the last. */
void ui_draw_level_complete(UI* ui, Renderer* r, int show_next);
void ui_update_level_complete(UI* ui, const Input* input, Renderer* r,
                               int show_next,
                               int* out_retry, int* out_next, int* out_exit);

/* Pause menu */
void ui_draw_pause_menu(UI* ui, Renderer* r);
void ui_update_pause_menu(UI* ui, const Input* input, Renderer* r,
                           int* out_resume, int* out_restart,
                           int* out_main_menu, int* out_quit);
