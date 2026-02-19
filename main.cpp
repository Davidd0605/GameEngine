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


//Define macros here
#define elif  else if

float globalWidth, globalHeight;

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
	GLFWwindow* window;
	setupGL(window);

	while (!glfwWindowShouldClose(window)) {

		scene->update();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void globalEnd(Scene* scene) {
	scene->end();
}
int main() {

	//Create the game scene
	GameScene* gameScene = new GameScene();

	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}