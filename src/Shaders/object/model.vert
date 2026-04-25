#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 VP;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = vec3(worldPos);
    normal = mat3(transpose(inverse(model))) * aNormal;
    texCoord = aTexCoord;

    gl_Position = VP * worldPos;
}
