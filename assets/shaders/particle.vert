#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_size;

void main() {
    gl_Position  = u_projection * u_view * vec4(a_position, 1.0);
    gl_PointSize = u_size;
}
