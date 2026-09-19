#version 450 core
layout (location = 0) in vec3 aPos;
layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform mat4 viewPos;
uniform float spriteScale;

void main() {
    Particle p = particles[gl_InstanceID];

    float c = cos(p.angle);
    float s = sin(p.angle);
    mat2 rot = mat2(c, s, -s, c);

    vec2 worldPos = p.pos + rot * (quadVertex * spriteScale);
    gl_Position = viewPos * vec4(worldPos, 0.0, 1.0);
}
