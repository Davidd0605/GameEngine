#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform float time;

uniform vec3 cameraPos;

struct Light {
	vec3 position;
	vec3 color;
};


void main() {
	FragColor = texture(screenTexture, texCoords);
}