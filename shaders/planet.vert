#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;   // world space position
out vec3 Normal;    // world space normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    // For a sphere centered at origin, the local position IS the normal
    Normal = normalize(mat3(transpose(inverse(model))) * aPos);
    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPos;
}
