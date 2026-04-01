#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float time;

vec2 curveUV(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(6.0, 6.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void main() {
    vec2 uv = curveUV(texCoords);

    // Kill pixels outside curved screen
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Chromatic aberration
    float aberration = 0.003;
    float r = texture(screenTexture, uv + vec2(aberration, 0.0)).r;
    float g = texture(screenTexture, uv).g;
    float b = texture(screenTexture, uv - vec2(aberration, 0.0)).b;
    vec3 col = vec3(r, g, b);

    // Scanlines
    float scanline = sin(uv.y * 800.0) * 0.04;
    col -= scanline;

    // Flicker
    col *= 0.95 + 0.05 * sin(time * 60.0);

    // Vignette
    vec2 vigUV = uv * (1.0 - uv.yx);
    float vignette = pow(vigUV.x * vigUV.y * 15.0, 0.25);
    col *= vignette;

    // Slight green tint like old CRT phosphor
    col *= vec3(0.9, 1.0, 0.85);

    FragColor = vec4(col, 1.0);
}