#version 330 core

uniform vec4 u_color;
out vec4 frag_color;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    float d    = dot(coord, coord);
    if (d > 0.25) discard;
    float alpha = 1.0 - (d * 4.0);
    frag_color  = vec4(u_color.rgb, u_color.a * alpha);
}
