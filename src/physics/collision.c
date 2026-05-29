#include "collision.h"
#include <math.h>

void aabb_from_transform(AABB* out, vec3 pos, vec3 size) {
    out->min[0] = pos[0] - size[0] * 0.5f;
    out->min[1] = pos[1] - size[1] * 0.5f;
    out->min[2] = pos[2] - size[2] * 0.5f;
    out->max[0] = pos[0] + size[0] * 0.5f;
    out->max[1] = pos[1] + size[1] * 0.5f;
    out->max[2] = pos[2] + size[2] * 0.5f;
}

int aabb_intersects(const AABB* a, const AABB* b) {
    return (a->min[0] <= b->max[0] && a->max[0] >= b->min[0]) &&
           (a->min[1] <= b->max[1] && a->max[1] >= b->min[1]) &&
           (a->min[2] <= b->max[2] && a->max[2] >= b->min[2]);
}
