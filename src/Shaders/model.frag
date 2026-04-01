#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex0;
uniform float time;
uniform int outline;
// Camera
uniform vec3 viewPos;

// Light system
#define MAX_LIGHTS 32

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

void main()
{
    vec4 texColor = texture(tex0, texCoord);

    if (texColor.a < 0.01)
    {
        texColor = vec4(0.8, 0.8, 0.8, 1.0);
    }

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 ambient = vec3(0.2);
    vec3 result = ambient;

    // Loop through all lights
    for (int i = 0; i < lightCount; i++)
    {
        Light light = lights[i];

        vec3 lightDir = normalize(light.position - fragPos);

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.color * light.intensity;

        // Specular (Phong)
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * light.color * light.intensity;

        result += diffuse + specular;
    }

    result *= vec3(texColor);
    if (outline == 0)
    {
        if (texColor.a < 0.5f) discard;
        else
            FragColor = vec4(result, texColor.a);
    } else {
        FragColor = vec4(1.0);
    }
}