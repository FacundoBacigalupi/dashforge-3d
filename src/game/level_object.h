#pragma once
#include "renderer/renderer.h"
#include "physics/collision.h"

typedef enum {
    LEVEL_OBJECT_BLOCK,
    LEVEL_OBJECT_SPIKE,
    LEVEL_OBJECT_START,
    LEVEL_OBJECT_END,
    LEVEL_OBJECT_DECORATION
} LevelObjectType;

typedef struct LevelObject {
    LevelObjectType type;
    Transform       transform;
    AABB            collider;
    int             solid;
    int             lethal;
    int             is_end;
    int             color_idx;  /* palette index, used by DECORATION */
} LevelObject;

void level_object_rebuild_collider(LevelObject* obj);
