#pragma once
#include <glad/glad.h>

typedef struct Texture {
    GLuint id;
    int    width;
    int    height;
    int    channels;
} Texture;

int  texture_load(Texture* t, const char* path);
void texture_make_solid(Texture* t, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void texture_bind(const Texture* t, int unit);
void texture_destroy(Texture* t);
