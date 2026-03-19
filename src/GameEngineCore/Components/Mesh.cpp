#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    int indices[], int indicesSize,
    ShaderPass* shaderPass,
    int stride, int noAttributes, int attributeSize[],
    std::vector<std::string> texturePaths) {

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
    ebo = new EBO(indices, indicesSize);
    vao->Unbind();
    if (vbo) vbo->Unbind();
    if (ebo) ebo->Unbind();

    for (const auto& path : texturePaths) {
        addTexture(path);
    }
}

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    ShaderPass* shaderPass,
    int stride, int noAttributes, int attributeSize[],
    std::vector<std::string> texturePaths) {

    this->shaderPass = shaderPass;
    this->vertices = vertices;
    this->verticesSize = verticesSize;
    this->indices = nullptr;
    this->indicesSize = 0;

    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();
    vao->Bind();
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
    vao->Unbind();
    if (vbo) vbo->Unbind();

    for (const auto& path : texturePaths) {
        addTexture(path);
    }
}

void Mesh::addTexture(const std::string& path) {
    if (textures.size() >= 16) {
        std::cerr << "ERROR :: TEXTURE CAPACITY EXCEEDED\n";
        return;
    }
    textures.push_back(new Texture(path.c_str()));
}

void Mesh::addTexture(Texture* texture) {
    if (textures.size() >= 16) {
        std::cerr << "ERROR :: TEXTURE CAPACITY EXCEEDED\n";
        return;
    }
    textures.push_back(texture);
}

std::vector<Texture*>& Mesh::getTextures() {
    return textures;
}

void Mesh::bindTextures() const {
    for (int i = 0; i < (int)textures.size(); i++) {
        if (textures[i]) textures[i]->bind(i);
    }
}

void Mesh::draw() {
    if (ebo) {
        ebo->Bind();
        glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, verticesSize);
    }
}

ShaderPass* Mesh::getShaderPass() { return this->shaderPass; }
VAO* Mesh::getVAO() { return this->vao; }
VBO* Mesh::getVBO() { return this->vbo; }
EBO* Mesh::getEBO() { return this->ebo; }

void Mesh::start() {}
void Mesh::update() {}
void Mesh::end() {}