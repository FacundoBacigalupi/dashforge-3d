#pragma once
#include "player.h"
#include "level.h"
#include "renderer/renderer.h"
#include "renderer/camera.h"
#include "core/input.h"
#include "particles/particles.h"
#include "ui/ui.h"

typedef enum {
    GAME_MODE_MAIN_MENU,
    GAME_MODE_HOW_TO_PLAY,
    GAME_MODE_LEVEL_SELECT,
    GAME_MODE_PLAYING,
    GAME_MODE_EDITOR,
    GAME_MODE_PAUSED,
    GAME_MODE_GAME_OVER,
    GAME_MODE_LEVEL_COMPLETE
} GameMode;

typedef enum {
    DEATH_NONE = 0,
    DEATH_SPIKE,
    DEATH_WALL,
    DEATH_FALL,
} DeathReason;

#define MAX_LEVELS 64
#define MAX_SKINS  16

typedef struct LevelEntry {
    char path[256];
    char display_name[128];
    int  is_custom;
} LevelEntry;

typedef struct Game {
    GameMode        mode;
    Player          player;
    Level           level;
    Renderer        renderer;
    Camera          camera;
    ParticleSystem  particles;
    UI              ui;
    Skin            skins[MAX_SKINS];
    int             skin_count;
    int             active_skin;
    float           accumulator;
    float           state_timer;
    char            level_path[256];
    int             was_grounded;
    int             should_quit;

    /* Level list */
    LevelEntry      level_list[MAX_LEVELS];
    int             level_count;
    int             current_level_index;
    int             level_select_cursor;
    int             level_select_scroll;

    /* Death debug */
    DeathReason     last_death_reason;
    char            last_death_info[256];

    /* HUD / editor state */
    int             death_count;            /* deaths since last level load */
    int             current_level_is_custom;/* 1 = custom/new level (TAB→editor allowed) */
    int             wants_new_level;        /* set by level select; handled in main.c */
} Game;

int  game_init(Game* g, int fb_w, int fb_h);
void game_scan_levels(Game* g);
void game_shutdown(Game* g);
void game_update(Game* g, const Input* input, float frame_dt);
void game_fixed_update(Game* g, const Input* input, float dt);
void game_render(Game* g);
void game_load_level(Game* g, const char* path);
void game_start_playing(Game* g);
void game_restart(Game* g);
void game_on_player_death(Game* g);
void game_on_level_complete(Game* g);
void game_resize(Game* g, int fb_w, int fb_h);
