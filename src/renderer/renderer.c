#include "renderer.h"
#include <glad/glad.h>
#include <stdio.h>
#include <math.h>

#define STB_EASY_FONT_IMPLEMENTATION
#include <stb_easy_font.h>

int renderer_init(Renderer* r, int width, int height) {
    r->fb_width  = width;
    r->fb_height = height;
    r->aspect    = (float)width / (float)height;
    r->camera    = NULL;

    if (!shader_load(&r->basic_shader,    "assets/shaders/basic.vert",    "assets/shaders/basic.frag"))    return 0;
    if (!shader_load(&r->particle_shader, "assets/shaders/particle.vert", "assets/shaders/particle.frag")) return 0;
    if (!shader_load(&r->grid_shader,     "assets/shaders/grid.vert",     "assets/shaders/grid.frag"))     return 0;
    if (!shader_load(&r->text_shader,     "assets/shaders/text.vert",     "assets/shaders/text.frag"))     return 0;

    /* Text VAO: stb_easy_font vertex = float x, y, z + uchar rgba = 16 bytes.
       We read only x,y at location 0, stride 16. */
    glGenVertexArrays(1, &r->text_vao);
    glGenBuffers(1, &r->text_vbo);
    glGenBuffers(1, &r->text_ebo);
    glBindVertexArray(r->text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, r->text_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->text_ebo);
    glBindVertexArray(0);

    mesh_make_cube(&r->cube_mesh);
    mesh_make_spike(&r->spike_mesh);
    mesh_make_quad(&r->quad_mesh);

    texture_make_solid(&r->white_tex, 255, 255, 255, 255);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return 1;
}

void renderer_shutdown(Renderer* r) {
    shader_destroy(&r->basic_shader);
    shader_destroy(&r->particle_shader);
    shader_destroy(&r->grid_shader);
    shader_destroy(&r->text_shader);
    mesh_destroy(&r->cube_mesh);
    mesh_destroy(&r->spike_mesh);
    mesh_destroy(&r->quad_mesh);
    texture_destroy(&r->white_tex);
    glDeleteVertexArrays(1, &r->text_vao);
    glDeleteBuffers(1, &r->text_vbo);
    glDeleteBuffers(1, &r->text_ebo);
}

void renderer_begin(Renderer* r, int fb_w, int fb_h) {
    r->fb_width  = fb_w;
    r->fb_height = fb_h;
    r->aspect    = (fb_h > 0) ? (float)fb_w / (float)fb_h : 1.0f;
    glViewport(0, 0, fb_w, fb_h);
    glClearColor(0.08f, 0.08f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_end(Renderer* r) {
    (void)r;
    glUseProgram(0);
}

void renderer_set_camera(Renderer* r, Camera* c) {
    r->camera = c;
}

static void build_model(mat4 out, Transform t) {
    glm_mat4_identity(out);
    glm_translate(out, t.position);
    glm_rotate(out, glm_rad(t.rotation_euler[0]), (vec3){1,0,0});
    glm_rotate(out, glm_rad(t.rotation_euler[1]), (vec3){0,1,0});
    glm_rotate(out, glm_rad(t.rotation_euler[2]), (vec3){0,0,1});
    glm_scale(out, t.scale);
}

void renderer_draw_mesh(Renderer* r, Mesh* mesh, Material* mat, Transform transform) {
    if (!r->camera) return;

    mat4 model, view, proj;
    build_model(model, transform);
    camera_get_view(r->camera, view);
    camera_get_projection(r->camera, r->aspect, proj);

    /* normal matrix: transpose(inverse(model_3x3)) */
    mat3 normal_mat;
    glm_mat4_pick3(model, normal_mat);
    glm_mat3_inv(normal_mat, normal_mat);
    glm_mat3_transpose(normal_mat);

    shader_use(&r->basic_shader);
    shader_set_mat4(&r->basic_shader, "u_model",       model);
    shader_set_mat4(&r->basic_shader, "u_view",        view);
    shader_set_mat4(&r->basic_shader, "u_projection",  proj);
    shader_set_mat3(&r->basic_shader, "u_normal_matrix", normal_mat);

    /* Light comes from upper-right and toward the viewer (-Z) so the
       front face (+Z normal, visible from the 2D side camera) gets diffuse. */
    vec3 light_dir = {0.4f, -0.7f, -0.8f};
    glm_vec3_normalize(light_dir);
    shader_set_vec3(&r->basic_shader, "u_light_dir",    light_dir);
    shader_set_vec3(&r->basic_shader, "u_light_color",  (vec3){0.75f, 0.75f, 0.75f});
    shader_set_vec3(&r->basic_shader, "u_ambient_color",(vec3){0.35f, 0.35f, 0.45f});

    if (mat && mat->diffuse) {
        texture_bind(mat->diffuse, 0);
        shader_set_int(&r->basic_shader, "u_use_texture", 1);
    } else {
        texture_bind(&r->white_tex, 0);
        shader_set_int(&r->basic_shader, "u_use_texture", 0);
    }
    shader_set_int(&r->basic_shader, "u_texture", 0);

    vec4 color = {1,1,1,1};
    if (mat) glm_vec4_copy(mat->color, color);
    shader_set_vec4(&r->basic_shader, "u_color", color);

    mesh_draw(mesh);
}

void renderer_draw_debug_aabb(Renderer* r,
    float minx, float miny, float minz,
    float maxx, float maxy, float maxz)
{
    if (!r->camera) return;
    /* Build a wireframe box using line loop — quick approach */
    float w = maxx - minx, h = maxy - miny, d = maxz - minz;
    Transform t;
    glm_vec3_copy((vec3){(minx+maxx)*0.5f,(miny+maxy)*0.5f,(minz+maxz)*0.5f}, t.position);
    glm_vec3_copy((vec3){0,0,0}, t.rotation_euler);
    glm_vec3_copy((vec3){w, h, d}, t.scale);

    mat4 model, view, proj;
    build_model(model, t);
    camera_get_view(r->camera, view);
    camera_get_projection(r->camera, r->aspect, proj);

    shader_use(&r->grid_shader);
    glUniformMatrix4fv(glGetUniformLocation(r->grid_shader.program, "u_view"),       1, GL_FALSE, view[0]);
    glUniformMatrix4fv(glGetUniformLocation(r->grid_shader.program, "u_projection"), 1, GL_FALSE, proj[0]);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mesh_draw(&r->cube_mesh);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderer_draw_text(Renderer* r, float px, float py, float scale,
                        const char* text, vec4 color) {
    /* stb_easy_font vertex: float x,y,z + uchar[4] rgba = 16 bytes */
    static char vbuf[32768];
    int nquads = stb_easy_font_print(0, 0, (char*)text, NULL, vbuf, (int)sizeof(vbuf));
    if (nquads <= 0) return;
    int total_verts = nquads * 4;

    /* Scale and translate vertices in place */
    float* fp = (float*)vbuf;
    for (int i = 0; i < total_verts; i++) {
        fp[i * 4 + 0] = fp[i * 4 + 0] * scale + px;
        fp[i * 4 + 1] = fp[i * 4 + 1] * scale + py;
    }

    /* Build quad → triangle index buffer */
    static unsigned int ibuf[2048 * 6];
    int nc = nquads < 2048 ? nquads : 2048;
    for (int q = 0; q < nc; q++) {
        ibuf[q*6+0] = (unsigned int)(q*4+0);
        ibuf[q*6+1] = (unsigned int)(q*4+1);
        ibuf[q*6+2] = (unsigned int)(q*4+2);
        ibuf[q*6+3] = (unsigned int)(q*4+0);
        ibuf[q*6+4] = (unsigned int)(q*4+2);
        ibuf[q*6+5] = (unsigned int)(q*4+3);
    }

    glBindVertexArray(r->text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, r->text_vbo);
    glBufferData(GL_ARRAY_BUFFER, total_verts * 16, vbuf, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->text_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nc * 6 * (int)sizeof(unsigned int), ibuf, GL_STREAM_DRAW);

    mat4 proj;
    glm_ortho(0.0f, (float)r->fb_width, (float)r->fb_height, 0.0f, -1.0f, 1.0f, proj);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    shader_use(&r->text_shader);
    shader_set_mat4(&r->text_shader, "u_projection", proj);
    shader_set_vec4(&r->text_shader, "u_color", color);
    glDrawElements(GL_TRIANGLES, nc * 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
}

void renderer_draw_text_ndc(Renderer* r, float ndc_x, float ndc_y, float scale,
                             const char* text, vec4 color) {
    float tw = stb_easy_font_width((char*)text) * scale;
    float th = stb_easy_font_height((char*)text) * scale;
    float px = (ndc_x + 1.0f) * 0.5f * (float)r->fb_width  - tw * 0.5f;
    float py = (1.0f - ndc_y) * 0.5f * (float)r->fb_height - th * 0.5f;
    renderer_draw_text(r, px, py, scale, text, color);
}

void renderer_draw_rect(Renderer* r, float cx, float cy, float w, float h, vec4 color) {
    mat4 model, view, proj;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(proj);

    vec3 pos = {cx, cy, 0.0f};
    vec3 scl = {w,  h,  1.0f};
    glm_translate(model, pos);
    glm_scale(model, scl);

    mat3 nm;
    glm_mat4_pick3(model, nm);

    glDisable(GL_DEPTH_TEST);
    shader_use(&r->basic_shader);
    shader_set_mat4(&r->basic_shader, "u_model",         model);
    shader_set_mat4(&r->basic_shader, "u_view",          view);
    shader_set_mat4(&r->basic_shader, "u_projection",    proj);
    shader_set_mat3(&r->basic_shader, "u_normal_matrix", nm);
    shader_set_vec3(&r->basic_shader, "u_light_dir",     (vec3){0.0f, 1.0f, 0.0f});
    shader_set_vec3(&r->basic_shader, "u_light_color",   (vec3){0.0f, 0.0f, 0.0f});
    shader_set_vec3(&r->basic_shader, "u_ambient_color", (vec3){1.0f, 1.0f, 1.0f});
    texture_bind(&r->white_tex, 0);
    shader_set_int (&r->basic_shader, "u_use_texture",   0);
    shader_set_int (&r->basic_shader, "u_texture",       0);
    shader_set_vec4(&r->basic_shader, "u_color",         color);
    mesh_draw(&r->quad_mesh);
    glEnable(GL_DEPTH_TEST);
}

Transform transform_default(void) {
    Transform t;
    glm_vec3_zero(t.position);
    glm_vec3_zero(t.rotation_euler);
    glm_vec3_copy((vec3){1,1,1}, t.scale);
    return t;
}
