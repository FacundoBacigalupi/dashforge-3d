#pragma once
#include "physics/collision.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"
#include "core/input.h"
#include "game/skin.h"

#define FIXED_DT         (1.0f / 60.0f)
#define GRAVITY          (-38.0f)
#define JUMP_FORCE        14.5f
#define RUN_SPEED         8.0f
#define JUMP_BUFFER_TIME  0.12f

typedef struct Player {
    vec3    position;
    vec3    velocity;
    vec3    spawn_position;
    float   width;
    float   height;
    float   depth;
    int     grounded;
    int     alive;
    float   visual_rotation_z; /* degrees */
    AABB    collider;
    Skin*   skin;
    float   level_time;   /* seconds since level start */
    float   level_pct;    /* 0..1 progress */
    float   jump_buffer;  /* seconds remaining for buffered jump input */
} Player;

void player_init(Player* p, vec3 spawn);
void player_respawn(Player* p);
void player_fixed_update(Player* p, const Input* input, float dt);
void player_update_visual(Player* p, float dt);
void player_update_collider(Player* p);
void player_set_skin(Player* p, Skin* skin);
void player_render(Player* p, Renderer* r);
