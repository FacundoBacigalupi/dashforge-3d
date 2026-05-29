#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef enum {
    KEY_SPACE      = GLFW_KEY_SPACE,
    KEY_R          = GLFW_KEY_R,
    KEY_TAB        = GLFW_KEY_TAB,
    KEY_ESCAPE     = GLFW_KEY_ESCAPE,
    KEY_BACKSPACE  = GLFW_KEY_BACKSPACE,
    KEY_F1         = GLFW_KEY_F1,
    KEY_F2         = GLFW_KEY_F2,
    KEY_F3         = GLFW_KEY_F3,
    KEY_1          = GLFW_KEY_1,
    KEY_2          = GLFW_KEY_2,
    KEY_3          = GLFW_KEY_3,
    KEY_4          = GLFW_KEY_4,
    KEY_5          = GLFW_KEY_5,
    KEY_P          = GLFW_KEY_P,
    KEY_Q          = GLFW_KEY_Q,
    KEY_E          = GLFW_KEY_E,
    KEY_W          = GLFW_KEY_W,
    KEY_S          = GLFW_KEY_S,
    KEY_A          = GLFW_KEY_A,
    KEY_D          = GLFW_KEY_D,
    KEY_UP         = GLFW_KEY_UP,
    KEY_DOWN       = GLFW_KEY_DOWN,
    KEY_LEFT       = GLFW_KEY_LEFT,
    KEY_RIGHT      = GLFW_KEY_RIGHT,
    KEY_ENTER      = GLFW_KEY_ENTER,
    KEY_LEFT_CTRL  = GLFW_KEY_LEFT_CONTROL,
    KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    KEY_COUNT      = GLFW_KEY_LAST + 1
} KeyCode;

typedef enum {
    MOUSE_LEFT   = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_RIGHT  = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
    MOUSE_COUNT  = 8
} MouseButton;

typedef struct Input {
    GLFWwindow* window;
    int         keys_current[KEY_COUNT];
    int         keys_previous[KEY_COUNT];
    int         mouse_current[MOUSE_COUNT];
    int         mouse_previous[MOUSE_COUNT];
    double      mouse_x, mouse_y;
    double      mouse_dx, mouse_dy;
    double      scroll_y;
    double      prev_mouse_x, prev_mouse_y;
    /* text typed this frame (UTF-8 codepoints < 128) */
    char        text_input[64];
    int         text_input_len;
} Input;

void input_init(Input* input, GLFWwindow* window);
void input_update(Input* input);

int input_pressed(const Input* input, KeyCode key);
int input_just_pressed(const Input* input, KeyCode key);
int input_just_released(const Input* input, KeyCode key);

int input_mouse_pressed(const Input* input, MouseButton btn);
int input_mouse_just_pressed(const Input* input, MouseButton btn);
int input_mouse_just_released(const Input* input, MouseButton btn);

void   input_get_mouse_pos(const Input* input, double* x, double* y);
void   input_get_mouse_delta(const Input* input, double* dx, double* dy);
double input_get_scroll(const Input* input);

int input_ctrl(const Input* input);
int input_shift(const Input* input);
