#version 330 core
out vec4 FragColor;
in vec2 texCoords;
uniform sampler2D screenTexture;

void main()
{
    vec2 resolution = vec2(textureSize(screenTexture, 0));
    vec2 texelSize = 1.0 / resolution;

    // extract bright areas
    vec3 color = texture(screenTexture, texCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    vec3 bright = color * smoothstep(0.6, 1.0, brightness);

    // blur bright areas with a simple box blur
    vec3 bloom = vec3(0.0);
    int radius = 6;
    float samples = 0.0;
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec2 sampleCoord = texCoords + offset;
            vec3 s = texture(screenTexture, sampleCoord).rgb;
            float b = dot(s, vec3(0.2126, 0.7152, 0.0722));
            bloom += s * smoothstep(0.6, 1.0, b);
            samples += 1.0;
        }
    }
    bloom /= samples;

    // additive blend bloom on top of original
    vec3 result = color + bloom * 10;

    FragColor = vec4(result, 1.0);
}