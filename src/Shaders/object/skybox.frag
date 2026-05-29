#version 430

in vec3 texCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main() {
	FragColor = texture(skybox, texCoords);
}