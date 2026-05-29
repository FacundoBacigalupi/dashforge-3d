#include "mesh.h"
#include <stdlib.h>

void mesh_init(Mesh* m, const float* vertices, int vert_count,
               const unsigned int* indices, int idx_count) {
    m->vertex_count = vert_count;
    m->index_count  = idx_count;

    glGenVertexArrays(1, &m->vao);
    glGenBuffers(1, &m->vbo);
    glGenBuffers(1, &m->ebo);

    glBindVertexArray(m->vao);

    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_count * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    /* position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* normal */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /* uv */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void mesh_destroy(Mesh* m) {
    if (m->ebo) { glDeleteBuffers(1, &m->ebo); m->ebo = 0; }
    if (m->vbo) { glDeleteBuffers(1, &m->vbo); m->vbo = 0; }
    if (m->vao) { glDeleteVertexArrays(1, &m->vao); m->vao = 0; }
}

void mesh_draw(const Mesh* m) {
    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void mesh_draw_lines(const Mesh* m) {
    glBindVertexArray(m->vao);
    glDrawElements(GL_LINES, m->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/* ---- Procedural meshes ---- */

void mesh_make_cube(Mesh* m) {
    /* 24 vertices (4 per face, 6 faces) pos+normal+uv */
    static const float verts[] = {
        /* Front face  (+Z) */
        -0.5f,-0.5f, 0.5f,  0, 0, 1,  0,0,
         0.5f,-0.5f, 0.5f,  0, 0, 1,  1,0,
         0.5f, 0.5f, 0.5f,  0, 0, 1,  1,1,
        -0.5f, 0.5f, 0.5f,  0, 0, 1,  0,1,
        /* Back face   (-Z) */
         0.5f,-0.5f,-0.5f,  0, 0,-1,  0,0,
        -0.5f,-0.5f,-0.5f,  0, 0,-1,  1,0,
        -0.5f, 0.5f,-0.5f,  0, 0,-1,  1,1,
         0.5f, 0.5f,-0.5f,  0, 0,-1,  0,1,
        /* Left face   (-X) */
        -0.5f,-0.5f,-0.5f, -1, 0, 0,  0,0,
        -0.5f,-0.5f, 0.5f, -1, 0, 0,  1,0,
        -0.5f, 0.5f, 0.5f, -1, 0, 0,  1,1,
        -0.5f, 0.5f,-0.5f, -1, 0, 0,  0,1,
        /* Right face  (+X) */
         0.5f,-0.5f, 0.5f,  1, 0, 0,  0,0,
         0.5f,-0.5f,-0.5f,  1, 0, 0,  1,0,
         0.5f, 0.5f,-0.5f,  1, 0, 0,  1,1,
         0.5f, 0.5f, 0.5f,  1, 0, 0,  0,1,
        /* Top face    (+Y) */
        -0.5f, 0.5f, 0.5f,  0, 1, 0,  0,0,
         0.5f, 0.5f, 0.5f,  0, 1, 0,  1,0,
         0.5f, 0.5f,-0.5f,  0, 1, 0,  1,1,
        -0.5f, 0.5f,-0.5f,  0, 1, 0,  0,1,
        /* Bottom face (-Y) */
        -0.5f,-0.5f,-0.5f,  0,-1, 0,  0,0,
         0.5f,-0.5f,-0.5f,  0,-1, 0,  1,0,
         0.5f,-0.5f, 0.5f,  0,-1, 0,  1,1,
        -0.5f,-0.5f, 0.5f,  0,-1, 0,  0,1,
    };
    static const unsigned int idx[] = {
        0,1,2, 2,3,0,       /* front  */
        4,5,6, 6,7,4,       /* back   */
        8,9,10, 10,11,8,    /* left   */
        12,13,14, 14,15,12, /* right  */
        16,17,18, 18,19,16, /* top    */
        20,21,22, 22,23,20  /* bottom */
    };
    mesh_init(m, verts, 24, idx, 36);
}

void mesh_make_spike(Mesh* m) {
    /* Simple pyramid: 5 vertices (4 base + 1 apex) */
    /* Centered: base at y=-0.5, apex at y=+0.5 — matches cube centering */
    static const float verts[] = {
        /* Base */
        -0.5f,-0.5f,-0.5f,  0,-1, 0,  0,0,
         0.5f,-0.5f,-0.5f,  0,-1, 0,  1,0,
         0.5f,-0.5f, 0.5f,  0,-1, 0,  1,1,
        -0.5f,-0.5f, 0.5f,  0,-1, 0,  0,1,
        /* Apex */
         0.0f, 0.5f, 0.0f,  0, 1, 0,  0.5f,0.5f,
        /* Side faces (duplicated for correct normals) */
        /* Front */
        -0.5f,-0.5f, 0.5f,  0, 0.5f, 0.866f,  0,0,
         0.5f,-0.5f, 0.5f,  0, 0.5f, 0.866f,  1,0,
         0.0f, 0.5f, 0.0f,  0, 0.5f, 0.866f,  0.5f,1,
        /* Back */
         0.5f,-0.5f,-0.5f,  0, 0.5f,-0.866f,  0,0,
        -0.5f,-0.5f,-0.5f,  0, 0.5f,-0.866f,  1,0,
         0.0f, 0.5f, 0.0f,  0, 0.5f,-0.866f,  0.5f,1,
        /* Left */
        -0.5f,-0.5f,-0.5f, -0.866f, 0.5f, 0,  0,0,
        -0.5f,-0.5f, 0.5f, -0.866f, 0.5f, 0,  1,0,
         0.0f, 0.5f, 0.0f, -0.866f, 0.5f, 0,  0.5f,1,
        /* Right */
         0.5f,-0.5f, 0.5f,  0.866f, 0.5f, 0,  0,0,
         0.5f,-0.5f,-0.5f,  0.866f, 0.5f, 0,  1,0,
         0.0f, 0.5f, 0.0f,  0.866f, 0.5f, 0,  0.5f,1,
    };
    static const unsigned int idx[] = {
        0,1,2, 2,3,0,   /* base */
        5,6,7,          /* front side */
        8,9,10,         /* back side  */
        11,12,13,       /* left side  */
        14,15,16,       /* right side */
    };
    mesh_init(m, verts, 17, idx, 18);
}

void mesh_make_quad(Mesh* m) {
    static const float verts[] = {
        -0.5f,-0.5f, 0.0f,  0, 0, 1,  0,0,
         0.5f,-0.5f, 0.0f,  0, 0, 1,  1,0,
         0.5f, 0.5f, 0.0f,  0, 0, 1,  1,1,
        -0.5f, 0.5f, 0.0f,  0, 0, 1,  0,1,
    };
    static const unsigned int idx[] = { 0,1,2, 2,3,0 };
    mesh_init(m, verts, 4, idx, 6);
}
