#pragma once
#include "renderer/mesh.h"
#include "renderer/texture.h"
#include "renderer/renderer.h"

typedef struct Skin {
    char    name[64];
    char    mesh_path[256];
    char    texture_path[256];
    Mesh*   mesh;        /* pointer into renderer.cube_mesh or custom */
    Texture texture;
    int     has_texture;
    vec4    color;
} Skin;

int  skin_load(Skin* skin, const char* folder, Mesh* fallback_cube_mesh);
void skin_destroy(Skin* skin);
