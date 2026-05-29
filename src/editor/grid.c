#include "grid.h"
#include <stdlib.h>

void grid_init(Grid* g, float cell_size, int half_width, int half_height) {
    g->cell_size = cell_size;
    g->grid_w    = half_width;
    g->grid_h    = half_height;

    int cols = half_width  * 2 + 1;
    int rows = half_height * 2 + 1;
    /* lines: (cols+1) vertical + (rows+1) horizontal, each 2 vertices */
    int total_lines = (cols + 1) + (rows + 1);
    int vert_count  = total_lines * 2;
    float* verts = malloc(vert_count * 3 * sizeof(float));
    int vi = 0;

    /* vertical lines along X */
    for (int x = -half_width; x <= half_width; x++) {
        verts[vi++] = x * cell_size; verts[vi++] = -half_height * cell_size; verts[vi++] = 0;
        verts[vi++] = x * cell_size; verts[vi++] =  half_height * cell_size; verts[vi++] = 0;
    }
    /* horizontal lines along Y */
    for (int y = -half_height; y <= half_height; y++) {
        verts[vi++] = -half_width * cell_size; verts[vi++] = y * cell_size; verts[vi++] = 0;
        verts[vi++] =  half_width * cell_size; verts[vi++] = y * cell_size; verts[vi++] = 0;
    }
    g->line_count = vi / 3 / 2;

    glGenVertexArrays(1, &g->vao);
    glGenBuffers(1, &g->vbo);
    glBindVertexArray(g->vao);
    glBindBuffer(GL_ARRAY_BUFFER, g->vbo);
    glBufferData(GL_ARRAY_BUFFER, vi * sizeof(float), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    free(verts);
}

void grid_destroy(Grid* g) {
    glDeleteBuffers(1, &g->vbo);
    glDeleteVertexArrays(1, &g->vao);
}

void grid_draw(const Grid* g, Renderer* r) {
    if (!r->camera) return;

    mat4 view, proj;
    camera_get_view(r->camera, view);
    camera_get_projection(r->camera, r->aspect, proj);

    shader_use(&r->grid_shader);
    glUniformMatrix4fv(glGetUniformLocation(r->grid_shader.program, "u_view"),       1, GL_FALSE, view[0]);
    glUniformMatrix4fv(glGetUniformLocation(r->grid_shader.program, "u_projection"), 1, GL_FALSE, proj[0]);
    glUniform4f(glGetUniformLocation(r->grid_shader.program, "u_color"), 0.4f, 0.4f, 0.5f, 0.4f);

    glBindVertexArray(g->vao);
    glDrawArrays(GL_LINES, 0, g->line_count * 2);
    glBindVertexArray(0);
}
