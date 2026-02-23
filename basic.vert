#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
layout (location = 1) in vec3 aCol; // position has attribute position 0

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec4 vertexColor; // specify a color output to the fragment shader


void main()
{
	gl_Position = projection * view * model  * vec4(aPos, 1.0); // we give a vec3 to vec4’s constructor
	vertexColor = vec4(aCol.x * sin(time),
		aCol.y * cos(time),
		aCol.z * tan(time), 
		1.0); // output variable to dark-red
}