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
    shaderPass->setFloat("time", glfwGetTime());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	shaderPass->setMatrix4("model", model);
	shaderPass->setMatrix4("view", view);
	shaderPass->setMatrix4("projection", projection);
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

ShaderPass* Mesh::getShaderPass() {
    return this->shaderPass;
}
 
void Mesh::start() {}
void Mesh::update() {

    this->Draw();
}
void Mesh::end() {}