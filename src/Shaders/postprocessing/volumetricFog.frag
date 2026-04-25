#version 330 core
out vec4 FragColor;
in vec2 texCoords;
uniform mat4 cameraVP;
uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform vec3 mainCameraPos;
uniform float time;
const int STEPS = 56;
const float DENSITY = 0.1;
const vec3 FOG_COLOR = vec3(0.3, 0.4, 0.4);
#define MAX_LIGHTS 32
struct DirectionalLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
};
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};
uniform int dirLightCount;
uniform int pointLightCount;
uniform DirectionalLight dirLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}
vec3 worldPosFromDepth(float depth) {
    vec4 ndc = vec4(texCoords * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 world = inverse(cameraVP) * ndc;
    return world.xyz / world.w;
}
bool insideCylinder(vec3 samplePos, int i) {
    vec3 lightDir = normalize(dirLights[i].direction);
    vec3 origin = dirLights[i].position;
    vec3 toSample = samplePos - origin;
    float projLen = dot(toSample, lightDir);

    // only the half pointing in light direction
    if (projLen < 0.0) return false;

    vec3 closestPointOnAxis = origin + lightDir * projLen;
    float radialDist = length(samplePos - closestPointOnAxis);
    return radialDist < dirLights[i].range;
}
float pointLightInfluence(vec3 samplePos, int i) {
    float dist = length(samplePos - pointLights[i].position);
    float r = pointLights[i].range;
    if (r <= 0.0 || dist > r) return 0.0;
    float x = dist / r;
    return 1.0 - (x * x);
}
void main() {
    float depth = texture(depthTexture, texCoords).r;
    vec3 worldPos = worldPosFromDepth(depth);
    vec3 rayOrigin = mainCameraPos;
    vec3 rayDir = normalize(worldPos - rayOrigin);
    float rayLength = distance(rayOrigin, worldPos);
    float stepSize = rayLength / float(STEPS);
    float noise = hash(texCoords + fract(time));
    float t = noise * stepSize;
    vec3 accum = vec3(0.0);
    float transmittance = 1.0;
    for (int i = 0; i < STEPS; i++) {
        vec3 samplePos = rayOrigin + rayDir * (t + stepSize * float(i));
        float fogStep = DENSITY * stepSize;
        vec3 lightContrib = vec3(0.0);
        // ---------------- DIRECTIONAL LIGHTS ----------------
        for (int j = 0; j < dirLightCount; j++) {
            if (insideCylinder(samplePos, j)) {
                lightContrib += vec3(1.0, 0.0, 0.0);
            }
        }
        // ---------------- POINT LIGHTS ----------------
        for (int j = 0; j < pointLightCount; j++) {
            float influence = pointLightInfluence(samplePos, j);
            lightContrib += pointLights[j].color *
                            pointLights[j].intensity *
                            influence;
        }
        vec3 sampleColor = FOG_COLOR + lightContrib;
        float absorb = fogStep;
        accum += transmittance * sampleColor * absorb;
        transmittance *= exp(-absorb);
        if (transmittance < 0.01)
            break;
    }
    vec4 scene = texture(screenTexture, texCoords);
    FragColor = vec4(accum + scene.rgb * transmittance, 1.0);
}