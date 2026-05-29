#pragma once
#include <glad/glad.h>
#include <cglm/cglm.h>

#define MAX_PARTICLES 2048

typedef struct Particle {
    vec3  position;
    vec3  velocity;
    vec4  color;
    float lifetime;
    float max_lifetime;
    float size;
    int   active;
} Particle;

typedef struct ParticleSystem {
    Particle particles[MAX_PARTICLES];
    int      next_slot;
    GLuint   vao;
    GLuint   vbo;
} ParticleSystem;

/* forward declare Renderer to avoid circular include */
struct Renderer;

void particles_init(ParticleSystem* ps);
void particles_reset(ParticleSystem* ps);
void particles_update(ParticleSystem* ps, float dt);
void particles_render(ParticleSystem* ps, struct Renderer* r);

void particles_emit_jump(ParticleSystem* ps, vec3 position);
void particles_emit_land(ParticleSystem* ps, vec3 position);
void particles_emit_death(ParticleSystem* ps, vec3 position);
void particles_emit_trail(ParticleSystem* ps, vec3 position);
