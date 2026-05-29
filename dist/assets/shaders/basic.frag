#version 330 core

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_texcoord;

uniform sampler2D u_texture;
uniform vec4      u_color;
uniform int       u_use_texture;
uniform vec3      u_light_dir;
uniform vec3      u_light_color;
uniform vec3      u_ambient_color;

out vec4 frag_color;

void main() {
    vec4 base_color = u_color;
    if (u_use_texture != 0) {
        base_color *= texture(u_texture, v_texcoord);
    }

    vec3 n   = normalize(v_normal);
    vec3 ld  = normalize(-u_light_dir);
    float diff = max(dot(n, ld), 0.0);

    vec3 ambient  = u_ambient_color * base_color.rgb;
    vec3 diffuse  = u_light_color * diff * base_color.rgb;
    vec3 result   = ambient + diffuse;

    frag_color = vec4(result, base_color.a);
}
