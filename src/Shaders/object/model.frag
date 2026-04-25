#version 330 core
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D tex0;
uniform float time;
uniform int outline;
uniform vec3 viewPos;

#define MAX_LIGHTS 32

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
};

uniform int pointLightCount;
uniform PointLight pointLights[MAX_LIGHTS];

uniform int dirLightCount;
uniform DirectionalLight dirLights[MAX_LIGHTS];

void main() {
    vec4 texColor = texture(tex0, texCoord);
    if (texColor.a < 0.01)
        texColor = vec4(0.8, 0.8, 0.8, 1.0);

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.1); // ambient

    // point lights
    for (int i = 0; i < pointLightCount; i++) {
        float dist = length(pointLights[i].position - fragPos);
        if (dist > pointLights[i].range) continue;

        float attenuation = 1.0 - clamp(dist / pointLights[i].range, 0.0, 1.0);
        vec3 lightDir = normalize(pointLights[i].position - fragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * pointLights[i].color * pointLights[i].intensity * attenuation;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * pointLights[i].color * pointLights[i].intensity * attenuation;

        result += diffuse + specular;
    }

    // directional lights
    for (int i = 0; i < dirLightCount; i++) {
        vec3 lightDir = normalize(-dirLights[i].direction);

        // Project fragPos onto the light direction axis to find closest point on the ray
        // Light ray originates from "above" along its direction
        vec3 lightOrigin = -lightDir * 1000.0; // far away origin along direction
        vec3 toFrag = fragPos - lightOrigin;
        float projLen = dot(toFrag, lightDir);
        vec3 closestPoint = lightOrigin + lightDir * projLen;
        float lateralDist = length(fragPos - closestPoint);

        // Skip fragments outside the cone radius
        if (lateralDist > dirLights[i].range) continue;

        // Soft falloff toward the edge of the cone
        float attenuation = 1.0 - clamp(lateralDist / dirLights[i].range, 0.0, 1.0);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * dirLights[i].color * dirLights[i].intensity * attenuation;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * dirLights[i].color * dirLights[i].intensity * attenuation;

        result += diffuse + specular;
    }

    result *= vec3(texColor);

    if (outline == 0) {
        if (texColor.a < 0.5f) discard;
        else FragColor = vec4(result, texColor.a);
    } else {
        FragColor = vec4(1.0);
    }
}