#include "level_object.h"

void level_object_rebuild_collider(LevelObject* obj) {
    /* For spikes: shrink collider slightly so players can clear edges */
    if (obj->type == LEVEL_OBJECT_SPIKE) {
        /* Tight hitbox: narrow (pyramid is pointy) and shifted toward the
           base so the tip doesn't cause phantom deaths. */
        vec3 shrunk = {
            obj->transform.scale[0] * 0.42f,
            obj->transform.scale[1] * 0.55f,
            obj->transform.scale[2] * 0.42f
        };
        /* Shift center toward the base (negative Y) so collider sits
           in the wide lower portion of the pyramid, not the narrow tip. */
        vec3 center = {
            obj->transform.position[0],
            obj->transform.position[1] - obj->transform.scale[1] * 0.15f,
            obj->transform.position[2]
        };
        aabb_from_transform(&obj->collider, center, shrunk);
    } else {
        aabb_from_transform(&obj->collider,
                            obj->transform.position,
                            obj->transform.scale);
    }
}
