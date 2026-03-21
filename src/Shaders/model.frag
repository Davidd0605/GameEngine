#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex0;
uniform float time;

// Simple directional light hardcoded for now
vec3 lightDir = normalize(vec3(1.0, 2.0, 1.0));
vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 ambientColor = vec3(0.2, 0.2, 0.2);

void main()
{
    // Sample texture if available, otherwise use a default color
    vec4 texColor = texture(tex0, texCoord);

    // If texture is essentially empty (no texture bound), use a flat color
    if (texColor.a < 0.01)
    {
        texColor = vec4(0.8, 0.8, 0.8, 1.0);
    }

    // Diffuse lighting
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine
    vec3 ambient = ambientColor;
    vec3 result = (ambient + diffuse) * vec3(texColor);

    FragColor = vec4(result, texColor.a);
}
