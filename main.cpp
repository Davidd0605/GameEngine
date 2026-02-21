//Standard headers

//OpenGL headers
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

//Custom headers
#include "Scene.h"
#include "GameScene.h"
#include "GameObject.h"

//Define macros here
#define elif  else if
GLFWwindow* window;
float globalWidth, globalHeight;



// Some utils
void windowResize(GLFWwindow * window, int width, int height) {
	globalWidth = width;
	globalHeight = height;
	glViewport(0, 0, globalWidth, globalHeight);
}
void setupGL(GLFWwindow* & window) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	globalHeight = globalWidth = 600;
	window = glfwCreateWindow(globalWidth, globalHeight, "David engine", NULL, NULL);


	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, globalWidth, globalHeight);
	glfwSetWindowSizeCallback(window, windowResize);
}


void globalStart(Scene* scene) {
	scene->start();
}

void globalUpdate(Scene* scene) {
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		scene->update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void globalEnd(Scene* scene) {
	scene->end();
}
int main() {

	setupGL(window);

	//Create the game scene
	GameScene* gameScene = new GameScene("Test game scene 1");
	gameObject* go = new gameObject("Testicle");

	//Bootleg object with mesh componene
	float vertices[] = {
		 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	int indices[] = { 0, 1, 2 };
	int attributeSizes[] = { 3, 3 };

	go->addComponent(
		new Mesh(
			vertices,
			3,	//size in bytes of vertex data
			sizeof(vertices),					
			new ShaderPass("basic.frag", "basic.vert"),
			6,					
			2,					
			attributeSizes			
		)
	);
	gameScene->addObject(go);

	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}
