#pragma once
#include "game/level.h"
#include "renderer/renderer.h"
#include "renderer/camera.h"
#include "core/input.h"
#include "editor/grid.h"

typedef struct Editor {
    Level*          level;
    Camera*         camera;
    Grid            grid;
    LevelObjectType selected_type;
    float           camera_offset_x;
    float           camera_offset_y;
    float           cursor_x;
    float           cursor_y;
    int             cursor_grid_x;
    int             cursor_grid_y;
    int             snap_size;
    char            current_file[256];
    int             unsaved;
    int             deco_color_idx; /* 0-7 palette */
    /* save-as dialog */
    int             save_dialog_open;
    char            name_buf[64];
    int             name_buf_len;
    int             is_new_level;        /* 1 = never saved yet */
    int             level_saved;         /* set to 1 after a successful save */
    int             save_name_conflict;  /* 1 = file with that name already exists */
} Editor;

void editor_init(Editor* e, Level* level, Camera* camera);
void editor_shutdown(Editor* e);
void editor_update(Editor* e, const Input* input, Renderer* r, float dt);
void editor_render(Editor* e, Renderer* r);

void editor_save(Editor* e);
void editor_load(Editor* e, const char* path);
