#include "skin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int skin_load(Skin* skin, const char* folder, Mesh* fallback_cube_mesh) {
    memset(skin, 0, sizeof(*skin));
    glm_vec4_copy((vec4){1,1,1,1}, skin->color);
    skin->mesh = fallback_cube_mesh;

    char skin_txt[512];
    snprintf(skin_txt, sizeof(skin_txt), "%s/skin.txt", folder);

    FILE* f = fopen(skin_txt, "r");
    if (!f) {
        fprintf(stderr, "Skin: cannot open '%s'\n", skin_txt);
        strcpy(skin->name, "Default");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        char val[256];
        if (sscanf(line, "name %[^\n]", val) == 1) {
            strncpy(skin->name, val, sizeof(skin->name)-1);
        } else if (sscanf(line, "mesh %s", val) == 1) {
            if (strcmp(val, "generated_cube") != 0) {
                strncpy(skin->mesh_path, val, sizeof(skin->mesh_path)-1);
            }
        } else if (sscanf(line, "texture %s", val) == 1) {
            strncpy(skin->texture_path, val, sizeof(skin->texture_path)-1);
        } else {
            float r, g, b;
            if (sscanf(line, "color %f %f %f", &r, &g, &b) == 3) {
                skin->color[0] = r; skin->color[1] = g; skin->color[2] = b; skin->color[3] = 1.0f;
            }
        }
    }
    fclose(f);

    if (skin->texture_path[0]) {
        char tex_path[512];
        snprintf(tex_path, sizeof(tex_path), "%s/%s", folder, skin->texture_path);
        skin->has_texture = texture_load(&skin->texture, tex_path);
    }

    return 1;
}

void skin_destroy(Skin* skin) {
    if (skin->has_texture) texture_destroy(&skin->texture);
}
