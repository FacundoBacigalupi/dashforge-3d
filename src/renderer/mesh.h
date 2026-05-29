#pragma once
#include <glad/glad.h>

typedef struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int    vertex_count;
    int    index_count;
} Mesh;

/* layout: pos(3) + normal(3) + uv(2) = 8 floats per vertex */
void mesh_init(Mesh* m, const float* vertices, int vert_count,
               const unsigned int* indices, int idx_count);
void mesh_destroy(Mesh* m);
void mesh_draw(const Mesh* m);
void mesh_draw_lines(const Mesh* m);

void mesh_make_cube(Mesh* m);
void mesh_make_spike(Mesh* m);
void mesh_make_quad(Mesh* m);
