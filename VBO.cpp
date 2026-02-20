#include "VBO.h"


/// <summary>
/// 
/// </summary>
/// <param name="vertices"> Vector containing vertex data</param>
/// <param name="drawType"> GLenum for drawing type for buffer</param>
/// <param name="stride"> Stride between vertices in the array</param>
/// <param name="noAttributes"> The number of separate attributes we are sending on layout positions </param>
/// <param name="attributeSize"> The number of floats in each attribute</param>
VBO::VBO(float vertices[], GLenum drawType, int stride, int noAttributes, int attributeSize[]) {
	this->vertices = vertices;
	this->size = size;
	glGenBuffers(1, &ID);
	this->Bind();

	//Send data in the buffer
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices), vertices, drawType);


	//Format data in buffer 

	int offset = 0;
	for (int i = 0; i < noAttributes; i++) {
		//slot, size, type, normalized, stride, pointer to first component of first attribute
		glVertexAttribPointer(i, attributeSize[i], GL_FLOAT, GL_FALSE,
			sizeof(float) * stride,
			(void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += attributeSize[i];	
	}

	this->Unbind();

}

void VBO::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}