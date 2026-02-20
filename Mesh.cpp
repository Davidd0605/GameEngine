#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    int indices[], int indicesSize,
    ShaderPass* shaderPass,
    int stride, int noAttributes, int attributeSize[],
    bool useEbo) {

    this->useEbo = useEbo;

    this->shaderPass = shaderPass;
    this->vertices = vertices;
    this->indices = indices;

    this->verticesSize = verticesSize;
    this->indicesSize = indicesSize;

    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();

    vao->Bind();
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);

    vao->Unbind();
    if (vbo) vbo->Unbind();
    if (ebo) ebo->Unbind();
    
}

void Mesh::Draw() {
    if (shaderPass == nullptr || vao == nullptr) return;

    shaderPass->bind();
    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, verticesSize);
}

void Mesh::start() {}
void Mesh::update() {

    this->Draw();
}
void Mesh::end() {}