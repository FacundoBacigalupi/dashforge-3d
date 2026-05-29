#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* handle;
    int width;
    int height;
    int fb_width;
    int fb_height;
    float dpi_scale;
} Window;

int  window_init(Window* w, int width, int height, const char* title);
void window_shutdown(Window* w);
int  window_should_close(const Window* w);
void window_swap_buffers(Window* w);
void window_poll_events(void);
void window_get_size(const Window* w, int* width, int* height);
void window_set_title(Window* w, const char* title);
void window_set_icon(Window* w, const char* path);
GLFWwindow* window_handle(const Window* w);
