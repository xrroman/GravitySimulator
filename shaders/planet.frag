#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 uColor;
uniform vec3 uLightPos;   // Sun position in world space
uniform vec3 uViewPos;    // Camera position in world space
uniform bool uIsSun;      // true = emissive, skip lighting

void main() {
    vec4 tex = texture(texture1, TexCoord);
    vec3 albedo = tex.rgb * uColor;

    if (uIsSun) {
        // Sun emits its own light, render full bright + subtle glow rim
        FragColor = vec4(albedo * 1.2, 1.0);
        return;
    }

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    vec3 viewDir = normalize(uViewPos  - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    // Ambient, faint so the dark side isn't pure black
    float ambient = 0.1;

    // Diffuse Lambertian
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular Blinn-Phong
    float spec = pow(max(dot(norm, halfDir), 0.0), 32.0) * 0.4;

    vec3 color = albedo * (ambient + diff) + vec3(spec);
    FragColor = vec4(color, 1.0);
}
