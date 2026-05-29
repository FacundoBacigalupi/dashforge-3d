#pragma once

typedef struct GameTime {
    double last_frame;
    double delta;
    double total;
    int    frame_count;
    double fps_timer;
    float  fps;
} GameTime;

void  time_init(GameTime* t);
void  time_update(GameTime* t);
float time_get_delta_seconds(const GameTime* t);
float time_get_total_seconds(const GameTime* t);
float time_get_fps(const GameTime* t);
