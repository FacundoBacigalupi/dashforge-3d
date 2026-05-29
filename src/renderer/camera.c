#include "camera.h"
#include <math.h>

void camera_init(Camera* c) {
    c->mode         = CAMERA_MODE_SIDE;
    glm_vec3_copy((vec3){0,3,10}, c->position);
    glm_vec3_copy((vec3){0,0,0},  c->target);
    glm_vec3_copy((vec3){0,1,0},  c->up);
    c->fov_degrees   = 60.0f;
    c->near_plane    = 0.1f;
    c->far_plane     = 300.0f;
    c->orbit_angle   = 0.0f;
    c->orbit_radius  = 12.0f;
    c->orbit_height  = 5.0f;
}

void camera_get_view(const Camera* c, mat4 out) {
    glm_lookat((float*)c->position, (float*)c->target, (float*)c->up, out);
}

void camera_get_projection(const Camera* c, float aspect, mat4 out) {
    glm_perspective(glm_rad(c->fov_degrees), aspect, c->near_plane, c->far_plane, out);
}

void camera_update_orbit(Camera* c, float dangle, float dheight, float dradius) {
    c->orbit_angle  += dangle;
    c->orbit_height += dheight;
    c->orbit_radius += dradius;
    if (c->orbit_radius < 3.0f)  c->orbit_radius = 3.0f;
    if (c->orbit_radius > 50.0f) c->orbit_radius = 50.0f;
    if (c->orbit_height < -5.0f) c->orbit_height = -5.0f;
    if (c->orbit_height > 20.0f) c->orbit_height = 20.0f;
}

void camera_snap_to_player(Camera* c, vec3 player_pos) {
    switch (c->mode) {
    case CAMERA_MODE_SIDE:
        /* Flat 2D: look straight on, player on left third of screen */
        c->position[0] = player_pos[0] + 4.0f;
        c->position[1] = player_pos[1] + 2.0f;
        c->position[2] = 12.0f;
        c->target[0]   = player_pos[0] + 4.0f; /* same X = no horizontal drift */
        c->target[1]   = player_pos[1] + 2.0f; /* same Y = perfectly flat, no tilt */
        c->target[2]   = 0.0f;
        break;
    case CAMERA_MODE_THIRD_PERSON:
        c->position[0] = player_pos[0] - 7.0f;
        c->position[1] = player_pos[1] + 3.0f;
        c->position[2] = player_pos[2];
        c->target[0]   = player_pos[0] + 4.0f;
        c->target[1]   = player_pos[1] + 1.0f;
        c->target[2]   = player_pos[2];
        break;
    case CAMERA_MODE_ORBIT:
        c->position[0] = player_pos[0] + cosf(c->orbit_angle) * c->orbit_radius;
        c->position[2] = player_pos[2] + sinf(c->orbit_angle) * c->orbit_radius;
        c->position[1] = player_pos[1] + c->orbit_height;
        glm_vec3_copy(player_pos, c->target);
        break;
    }
}
