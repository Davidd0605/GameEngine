#ifndef VBO_H
#define VBO_H

//#include <GLFW/glfw3.h>
#include <glad/glad.h>

/// <summary>
/// Vertex Buffer Object class
/// Needed in order to store the vertices of the triangle in the GPU memory
/// Class structure is pure syntacitc sugar , 
/// as the VBO is just an unsigned int that stores the ID of the buffer in the GPU memory
/// </summary>
class VBO
{
public:
	GLuint ID;
	VBO(float vertices[], GLenum drawType, int stride, int noAttributes, int attributeSize[]);
	void Bind();
	void Unbind();
	void Delete();

private:
	float* vertices;
	int size;
};
#endif // !VBO_H



