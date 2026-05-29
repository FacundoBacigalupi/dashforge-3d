#pragma once
#include <cglm/cglm.h>

typedef enum {
    CAMERA_MODE_SIDE,
    CAMERA_MODE_THIRD_PERSON,
    CAMERA_MODE_ORBIT
} CameraMode;

typedef struct Camera {
    CameraMode mode;
    vec3       position;
    vec3       target;
    vec3       up;
    float      fov_degrees;
    float      near_plane;
    float      far_plane;
    float      orbit_angle;
    float      orbit_radius;
    float      orbit_height;
} Camera;

void camera_init(Camera* c);
void camera_get_view(const Camera* c, mat4 out);
void camera_get_projection(const Camera* c, float aspect, mat4 out);
void camera_update_orbit(Camera* c, float dangle, float dheight, float dradius);
void camera_snap_to_player(Camera* c, vec3 player_pos);
