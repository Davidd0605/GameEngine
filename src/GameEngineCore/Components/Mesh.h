#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Rendering/ShaderPass.h"
#include "../Rendering/GPUobjects/VAO.h"
#include "../Rendering/GPUobjects/VBO.h"
#include "../Rendering/GPUobjects/EBO.h"
#include "Component.h"

#include <iostream>


/// <summary>
/// Class for the mesh componenet.
/// Responsible for storing vertex and index data, as well as the shader pass to be used for rendering the mesh.
/// Also responsible for creating the VBO, EBO and VAO objects and configuring them correctly.
/// Draws the mesh using the shader pass and the vertex and index data stored in the GPU memory.
/// The mesh component can be used to render any kind of mesh, as long as the vertex and index 
/// data is provided in the correct format and the shader pass is compatible with the vertex attributes.
/// 
/// May be used as a component of a game object.
/// </summary>
class Mesh : public Component {
public:
    Mesh(float vertices[], int sizeofVertices, int verticesSize,
         int indices[], int indicesSize,
         ShaderPass* shaderPass,
         int stride, int noAttributes, int attributeSize[]);

    Mesh(float vertices[], int sizeofVertices, int verticesSize,
         ShaderPass* shaderPass,
		 int stride, int noAttributes, int attributeSize[]);

	ShaderPass* getShaderPass();
    VAO* getVAO();
    VBO* getVBO();
    EBO* getEBO();

    void drawEBO();
    void draw();
    void start() override;
    void update() override;
    void end() override;

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

