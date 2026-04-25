#version 330 core
out vec4 FragColor;
in vec2 texCoords;
uniform sampler2D screenTexture;

void main()
{
    vec2 resolution = vec2(textureSize(screenTexture, 0));
    float pixelSize = 8.0;

    vec2 pixelated = floor(texCoords * resolution / pixelSize) * pixelSize / resolution;

    FragColor = texture(screenTexture, pixelated);
}