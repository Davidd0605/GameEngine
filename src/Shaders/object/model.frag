#version 330 core
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D _tex0; // diffuse
uniform sampler2D _tex1; // metallic roughness
uniform sampler2D _tex2; // normal map
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
    vec4 texColor = texture(_tex0, texCoord);
    if (texColor.a < 0.01)
        texColor = vec4(0.8, 0.8, 0.8, 1.0);

    // metallic roughness - r=occlusion, g=roughness, b=metallic
    vec4 mrSample = texture(_tex1, texCoord);
    float roughness = mrSample.g;
    float metallic  = mrSample.b;

    // normal map
    vec3 normalSample = texture(_tex2, texCoord).rgb * 2.0 - 1.0;
    // blend between geometry normal and normal map
    vec3 norm = normalize(normal + normalSample * 0.5);

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
        float shininess = mix(2.0, 64.0, 1.0 - roughness);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * pointLights[i].color * pointLights[i].intensity * attenuation * metallic;
        result += diffuse + specular;
    }

    // directional lights
    for (int i = 0; i < dirLightCount; i++) {
        vec3 lightDir = normalize(-dirLights[i].direction);
        vec3 lightOrigin = -lightDir * 1000.0;
        vec3 toFrag = fragPos - lightOrigin;
        float projLen = dot(toFrag, lightDir);
        vec3 closestPoint = lightOrigin + lightDir * projLen;
        float lateralDist = length(fragPos - closestPoint);
        if (lateralDist > dirLights[i].range) continue;
        float attenuation = 1.0 - clamp(lateralDist / dirLights[i].range, 0.0, 1.0);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * dirLights[i].color * dirLights[i].intensity * attenuation;
        vec3 reflectDir = reflect(-lightDir, norm);
        float shininess = mix(2.0, 64.0, 1.0 - roughness);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * dirLights[i].color * dirLights[i].intensity * attenuation * metallic;
        result += diffuse + specular;
    }

    result *= vec3(texColor);

    if (outline == 0) {
        if (texColor.a < 0.5) discard;
        else FragColor = vec4(result, texColor.a);
    } else {
        FragColor = vec4(1.0);
    }
}