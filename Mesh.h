#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderPass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <iostream>
class Mesh {
	public:
		Mesh(float vertices[], int verticesSize,
			int indices[], int indicesSize,
			ShaderPass* shaderPass,
			int stride, int noAttributes, int attributeSize[],
			bool useEbo);

		void virtual Draw(); //allow for polymorphism in case of animated meshes or other mesh types in the future

		bool useEbo;
	private :

		//GPU buffer objects
		//EBO* ebo;
		VBO* vbo;
		VAO* vao;

		//Mesh data
		float* vertices;
		int verticesSize;

		int* indices;
		int indicesSize;

};

#endif

