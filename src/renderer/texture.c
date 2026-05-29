#include "texture.h"
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int texture_load(Texture* t, const char* path) {
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path, &t->width, &t->height, &t->channels, 0);
    if (!data) {
        fprintf(stderr, "Texture: cannot load '%s': %s\n", path, stbi_failure_reason());
        return 0;
    }

    GLenum fmt = GL_RGB;
    if (t->channels == 4) fmt = GL_RGBA;
    else if (t->channels == 1) fmt = GL_RED;

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, t->width, t->height, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return 1;
}

void texture_make_solid(Texture* t, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    unsigned char data[4] = { r, g, b, a };
    t->width    = 1;
    t->height   = 1;
    t->channels = 4;

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_bind(const Texture* t, int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, t->id);
}

void texture_destroy(Texture* t) {
    if (t->id) { glDeleteTextures(1, &t->id); t->id = 0; }
}
