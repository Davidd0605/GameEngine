//Standard headers

//OpenGL headers
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")
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

	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	globalWidth = GetSystemMetrics(SM_CXSCREEN);
	globalHeight = GetSystemMetrics(SM_CYSCREEN);

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

	//Adapt to screen DPI


	setupGL(window);

	
	//Temporary create objects here, will be moved to a scene loader later
	GameScene* gameScene = new GameScene("Test game scene 1");
	gameObject* go = new gameObject("Testicle");

	float vertices[] = {
		// Front face
		-0.5f, -0.5f,  0.5f,  // 0
		 0.5f, -0.5f,  0.5f,  // 1
		 0.5f,  0.5f,  0.5f,  // 2
		-0.5f,  0.5f,  0.5f,  // 3

		// Back face
		-0.5f, -0.5f, -0.5f,  // 4
		 0.5f, -0.5f, -0.5f,  // 5
		 0.5f,  0.5f, -0.5f,  // 6
		-0.5f,  0.5f, -0.5f   // 7
	};
	int indices[] = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Right
		1, 5, 6,
		6, 2, 1,

		// Back
		5, 4, 7,
		7, 6, 5,

		// Left
		4, 0, 3,
		3, 7, 4,

		// Top
		3, 2, 6,
		6, 7, 3,

		// Bottom
		4, 5, 1,
		1, 0, 4
	};
	int attributeSizes[] = { 3};
	

	//Mesh with EBO test
	go->addComponent(
		new Mesh(
			vertices,
			3,
			sizeof(vertices),
			indices,
			36,
			new ShaderPass("basic.frag", "basic.vert"),
			3,
			1,
			attributeSizes
		)
	);
	gameScene->addObject(go);

	//Start the game loop
	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}
