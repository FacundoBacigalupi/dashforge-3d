#pragma once
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct Shader {
    GLuint program;
} Shader;

int  shader_load(Shader* s, const char* vert_path, const char* frag_path);
void shader_destroy(Shader* s);
void shader_use(const Shader* s);

void shader_set_int(const Shader* s, const char* name, int value);
void shader_set_float(const Shader* s, const char* name, float value);
void shader_set_vec3(const Shader* s, const char* name, vec3 v);
void shader_set_vec4(const Shader* s, const char* name, vec4 v);
void shader_set_mat3(const Shader* s, const char* name, mat3 m);
void shader_set_mat4(const Shader* s, const char* name, mat4 m);
