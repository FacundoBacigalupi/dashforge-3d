#include "player.h"
#include <math.h>
#include <string.h>

static float snap_to_90(float deg) {
    return roundf(deg / 90.0f) * 90.0f;
}

void player_init(Player* p, vec3 spawn) {
    memset(p, 0, sizeof(*p));
    glm_vec3_copy(spawn, p->spawn_position);
    p->width   = 0.9f;
    p->height  = 0.9f;
    p->depth   = 0.9f;
    p->alive   = 1;
    p->skin    = NULL;
    player_respawn(p);
}

void player_respawn(Player* p) {
    glm_vec3_copy(p->spawn_position, p->position);
    glm_vec3_zero(p->velocity);
    p->grounded         = 0;
    p->alive            = 1;
    p->visual_rotation_z = 0.0f;
    p->level_time       = 0.0f;
    p->level_pct        = 0.0f;
    p->jump_buffer      = 0.0f;
    player_update_collider(p);
}

void player_fixed_update(Player* p, const Input* input, float dt) {
    (void)input; /* jump buffer set at frame rate in game_update */
    if (!p->alive) return;

    p->velocity[0] = RUN_SPEED;
    p->velocity[1] += GRAVITY * dt;

    if (p->grounded && p->jump_buffer > 0.0f) {
        p->velocity[1] = JUMP_FORCE;
        p->grounded     = 0;
        p->jump_buffer  = 0.0f;
    }

    p->jump_buffer -= dt;
    if (p->jump_buffer < 0.0f) p->jump_buffer = 0.0f;

    p->position[0] += p->velocity[0] * dt;
    p->position[1] += p->velocity[1] * dt;

    p->level_time += dt;
    player_update_collider(p);
}

void player_update_visual(Player* p, float dt) {
    if (!p->grounded) {
        /* ~360 degrees per second */
        p->visual_rotation_z -= 360.0f * dt;
    } else {
        p->visual_rotation_z = snap_to_90(p->visual_rotation_z);
    }
}

void player_update_collider(Player* p) {
    vec3 size = {p->width, p->height, p->depth};
    aabb_from_transform(&p->collider, p->position, size);
}

void player_set_skin(Player* p, Skin* skin) {
    p->skin = skin;
}

void player_render(Player* p, Renderer* r) {
    if (!p->alive) return;

    Transform t = transform_default();
    glm_vec3_copy(p->position, t.position);
    t.rotation_euler[2] = p->visual_rotation_z;
    glm_vec3_copy((vec3){p->width, p->height, p->depth}, t.scale);

    Material mat;
    if (p->skin) {
        mat.diffuse = p->skin->has_texture ? &p->skin->texture : NULL;
        glm_vec4_copy(p->skin->color, mat.color);
        Mesh* mesh = p->skin->mesh ? p->skin->mesh : &r->cube_mesh;
        renderer_draw_mesh(r, mesh, &mat, t);
    } else {
        mat.diffuse = NULL;
        glm_vec4_copy((vec4){0.3f, 0.6f, 1.0f, 1.0f}, mat.color);
        renderer_draw_mesh(r, &r->cube_mesh, &mat, t);
    }

#if DEBUG_RENDER_COLLIDERS
    renderer_draw_debug_aabb(r,
        p->collider.min[0], p->collider.min[1], p->collider.min[2],
        p->collider.max[0], p->collider.max[1], p->collider.max[2]);
#endif
}
