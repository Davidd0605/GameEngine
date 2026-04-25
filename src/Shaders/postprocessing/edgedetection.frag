#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main() {
    vec2 texel = 1.0 / textureSize(screenTexture, 0);

    // Sobel kernels
    // X direction
    vec3 sobelX =
        -1.0 * texture(screenTexture, texCoords + vec2(-texel.x, -texel.y)).rgb +
        -2.0 * texture(screenTexture, texCoords + vec2(-texel.x,  0.0     )).rgb +
        -1.0 * texture(screenTexture, texCoords + vec2(-texel.x,  texel.y )).rgb +
         1.0 * texture(screenTexture, texCoords + vec2( texel.x, -texel.y )).rgb +
         2.0 * texture(screenTexture, texCoords + vec2( texel.x,  0.0     )).rgb +
         1.0 * texture(screenTexture, texCoords + vec2( texel.x,  texel.y )).rgb;

    // Y direction
    vec3 sobelY =
        -1.0 * texture(screenTexture, texCoords + vec2(-texel.x, -texel.y)).rgb +
        -2.0 * texture(screenTexture, texCoords + vec2( 0.0,     -texel.y)).rgb +
        -1.0 * texture(screenTexture, texCoords + vec2( texel.x, -texel.y)).rgb +
         1.0 * texture(screenTexture, texCoords + vec2(-texel.x,  texel.y)).rgb +
         2.0 * texture(screenTexture, texCoords + vec2( 0.0,      texel.y)).rgb +
         1.0 * texture(screenTexture, texCoords + vec2( texel.x,  texel.y)).rgb;

    vec3 edge = sqrt(sobelX * sobelX + sobelY * sobelY);
    float edgeStrength = length(edge) * 3.0;

    vec3 sceneColor = texture(screenTexture, texCoords).rgb;

    // Blend edges over scene — black outlines
    vec3 result = mix(sceneColor, vec3(0.0), clamp(edgeStrength, 0.0, 1.0));

    FragColor = vec4(result, 1.0);
}