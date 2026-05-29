#include "level.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void level_init(Level* level) {
    memset(level, 0, sizeof(*level));
    strcpy(level->name, "Untitled");
    glm_vec3_copy((vec3){0, 1, 0}, level->player_start);
    glm_vec3_copy((vec3){60, 1, 0}, level->level_end);
}

static LevelObjectType parse_type(const char* s) {
    if (!strcmp(s, "block"))      return LEVEL_OBJECT_BLOCK;
    if (!strcmp(s, "spike"))      return LEVEL_OBJECT_SPIKE;
    if (!strcmp(s, "start"))      return LEVEL_OBJECT_START;
    if (!strcmp(s, "end"))        return LEVEL_OBJECT_END;
    if (!strcmp(s, "decoration")) return LEVEL_OBJECT_DECORATION;
    return LEVEL_OBJECT_BLOCK;
}

static const char* type_to_str(LevelObjectType t) {
    switch (t) {
    case LEVEL_OBJECT_BLOCK:      return "block";
    case LEVEL_OBJECT_SPIKE:      return "spike";
    case LEVEL_OBJECT_START:      return "start";
    case LEVEL_OBJECT_END:        return "end";
    case LEVEL_OBJECT_DECORATION: return "decoration";
    }
    return "block";
}

int level_load(Level* level, const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Level: cannot open '%s'\n", path);
        return 0;
    }

    level_init(level);
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        char key[64], val[256];
        if (sscanf(line, "level_name %[^\n]", val) == 1) {
            strncpy(level->name, val, sizeof(level->name)-1);
        } else if (sscanf(line, "start %f %f %f",
                           &level->player_start[0],
                           &level->player_start[1],
                           &level->player_start[2]) == 3) {
        } else if (sscanf(line, "end %f %f %f",
                           &level->level_end[0],
                           &level->level_end[1],
                           &level->level_end[2]) == 3) {
        } else if (sscanf(line, "object %s", key) == 1) {
            LevelObject obj = {0};
            obj.type = parse_type(key);
            float x=0,y=0,z=0, sx=1,sy=1,sz=1, roty=0;
            sscanf(line, "object %*s %f %f %f %f %f %f %f",
                   &x, &y, &z, &sx, &sy, &sz, &roty);
            glm_vec3_copy((vec3){x, y, z}, obj.transform.position);
            glm_vec3_copy((vec3){0, roty, 0}, obj.transform.rotation_euler);
            glm_vec3_copy((vec3){sx, sy, sz}, obj.transform.scale);

            obj.solid     = (obj.type == LEVEL_OBJECT_BLOCK);
            obj.lethal    = (obj.type == LEVEL_OBJECT_SPIKE);
            obj.is_end    = (obj.type == LEVEL_OBJECT_END);
            obj.color_idx = 0;
            /* optional color_idx as 9th token */
            sscanf(line, "object %*s %*f %*f %*f %*f %*f %*f %*f %d", &obj.color_idx);

            level_object_rebuild_collider(&obj);
            level_add_object(level, obj);
        }
    }
    fclose(f);
    return 1;
}

int level_save(const Level* level, const char* path) {
    FILE* f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "Level: cannot write '%s'\n", path);
        return 0;
    }
    fprintf(f, "# DashForge 3D level format v1\n");
    fprintf(f, "level_name %s\n", level->name);
    fprintf(f, "start %.4f %.4f %.4f\n",
            level->player_start[0], level->player_start[1], level->player_start[2]);
    fprintf(f, "end %.4f %.4f %.4f\n",
            level->level_end[0], level->level_end[1], level->level_end[2]);
    fprintf(f, "\n# object <type> <x> <y> <z> <sx> <sy> <sz> <rotY>\n");
    for (int i = 0; i < level->object_count; i++) {
        const LevelObject* obj = &level->objects[i];
        if (obj->type == LEVEL_OBJECT_DECORATION)
            fprintf(f, "object %s %.4f %.4f %.4f %.4f %.4f %.4f %.4f %d\n",
                    type_to_str(obj->type),
                    obj->transform.position[0], obj->transform.position[1], obj->transform.position[2],
                    obj->transform.scale[0], obj->transform.scale[1], obj->transform.scale[2],
                    obj->transform.rotation_euler[1], obj->color_idx);
        else
            fprintf(f, "object %s %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
                    type_to_str(obj->type),
                    obj->transform.position[0], obj->transform.position[1], obj->transform.position[2],
                    obj->transform.scale[0], obj->transform.scale[1], obj->transform.scale[2],
                    obj->transform.rotation_euler[1]);
    }
    fclose(f);
    return 1;
}

int level_add_object(Level* level, LevelObject object) {
    if (level->object_count >= MAX_LEVEL_OBJECTS) return 0;
    level->objects[level->object_count++] = object;
    return 1;
}

void level_remove_object_at(Level* level, int index) {
    if (index < 0 || index >= level->object_count) return;
    for (int i = index; i < level->object_count - 1; i++) {
        level->objects[i] = level->objects[i + 1];
    }
    level->object_count--;
}

void level_rebuild_colliders(Level* level) {
    for (int i = 0; i < level->object_count; i++) {
        level_object_rebuild_collider(&level->objects[i]);
    }
}

float level_get_length(const Level* level) {
    return level->level_end[0];
}
