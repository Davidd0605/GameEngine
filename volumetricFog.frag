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

// -------------------- LIGHT --------------------

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
};

uniform int dirLightCount;
uniform DirectionalLight dirLights[MAX_LIGHTS];

// -------------------- UTILS --------------------

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

vec3 worldPosFromDepth(float depth) {
    vec4 ndc = vec4(texCoords * 2.0 - 1.0,
                    depth * 2.0 - 1.0,
                    1.0);
    vec4 world = inverse(cameraVP) * ndc;
    return world.xyz / world.w;
}

// -------------------- LIGHT SHAPING --------------------

float lightInfluence(vec3 samplePos, int i) {
    vec3 lightDir = normalize(-dirLights[i].direction);

    vec3 lightOrigin = -lightDir * 1000.0;
    vec3 toSample = samplePos - lightOrigin;

    float projLen = dot(toSample, lightDir);
    vec3 closestPoint = lightOrigin + lightDir * projLen;

    float dist = length(samplePos - closestPoint);

    float r = dirLights[i].range;

    if (dist > r) return 0.0;

    // smooth falloff (IMPORTANT FIX)
    float x = dist / r;
    return 1.0 - (x * x);
}

// -------------------- MAIN --------------------

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

        // ---------------- LIGHTING ----------------
        for (int j = 0; j < dirLightCount; j++) {

            float influence = lightInfluence(samplePos, j);

            vec3 L = normalize(-dirLights[j].direction);

            float phase = max(dot(rayDir, L), 0.0);
            phase = pow(phase, 3.0); // stronger beams

            lightContrib += dirLights[j].color *
                            dirLights[j].intensity *
                            influence *
                            phase;
        }

        // ---------------- FINAL FOG COLOR ----------------
        vec3 sampleColor = FOG_COLOR + lightContrib;

        // Beer-Lambert style accumulation (IMPORTANT FIX)
        float absorb = fogStep;
        accum += transmittance * sampleColor * absorb;
        transmittance *= exp(-absorb);

        if (transmittance < 0.01)
            break;
    }

    vec4 scene = texture(screenTexture, texCoords);

    FragColor = vec4(accum + scene.rgb * transmittance, 1.0);
}