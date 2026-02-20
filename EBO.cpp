#include "EBO.h"

EBO::EBO(int indices[], int size) {
	glGenBuffers(1, &ID);
	this->indices = indices;
	this->size = size;
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}
