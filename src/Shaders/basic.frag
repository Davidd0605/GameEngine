#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float time;

void main() {
    vec4 t0 = texture(tex0, texCoord);
    vec4 t1 = texture(tex1, texCoord);
    vec4 t2 = texture(tex2, texCoord);

    float cycle = mod(time, 3.0);

    vec4 col;
    if (cycle < 1.0)
        col = mix(t0, t1, cycle);
    else if (cycle < 2.0)
        col = mix(t1, t2, cycle - 1.0);
    else
        col = mix(t2, t0, cycle - 2.0);

    FragColor = col;
}