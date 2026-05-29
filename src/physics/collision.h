#pragma once
#include <cglm/cglm.h>

typedef struct AABB {
    vec3 min;
    vec3 max;
} AABB;

void aabb_from_transform(AABB* out, vec3 pos, vec3 size);
int  aabb_intersects(const AABB* a, const AABB* b);

/* resolve_result: bit 0 = hit_x, bit 1 = hit_y (floor), bit 2 = hit_y_ceil, bit 3 = lethal */
typedef struct CollisionResult {
    int   grounded;
    int   blocked_x;
    int   blocked_ceil;
    int   lethal;
    float resolve_y;
    float resolve_x;
} CollisionResult;
