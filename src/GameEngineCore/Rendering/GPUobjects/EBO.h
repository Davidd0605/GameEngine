#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
/// <summary>
/// Element Buffer Object class
/// Needed in order to store the indices of the triangle in the GPU memory,
/// facilitates reusability of vertices and reduces memory usage
/// </summary>
class EBO
{
	public:
		GLuint ID;
		EBO(int indices[], int size);
		void Bind();
		void Unbind();
		void Delete();
	private:
		int* indices;
		int size;
};

