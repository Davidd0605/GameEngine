#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderPass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Component.h"

#include <iostream>

class Mesh : public Component {
public:
    Mesh(float vertices[], int sizeofVertices, int verticesSize,
         int indices[], int indicesSize,
         ShaderPass* shaderPass,
         int stride, int noAttributes, int attributeSize[],
         bool useEbo);

    virtual void Draw(); // allow for polymorphism in case of other mesh types in the future

    // Component lifecycle overrides
    void start() override;
    void update() override;
    void end() override;

    bool useEbo;
private :

    //GPU buffer objects
    EBO* ebo;
    VBO* vbo;
    VAO* vao;

    //Mesh data
    float* vertices;
    int verticesSize;

    int* indices;
    int indicesSize;

    ShaderPass* shaderPass;
};

#endif

