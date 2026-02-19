#ifndef  GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include <GLFW/glfw3.h>


class gameObject {

public:
	
	float vertices[100];
	int indices[100];
	std::string name;

	std::vector<gameObject*> children;
	
	//vector of components

	//Transform component
	//Mesh component
	//Collider component


	GLuint VBO, VAO, EBO;

	void start();
	void update();
	void end();
	void render();

	template<typename T> T* getComponent();

	//addComponent()
};
#endif // ! GAME_OBJECT_H


