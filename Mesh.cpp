#include "Mesh.h"


Mesh::Mesh(float vertices[], int verticesSize,
	int indices[], int indicesSize,
	ShaderPass* shaderPass,
	int stride, int noAttributes, int attributeSize[],
	bool useEbo) {

	this->useEbo = useEbo;

	this->vertices = vertices;
	this->indices = indices;

	this->verticesSize = verticesSize;
	this->indicesSize = indicesSize;

	vao = new VAO();
	if (vertices == nullptr) {
		std::cerr << "Error :: Mesh :: vertices array is null" << std::endl;
	}
	else {
		vbo = new VBO(vertices, GL_STATIC_DRAW, stride, noAttributes, attributeSize);
	}

	//TODO add EBO suport
	//if (indices == nullptr) {
	//	std::cerr << "Error :: Mesh :: indices array is null" << std::endl;
	//}
	//else {
	//	//ebo = new EBO(indices, GL_STATIC_DRAW);
	//}

	vao->Bind();
	vbo->Bind();
	//ebo->Bind(); when add EBO support
	vao->Unbind();
}

void Mesh::Draw() {
	vao->Bind();

	//For EBO support
	//glDrawElements(GL_TRIANGLES, verticesSize, GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, verticesSize);
	vao->Unbind();
}