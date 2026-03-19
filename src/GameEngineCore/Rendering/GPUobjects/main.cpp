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
#include "../../Scenes/Scene.h"
#include "../../Scenes/GameScene.h"
#include "../../GameObjects/GameObject.h"
#include "../../Systems/RenderSystem.h"

#define elif else if

GLFWwindow* window;
float globalWidth, globalHeight;

void windowResize(GLFWwindow* window, int width, int height) {
	globalWidth = width;
	globalHeight = height;
	glViewport(0, 0, globalWidth, globalHeight);
}

void setupGL(GLFWwindow*& window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	globalWidth = GetSystemMetrics(SM_CXSCREEN);
	globalHeight = GetSystemMetrics(SM_CYSCREEN);
	window = glfwCreateWindow(globalWidth, globalHeight, "David engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, globalWidth, globalHeight);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResize);
}

void globalStart(Scene* scene) { scene->start(); }

void globalUpdate(Scene* scene) {
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void globalEnd(Scene* scene) { scene->end(); }

gameObject* makeCube(float* vertices, int* indices, int attributeSizes[],
	glm::vec3 position, glm::vec3 rotation, std::string name) {

	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->addComponent(
		new Mesh(
			vertices,
			3,
			sizeof(float) * 24,
			indices,
			36,
			new ShaderPass("src/Shaders/basic.frag", "src/Shaders/basic.vert"),
			3,
			1,
			attributeSizes
		)
	);
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setRotationX(rotation.x);
	go->getComponent<Transform>()->setRotationY(rotation.y);
	go->getComponent<Transform>()->setRotationZ(rotation.z);
	return go;
}

int main() {
	setupGL(window);

	GameScene* gameScene = new GameScene("Test game scene 1");

	float vertices[] = {
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	int indices[] = {
		0, 1, 2, 2, 3, 0,
		1, 5, 6, 6, 2, 1,
		5, 4, 7, 7, 6, 5,
		4, 0, 3, 3, 7, 4,
		3, 2, 6, 6, 7, 3,
		4, 5, 1, 1, 0, 4
	};
	int attributeSizes[] = { 3 };

	// --- Cubes ---
	gameScene->addObject(makeCube(vertices, indices, attributeSizes,
		glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Cube_Center"));

	gameScene->addObject(makeCube(vertices, indices, attributeSizes,
		glm::vec3(-3.0f, 0.0f, -5.0f), glm::vec3(30.0f, 45.0f, 0.0f), "Cube_Left"));

	gameScene->addObject(makeCube(vertices, indices, attributeSizes,
		glm::vec3(3.0f, 0.0f, -5.0f), glm::vec3(-20.0f, 60.0f, 0.0f), "Cube_Right"));

	gameScene->addObject(makeCube(vertices, indices, attributeSizes,
		glm::vec3(0.0f, 2.5f, -7.0f), glm::vec3(45.0f, 15.0f, 0.0f), "Cube_Top"));

	gameScene->addObject(makeCube(vertices, indices, attributeSizes,
		glm::vec3(0.0f, -2.5f, -3.0f), glm::vec3(10.0f, 90.0f, 0.0f), "Cube_Bottom"));

	// --- Camera ---
	gameObject* cameraGO = new gameObject("MainCamera");
	cameraGO->addComponent(new Transform());
	cameraGO->addComponent(new Camera(45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f));
	cameraGO->getComponent<Transform>()->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	// --- Scene setup ---
	gameScene->addObject(cameraGO);
	gameScene->setMainCamera(cameraGO);
	gameScene->addSystem(new RenderSystem());

	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}