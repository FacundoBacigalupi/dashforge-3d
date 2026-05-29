#include "particles.h"
#include "renderer/renderer.h"
#include "renderer/shader.h"
#include <glad/glad.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static unsigned int g_rng_state = 0x12345678u;

static float rng_f(void) {
    g_rng_state ^= g_rng_state << 13;
    g_rng_state ^= g_rng_state >> 17;
    g_rng_state ^= g_rng_state << 5;
    return (float)(g_rng_state & 0xFFFFFF) / (float)0xFFFFFF;
}

static float rng_range(float lo, float hi) {
    return lo + rng_f() * (hi - lo);
}

void particles_init(ParticleSystem* ps) {
    memset(ps, 0, sizeof(*ps));
    glGenVertexArrays(1, &ps->vao);
    glGenBuffers(1, &ps->vbo);
    glBindVertexArray(ps->vao);
    glBindBuffer(GL_ARRAY_BUFFER, ps->vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void particles_reset(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) ps->particles[i].active = 0;
    ps->next_slot = 0;
}

static Particle* alloc_particle(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        int idx = (ps->next_slot + i) % MAX_PARTICLES;
        if (!ps->particles[idx].active) {
            ps->next_slot = (idx + 1) % MAX_PARTICLES;
            return &ps->particles[idx];
        }
    }
    /* Pool full: reuse oldest (overwrite at next_slot) */
    Particle* p = &ps->particles[ps->next_slot];
    ps->next_slot = (ps->next_slot + 1) % MAX_PARTICLES;
    return p;
}

void particles_update(ParticleSystem* ps, float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle* p = &ps->particles[i];
        if (!p->active) continue;
        p->lifetime -= dt;
        if (p->lifetime <= 0.0f) { p->active = 0; continue; }
        p->position[0] += p->velocity[0] * dt;
        p->position[1] += p->velocity[1] * dt;
        p->position[2] += p->velocity[2] * dt;
        p->velocity[1]  -= 15.0f * dt;  /* mini-gravity */
        float t = p->lifetime / p->max_lifetime;
        p->color[3] = t;
    }
}

void particles_render(ParticleSystem* ps, Renderer* r) {
    if (!r->camera) return;

    /* Collect active particle positions */
    static float buf[MAX_PARTICLES * 3];
    static vec4  colors[MAX_PARTICLES];
    static float sizes[MAX_PARTICLES];
    int count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle* p = &ps->particles[i];
        if (!p->active) continue;
        buf[count*3+0] = p->position[0];
        buf[count*3+1] = p->position[1];
        buf[count*3+2] = p->position[2];
        glm_vec4_copy(p->color, colors[count]);
        sizes[count] = p->size;
        count++;
    }
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, ps->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * 3 * sizeof(float), buf);

    mat4 view, proj;
    camera_get_view(r->camera, view);
    camera_get_projection(r->camera, r->aspect, proj);

    shader_use(&r->particle_shader);
    glUniformMatrix4fv(glGetUniformLocation(r->particle_shader.program, "u_view"),       1, GL_FALSE, view[0]);
    glUniformMatrix4fv(glGetUniformLocation(r->particle_shader.program, "u_projection"), 1, GL_FALSE, proj[0]);

    glDepthMask(GL_FALSE);
    glBindVertexArray(ps->vao);
    /* Render each particle individually (allows per-particle color/size) */
    for (int i = 0; i < count; i++) {
        glUniform4fv(glGetUniformLocation(r->particle_shader.program, "u_color"), 1, colors[i]);
        glUniform1f(glGetUniformLocation(r->particle_shader.program, "u_size"),  sizes[i]);
        glDrawArrays(GL_POINTS, i, 1);
    }
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

static void emit(ParticleSystem* ps, vec3 pos,
                 float vx_lo, float vx_hi,
                 float vy_lo, float vy_hi,
                 float vz_lo, float vz_hi,
                 float r, float g, float b,
                 float life, float size, int count)
{
    for (int i = 0; i < count; i++) {
        Particle* p = alloc_particle(ps);
        glm_vec3_copy(pos, p->position);
        p->velocity[0]   = rng_range(vx_lo, vx_hi);
        p->velocity[1]   = rng_range(vy_lo, vy_hi);
        p->velocity[2]   = rng_range(vz_lo, vz_hi);
        p->color[0] = r; p->color[1] = g; p->color[2] = b; p->color[3] = 1.0f;
        p->lifetime      = rng_range(life * 0.5f, life);
        p->max_lifetime  = p->lifetime;
        p->size          = size * rng_range(0.7f, 1.3f);
        p->active        = 1;
    }
}

void particles_emit_jump(ParticleSystem* ps, vec3 position) {
    vec3 pos; glm_vec3_copy(position, pos); pos[1] -= 0.4f;
    emit(ps, pos, -3,-1, 1,4, -1,1, 0.8f,0.8f,0.3f, 0.4f, 6.0f, 6);
}

void particles_emit_land(ParticleSystem* ps, vec3 position) {
    vec3 pos; glm_vec3_copy(position, pos); pos[1] -= 0.4f;
    emit(ps, pos, -4,4, 0,3, -1,1, 0.7f,0.7f,0.7f, 0.3f, 5.0f, 8);
}

void particles_emit_death(ParticleSystem* ps, vec3 position) {
    emit(ps, position, -6,6, -2,8, -2,2, 1.0f,0.4f,0.1f, 0.8f, 8.0f, 24);
    emit(ps, position, -4,4, 0,5, -2,2, 1.0f,0.9f,0.2f, 0.5f, 5.0f, 12);
}

void particles_emit_trail(ParticleSystem* ps, vec3 position) {
    vec3 pos; glm_vec3_copy(position, pos);
    pos[0] -= 0.5f;
    emit(ps, pos, -2,0, -0.5f,0.5f, -0.5f,0.5f, 0.3f,0.5f,1.0f, 0.25f, 4.0f, 1);
}
