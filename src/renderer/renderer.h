#pragma once
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include <cglm/cglm.h>

#define DEBUG_RENDER_COLLIDERS 0

typedef struct Transform {
    vec3 position;
    vec3 rotation_euler; /* degrees */
    vec3 scale;
} Transform;

typedef struct Material {
    Texture* diffuse;
    vec4     color;  /* rgba, multiplied with texture */
} Material;

typedef struct Renderer {
    Shader  basic_shader;
    Shader  particle_shader;
    Shader  grid_shader;
    Shader  text_shader;
    Mesh    cube_mesh;
    Mesh    spike_mesh;
    Mesh    quad_mesh;
    Camera* camera;
    int     fb_width;
    int     fb_height;
    float   aspect;
    Texture white_tex;
    /* text rendering (stb_easy_font) */
    GLuint  text_vao;
    GLuint  text_vbo;
    GLuint  text_ebo;
} Renderer;

int  renderer_init(Renderer* r, int width, int height);
void renderer_shutdown(Renderer* r);
void renderer_begin(Renderer* r, int fb_w, int fb_h);
void renderer_end(Renderer* r);
void renderer_set_camera(Renderer* r, Camera* c);

void renderer_draw_mesh(Renderer* r, Mesh* mesh, Material* mat, Transform transform);
void renderer_draw_debug_aabb(Renderer* r, float minx, float miny, float minz,
                               float maxx, float maxy, float maxz);
/* 2D rect in NDC space: cx,cy = center (-1..1), w,h = size in NDC units */
void renderer_draw_rect(Renderer* r, float cx, float cy, float w, float h, vec4 color);

/* Text at pixel position (top-left origin, Y down) */
void renderer_draw_text(Renderer* r, float px, float py, float scale,
                        const char* text, vec4 color);
/* Text centered at an NDC coordinate */
void renderer_draw_text_ndc(Renderer* r, float ndc_x, float ndc_y, float scale,
                             const char* text, vec4 color);

Transform transform_default(void);
