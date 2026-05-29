#pragma once
#include "renderer/renderer.h"
#include "renderer/shader.h"

typedef struct Grid {
    GLuint  vao;
    GLuint  vbo;
    int     line_count;
    float   cell_size;
    int     grid_w;
    int     grid_h;
} Grid;

void grid_init(Grid* g, float cell_size, int half_width, int half_height);
void grid_destroy(Grid* g);
void grid_draw(const Grid* g, Renderer* r);
