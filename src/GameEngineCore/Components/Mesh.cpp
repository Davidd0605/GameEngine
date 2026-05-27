#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    int indices[], int indicesSize,
    Material* material,
    int stride, int noAttributes, int attributeSize[]) {

    this->material = material;
    this->vertices = vertices;
    this->indices = indices;
    this->verticesSize = verticesSize;
    this->indicesSize = indicesSize;
    this->stride = stride;
    this->noAttributes = noAttributes;
    for (int i = 0; i < noAttributes; i++)
        this->attributeSizes.push_back(attributeSize[i]);

    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();
    vao->Bind();
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
    ebo = new EBO(indices, indicesSize);
    vao->Unbind();
    if (vbo) vbo->Unbind();
    if (ebo) ebo->Unbind();
}

Mesh::Mesh(float vertices[], int verticesSize, int verticesBytes,
    Material* material,
    int stride, int noAttributes, int attributeSize[]) {

    this->material = material;
    this->vertices = vertices;
    this->verticesSize = verticesSize;
    this->indices = nullptr;
    this->indicesSize = 0;
    this->stride = stride;
    this->noAttributes = noAttributes;
    for (int i = 0; i < noAttributes; i++)
        this->attributeSizes.push_back(attributeSize[i]);

    vbo = nullptr;
    ebo = nullptr;
    vao = new VAO();
    vao->Bind();
    vbo = new VBO(vertices, verticesBytes, verticesSize, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
    vao->Unbind();
    if (vbo) vbo->Unbind();
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

Material* Mesh::getMaterial() { return this->material; }
VAO* Mesh::getVAO() { return this->vao; }
VBO* Mesh::getVBO() { return this->vbo; }
EBO* Mesh::getEBO() { return this->ebo; }

void Mesh::start() {}
void Mesh::update() {}
void Mesh::end() {}
void Mesh::fixedUpdate() {}

std::string Mesh::serialize() {
    nlohmann::json j;
    j["verticesSize"] = verticesSize;   // vertex count
    j["indicesSize"] = indicesSize;
    j["hasIndices"] = (indices != nullptr);
    j["stride"] = stride;
    j["attributeSizes"] = attributeSizes;

    if (isInlineMesh) {
        // verticesSize is the vertex COUNT; total floats = verticesSize * stride
        int totalFloats = verticesSize * stride;
        nlohmann::json verts = nlohmann::json::array();
        for (int i = 0; i < totalFloats; i++)
            verts.push_back(vertices[i]);
        j["vertices"] = verts;

        if (indices) {
            nlohmann::json inds = nlohmann::json::array();
            for (int i = 0; i < indicesSize; i++)
                inds.push_back(indices[i]);
            j["indices"] = inds;
        }

        // Serialize the full material (shader paths, textures, uniforms)
        if (material)
            j["material"] = nlohmann::json::parse(material->serialize());
    }

    return j.dump();
}
