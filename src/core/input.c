#include "input.h"
#include <string.h>

static double g_scroll_y   = 0.0;
/* Chars accumulate here during glfwPollEvents; copied to Input in input_update */
static char   g_text_buf[64] = {0};
static int    g_text_len     = 0;

static void scroll_callback(GLFWwindow* win, double xoff, double yoff) {
    (void)win; (void)xoff;
    g_scroll_y += yoff;
}

static void char_callback(GLFWwindow* win, unsigned int codepoint) {
    (void)win;
    if (codepoint < 128 && g_text_len < 63) {
        g_text_buf[g_text_len++] = (char)codepoint;
        g_text_buf[g_text_len]   = '\0';
    }
}

void input_init(Input* input, GLFWwindow* window) {
    memset(input, 0, sizeof(*input));
    input->window = window;
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, char_callback);
    glfwGetCursorPos(window, &input->mouse_x, &input->mouse_y);
    input->prev_mouse_x = input->mouse_x;
    input->prev_mouse_y = input->mouse_y;
}

void input_update(Input* input) {
    memcpy(input->keys_previous,  input->keys_current,  sizeof(input->keys_current));
    memcpy(input->mouse_previous, input->mouse_current, sizeof(input->mouse_current));

    for (int k = 32; k < KEY_COUNT; k++)
        input->keys_current[k] = glfwGetKey(input->window, k);
    for (int m = 0; m < MOUSE_COUNT; m++)
        input->mouse_current[m] = glfwGetMouseButton(input->window, m);

    input->prev_mouse_x = input->mouse_x;
    input->prev_mouse_y = input->mouse_y;
    glfwGetCursorPos(input->window, &input->mouse_x, &input->mouse_y);
    input->mouse_dx = input->mouse_x - input->prev_mouse_x;
    input->mouse_dy = input->mouse_y - input->prev_mouse_y;

    input->scroll_y = g_scroll_y;
    g_scroll_y = 0.0;

    /* Move accumulated chars (filled by char_callback during poll) into input,
       then reset the global buffer for next frame. */
    input->text_input_len = g_text_len;
    if (g_text_len > 0) {
        memcpy(input->text_input, g_text_buf, (size_t)(g_text_len + 1));
        g_text_len    = 0;
        g_text_buf[0] = '\0';
    } else {
        input->text_input[0] = '\0';
    }
}

int input_pressed(const Input* input, KeyCode key)      { return input->keys_current[key] == GLFW_PRESS; }
int input_just_pressed(const Input* input, KeyCode key) { return input->keys_current[key] == GLFW_PRESS && input->keys_previous[key] == GLFW_RELEASE; }
int input_just_released(const Input* input, KeyCode key){ return input->keys_current[key] == GLFW_RELEASE && input->keys_previous[key] == GLFW_PRESS; }
int input_mouse_pressed(const Input* input, MouseButton btn)      { return input->mouse_current[btn] == GLFW_PRESS; }
int input_mouse_just_pressed(const Input* input, MouseButton btn) { return input->mouse_current[btn] == GLFW_PRESS && input->mouse_previous[btn] == GLFW_RELEASE; }
int input_mouse_just_released(const Input* input, MouseButton btn){ return input->mouse_current[btn] == GLFW_RELEASE && input->mouse_previous[btn] == GLFW_PRESS; }
void   input_get_mouse_pos(const Input* input, double* x, double* y)  { *x=input->mouse_x; *y=input->mouse_y; }
void   input_get_mouse_delta(const Input* input, double* dx, double* dy){ *dx=input->mouse_dx; *dy=input->mouse_dy; }
double input_get_scroll(const Input* input) { return input->scroll_y; }
int input_ctrl(const Input* input)  { return input->keys_current[KEY_LEFT_CTRL]  == GLFW_PRESS; }
int input_shift(const Input* input) { return input->keys_current[KEY_LEFT_SHIFT] == GLFW_PRESS; }
