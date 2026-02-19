#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
/// <summary>
/// Vertex Array Object class
/// Needed in order to store the vertex attribute configuration and the VBO and EBO associated with it.
/// Cleaner structure and better readability.
/// </summary>
class VAO
{
	public:
		GLuint ID;
		VAO();
		void Bind();
		void Unbind();
		void Delete();
};

