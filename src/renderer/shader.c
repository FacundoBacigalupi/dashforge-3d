#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* read_file(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Shader: cannot open '%s'\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char* buf = malloc(size + 1);
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
}

static GLuint compile_shader(GLenum type, const char* src, const char* path) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "Shader compile error (%s):\n%s\n", path, log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int shader_load(Shader* s, const char* vert_path, const char* frag_path) {
    char* vsrc = read_file(vert_path);
    char* fsrc = read_file(frag_path);
    if (!vsrc || !fsrc) { free(vsrc); free(fsrc); return 0; }

    GLuint vs = compile_shader(GL_VERTEX_SHADER,   vsrc, vert_path);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fsrc, frag_path);
    free(vsrc); free(fsrc);
    if (!vs || !fs) { glDeleteShader(vs); glDeleteShader(fs); return 0; }

    s->program = glCreateProgram();
    glAttachShader(s->program, vs);
    glAttachShader(s->program, fs);
    glLinkProgram(s->program);
    glDetachShader(s->program, vs);
    glDetachShader(s->program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok;
    glGetProgramiv(s->program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(s->program, sizeof(log), NULL, log);
        fprintf(stderr, "Shader link error:\n%s\n", log);
        glDeleteProgram(s->program);
        s->program = 0;
        return 0;
    }
    return 1;
}

void shader_destroy(Shader* s) {
    if (s->program) { glDeleteProgram(s->program); s->program = 0; }
}

void shader_use(const Shader* s) { glUseProgram(s->program); }

void shader_set_int(const Shader* s, const char* n, int v) {
    glUniform1i(glGetUniformLocation(s->program, n), v);
}
void shader_set_float(const Shader* s, const char* n, float v) {
    glUniform1f(glGetUniformLocation(s->program, n), v);
}
void shader_set_vec3(const Shader* s, const char* n, vec3 v) {
    glUniform3fv(glGetUniformLocation(s->program, n), 1, v);
}
void shader_set_vec4(const Shader* s, const char* n, vec4 v) {
    glUniform4fv(glGetUniformLocation(s->program, n), 1, v);
}
void shader_set_mat3(const Shader* s, const char* n, mat3 m) {
    glUniformMatrix3fv(glGetUniformLocation(s->program, n), 1, GL_FALSE, m[0]);
}
void shader_set_mat4(const Shader* s, const char* n, mat4 m) {
    glUniformMatrix4fv(glGetUniformLocation(s->program, n), 1, GL_FALSE, m[0]);
}
