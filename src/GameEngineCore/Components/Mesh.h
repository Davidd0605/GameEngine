#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "../Rendering/ShaderPass.h"
#include "../Rendering/GPUobjects/VAO.h"
#include "../Rendering/GPUobjects/VBO.h"
#include "../Rendering/GPUobjects/EBO.h"
#include "../Rendering/Texture.h"
#include "Component.h"
#include <iostream>

/// <summary>
/// Mesh component.
/// Stores vertex/index data, shader and a list of textures.
/// Textures can be passed as paths at construction or added after via addTexture().
/// RenderSystem handles binding and drawing.
/// </summary>
class Mesh : public Component {
public:
    // With indices
    Mesh(float vertices[], int sizeofVertices, int verticesSize,
        int indices[], int indicesSize,
        ShaderPass* shaderPass,
        int stride, int noAttributes, int attributeSize[],
        std::vector<std::string> texturePaths = {});

    // Without indices
    Mesh(float vertices[], int sizeofVertices, int verticesSize,
        ShaderPass* shaderPass,
        int stride, int noAttributes, int attributeSize[],
        std::vector<std::string> texturePaths = {});

    // Textures
    void addTexture(const std::string& path);
    void addTexture(Texture* texture);
    std::vector<Texture*>& getTextures();
    void bindTextures() const;

    void draw();

    ShaderPass* getShaderPass();
    VAO* getVAO();
    VBO* getVBO();
    EBO* getEBO();

    void start() override;
    void update() override;
    void fixedUpdate() override;
    void end() override;

private:
    EBO* ebo;
    VBO* vbo;
    VAO* vao;

    std::vector<Texture*> textures;

    float* vertices;
    int verticesSize;

    int* indices;
    int indicesSize;

    ShaderPass* shaderPass;
};

#endif