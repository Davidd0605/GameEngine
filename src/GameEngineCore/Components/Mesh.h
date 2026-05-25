#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "../Rendering/Material.h"
#include "../Rendering/GPUobjects/VAO.h"
#include "../Rendering/GPUobjects/VBO.h"
#include "../Rendering/GPUobjects/EBO.h"
#include "Component.h"
#include <iostream>

/// <summary>
/// Mesh component.
/// Stores vertex/index data and a Material.
/// Material owns the shader and textures.
/// RenderSystem handles binding and drawing.
/// </summary>
class Mesh : public Component {
public:
    // with indices
    Mesh(float vertices[], int verticesSize, int verticesBytes,
        int indices[], int indicesSize,
        Material* material,
        int stride, int noAttributes, int attributeSize[]);

    // without indices
    Mesh(float vertices[], int verticesSize, int verticesBytes,
        Material* material,
        int stride, int noAttributes, int attributeSize[]);

    void draw();

    Material* getMaterial();
    VAO* getVAO();
    VBO* getVBO();
    EBO* getEBO();

    float* getVertices() { return vertices; }
    int* getIndices() { return indices; }
    int getVerticesSize() { return verticesSize; }   // vertex count
    int getIndicesSize() { return indicesSize; }

    // Set true for hand-made meshes so full vertex data gets written to JSON.
    // Model-loaded meshes leave this false; their parent's modelPath is enough.
    bool isInlineMesh = false;

    void start() override;
    void update() override;
    void fixedUpdate() override;
    void end() override;

    std::string serialize() override;

private:
    EBO* ebo;
    VBO* vbo;
    VAO* vao;

    float* vertices;
    int verticesSize;   // number of vertices (not floats)

    int* indices;
    int indicesSize;

    Material* material;

    // Stored so serialize() can write the full attribute layout needed to reconstruct the VAO.
    int stride;
    int noAttributes;
    std::vector<int> attributeSizes;
};

#endif
