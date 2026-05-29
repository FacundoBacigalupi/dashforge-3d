#include "time.h"
#include <GLFW/glfw3.h>

void time_init(GameTime* t) {
    t->last_frame  = glfwGetTime();
    t->delta       = 0.0;
    t->total       = 0.0;
    t->frame_count = 0;
    t->fps_timer   = 0.0;
    t->fps         = 0.0f;
}

void time_update(GameTime* t) {
    double now = glfwGetTime();
    t->delta   = now - t->last_frame;
    if (t->delta > 0.1) t->delta = 0.1; /* clamp large spikes */
    t->last_frame = now;
    t->total += t->delta;
    t->frame_count++;
    t->fps_timer += t->delta;
    if (t->fps_timer >= 1.0) {
        t->fps = (float)(t->frame_count / t->fps_timer);
        t->frame_count = 0;
        t->fps_timer   = 0.0;
    }
}

float time_get_delta_seconds(const GameTime* t) {
    return (float)t->delta;
}

float time_get_total_seconds(const GameTime* t) {
    return (float)t->total;
}

float time_get_fps(const GameTime* t) {
    return t->fps;
}
