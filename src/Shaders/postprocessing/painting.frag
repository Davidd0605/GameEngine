#version 330 core
out vec4 FragColor;
in vec2 texCoords;
uniform sampler2D screenTexture;

// Kuwahara filter - oil painting effect
// Samples 4 quadrant regions around each pixel,
// picks the one with lowest variance (most uniform color)

const int RADIUS = 4;

vec3 regionMeanAndVariance(vec2 uv, vec2 texel, int x0, int y0, int x1, int y1, out float variance) {
    vec3 sum = vec3(0.0);
    vec3 sumSq = vec3(0.0);
    float count = 0.0;

    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            vec3 s = texture(screenTexture, uv + vec2(float(x), float(y)) * texel).rgb;
            sum += s;
            sumSq += s * s;
            count += 1.0;
        }
    }

    vec3 mean = sum / count;
    vec3 var = (sumSq / count) - (mean * mean);
    variance = dot(var, vec3(0.299, 0.587, 0.114)); // luminance weighted variance
    return mean;
}

void main() {
    vec2 texel = 1.0 / vec2(textureSize(screenTexture, 0));

    float v0, v1, v2, v3;
    vec3 m0 = regionMeanAndVariance(texCoords, texel, -RADIUS,  0,        0, RADIUS,  v0); // top-left
    vec3 m1 = regionMeanAndVariance(texCoords, texel,  0,        0,  RADIUS, RADIUS,  v1); // top-right
    vec3 m2 = regionMeanAndVariance(texCoords, texel, -RADIUS, -RADIUS,  0,  0,       v2); // bottom-left
    vec3 m3 = regionMeanAndVariance(texCoords, texel,  0,       -RADIUS, RADIUS, 0,   v3); // bottom-right

    // pick region with lowest variance
    vec3 result = m0;
    float minVar = v0;
    if (v1 < minVar) { result = m1; minVar = v1; }
    if (v2 < minVar) { result = m2; minVar = v2; }
    if (v3 < minVar) { result = m3; }

    // slightly boost saturation to make it feel more painted
    float lum = dot(result, vec3(0.299, 0.587, 0.114));
    result = mix(vec3(lum), result, 1.3);

    FragColor = vec4(result, 1.0);
}
