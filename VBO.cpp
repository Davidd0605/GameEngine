#include "VBO.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>

VBO::VBO(float vertices[], int sizeofVertices, int verticesSize, GLenum drawType, int stride, int noAttributes, int attributeSize[]) {
    this->vertices = vertices;
    this->size = verticesSize;

    glGenBuffers(1, &ID);

    Bind();

    if (vertices != nullptr && verticesSize > 0) {
        glBufferData(GL_ARRAY_BUFFER, sizeofVertices, vertices, drawType);
    } else {
        std::cerr << "Warning: VBO created with null or empty vertex data\n";
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, drawType);
    }

    int offsetBytes = 0;
    for (int i = 0; i < noAttributes; ++i) {
        GLsizei attribSize = attributeSize[i]; // number of floats for this attribute
        GLsizei strideBytes = sizeof(float) * stride;
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes * sizeof(float) ) );
        glEnableVertexAttribArray(i);
        offsetBytes += attribSize;
    }
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    if (ID != 0) {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}