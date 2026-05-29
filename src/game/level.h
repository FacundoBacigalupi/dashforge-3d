#pragma once
#include "level_object.h"

#define MAX_LEVEL_OBJECTS 4096

typedef struct Level {
    char        name[128];
    vec3        player_start;
    vec3        level_end;
    LevelObject objects[MAX_LEVEL_OBJECTS];
    int         object_count;
} Level;

void level_init(Level* level);
int  level_load(Level* level, const char* path);
int  level_save(const Level* level, const char* path);
int  level_add_object(Level* level, LevelObject object);
void level_remove_object_at(Level* level, int index);
void level_rebuild_colliders(Level* level);
float level_get_length(const Level* level);
