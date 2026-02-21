#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    int indices[], int indicesSize,
    ShaderPass* shaderPass,
    int stride, int noAttributes, int attributeSize[]) {

    this->shaderPass = shaderPass;
    this->vertices = vertices;
    this->indices = indices;

    this->verticesSize = verticesSize;
    this->indicesSize = indicesSize;

    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();

    vao->Bind();

    //Careful constructor BINDS and never unbinds the buffers
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
	ebo = new EBO(indices, indicesSize);

    vao->Unbind();
    if (vbo) vbo->Unbind();
    if (ebo) ebo->Unbind();
    
}

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    ShaderPass* shaderPass,
    int stride, int noAttributes, int attributeSize[]) {

	
    this->shaderPass = shaderPass;
    this->vertices = vertices;
    this->verticesSize = verticesSize;

    this->indices = nullptr;
	this->indicesSize = 0;  

    //Initialize buffers
    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();


    vao->Bind();
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
    vao->Unbind();
    if (vbo) vbo->Unbind();

}

void Mesh::Draw() {
    if (shaderPass == nullptr || vao == nullptr) return;

    shaderPass->bind();
    vao->Bind();

    if(ebo) {
        glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, verticesSize);
    }
    vao->Unbind();
    shaderPass->unbind();
}

void Mesh::start() {}
void Mesh::update() {

    this->Draw();
}
void Mesh::end() {}