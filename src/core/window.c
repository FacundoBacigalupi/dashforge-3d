#include <glad/glad.h>
#include "window.h"
#include <stdio.h>
#include <stb_image.h>

static void framebuffer_size_callback(GLFWwindow* win, int w, int h) {
    Window* window = glfwGetWindowUserPointer(win);
    window->fb_width  = w;
    window->fb_height = h;
    glViewport(0, 0, w, h);
}

static void window_size_callback(GLFWwindow* win, int w, int h) {
    Window* window = glfwGetWindowUserPointer(win);
    window->width  = w;
    window->height = h;
}

int window_init(Window* w, int width, int height, const char* title) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    w->handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!w->handle) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }

    w->width  = width;
    w->height = height;
    glfwGetFramebufferSize(w->handle, &w->fb_width, &w->fb_height);
    w->dpi_scale = (float)w->fb_width / (float)w->width;

    glfwSetWindowUserPointer(w->handle, w);
    glfwSetFramebufferSizeCallback(w->handle, framebuffer_size_callback);
    glfwSetWindowSizeCallback(w->handle, window_size_callback);

    glfwMakeContextCurrent(w->handle);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to init GLAD\n");
        glfwDestroyWindow(w->handle);
        glfwTerminate();
        return 0;
    }

    glViewport(0, 0, w->fb_width, w->fb_height);
    return 1;
}

void window_shutdown(Window* w) {
    if (w->handle) glfwDestroyWindow(w->handle);
    glfwTerminate();
}

int window_should_close(const Window* w) {
    return glfwWindowShouldClose(w->handle);
}

void window_swap_buffers(Window* w) {
    glfwSwapBuffers(w->handle);
}

void window_poll_events(void) {
    glfwPollEvents();
}

void window_get_size(const Window* w, int* width, int* height) {
    *width  = w->width;
    *height = w->height;
}

void window_set_title(Window* w, const char* title) {
    glfwSetWindowTitle(w->handle, title);
}

void window_set_icon(Window* w, const char* path) {
    int iw, ih, ch;
    unsigned char* px = stbi_load(path, &iw, &ih, &ch, 4);
    if (!px) return;
    GLFWimage img = { iw, ih, px };
    glfwSetWindowIcon(w->handle, 1, &img);
    stbi_image_free(px);
}

GLFWwindow* window_handle(const Window* w) {
    return w->handle;
}
