//Standard headers

//OpenGL headers
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <shellscalingapi.h>
#include <stb/stb_image.h>
#pragma comment(lib, "Shcore.lib")
//GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

//Custom headers
#include "Scenes/Scene.h"
#include "Scenes/GameScene.h"
#include "GameObjects/GameObject.h"
#include "Systems/RenderSystem.h"
#include "Utilities/Time.h"
#include "Functionalities/CameraController.h"
#include "Rendering/ModelLoader.h"
#include "Functionalities/lightSpin.h"

#define elif else if

GLFWwindow* window;
float globalWidth, globalHeight;

const float FIXED_TIMESTEP = 1.0f / 60.0f;
float fixedUpdateAccumulator = 0.0f;

void windowResize(GLFWwindow* window, int width, int height) {
	globalWidth = width;
	globalHeight = height;
	glViewport(0, 0, globalWidth, globalHeight);
}

void setupGL(GLFWwindow*& window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	globalWidth = GetSystemMetrics(SM_CXSCREEN);
	globalHeight = GetSystemMetrics(SM_CYSCREEN);

	window = glfwCreateWindow(globalWidth, globalHeight, "Kartof engine", NULL, NULL);
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

	//const char* version = (const char*)glGetString(GL_VERSION);
	//printf("OpenGL version: %s\n", version);

	glViewport(0, 0, globalWidth, globalHeight);

	//Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glfwSetWindowSizeCallback(window, windowResize);
	glfwSetScrollCallback(window, Input::scrollCallback);
}

void globalStart(Scene* scene) { scene->start(); }

void globalUpdate(Scene* scene) {
	while (!glfwWindowShouldClose(window)) {

		Time::update();
		Input::update(window);
		scene->update();
		fixedUpdateAccumulator += Time::deltaTime;
		while (fixedUpdateAccumulator >= FIXED_TIMESTEP) {
			scene->fixedUpdate();
			fixedUpdateAccumulator -= FIXED_TIMESTEP;
		}

		std::string FPS = std::to_string(Time::fps);
		std::string title = "Kartof: " + FPS + " FPS \n";
		glfwSetWindowTitle(window, title.c_str());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void globalEnd(Scene* scene) { scene->end(); }

//UTILITY FUNCTIONS
gameObject* makeCube(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[], glm::vec3 position, glm::vec3 rotation, std::string name) {

	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->addComponent(
		new Mesh(
			vertices,
			vertexCount,
			verticesBytes,
			new ShaderPass("src/Shaders/object/basic.frag", "src/Shaders/object/basic.vert"),
			8,
			3,
			attributeSizes,
			{ "resources/textures/gaypeace.jpeg", "resources/textures/isreal.jpeg", "resources/textures/damian.jpeg" }
		)
	);
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setRotationX(rotation.x);
	go->getComponent<Transform>()->setRotationY(rotation.y);
	go->getComponent<Transform>()->setRotationZ(rotation.z);

	return go;
}
gameObject* makeLight(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[], glm::vec3 position, glm::vec3 color, float intensity, float range, std::string name)
{
	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setScale(glm::vec3(.3, .3, .3));
	go->addComponent(new Light(LightType::Point, intensity, color, range));

	ShaderPass* lightShader = new ShaderPass("src/Shaders/object/light.frag", "src/Shaders/object/light.vert");
	go->addComponent(new Mesh(vertices, vertexCount, verticesBytes, lightShader, 8, 3, attributeSizes, {}));
	return go;
}
gameObject* makeDirectionalLight(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[], glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, float range, std::string name)
{
	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setScale(glm::vec3(.3, .3, .3));
	go->addComponent(new Light(LightType::Directional, intensity, color, range, direction));

	ShaderPass* lightShader = new ShaderPass("src/Shaders/object/light.frag", "src/Shaders/object/light.vert");
	go->addComponent(new Mesh(vertices, vertexCount, verticesBytes, lightShader, 8, 3, attributeSizes, {}));
	return go;
}



int main() {

	/// --- START OF SCENE SETUP ---
	setupGL(window);
	GameScene* gameScene = new GameScene("Test game scene 1");

	// x, y, z,   r, g, b,   u, v
	float cubeVertices[] = {
		// Front
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		// Back
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 // Left
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		 // Right
		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		  // Top
		  -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		  -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		  // Bottom
		  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		   0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		   0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		  -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
	};
	int attributeSizes[] = { 3, 3, 2 }; // pos, col, uv

	// --- Cubes ---
	gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Cube_Center"));

	gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(-3.0f, 0.0f, -5.0f), glm::vec3(30.0f, 45.0f, 0.0f), "Cube_Left"));

	gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(3.0f, 0.0f, -5.0f), glm::vec3(-20.0f, 60.0f, 0.0f), "Cube_Right"));

	gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(0.0f, 2.5f, -7.0f), glm::vec3(45.0f, 15.0f, 0.0f), "Cube_Top"));

	gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(0.0f, -2.5f, -3.0f), glm::vec3(10.0f, 90.0f, 0.0f), "Cube_Bottom"));

	// --- Camera ---
	gameObject* cameraGO = new gameObject("MainCamera");
	cameraGO->addComponent(new Transform());
	cameraGO->addComponent(new Camera(45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f, 0, true));
	cameraGO->getComponent<Transform>()->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	cameraGO->addComponent(new CameraController(5.0f));
	gameScene->addObject(cameraGO);

	gameScene->addSystem(new RenderSystem());
	gameScene->getSystem<RenderSystem>()->postProcessingEnabled = true;
	gameScene->getSystem<RenderSystem>()->addPostProcessingShaderPass(new ShaderPass("src/Shaders/postprocessing/edgedetection.frag", "src/Shaders/utility/plainFBO.vert"));
	gameScene->getSystem<RenderSystem>()->addPostProcessingShaderPass(new ShaderPass("src/Shaders/postprocessing/volumetricFog.frag", "src/Shaders/utility/plainFBO.vert"));

	gameObject* light  = makeDirectionalLight(
		cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(0.0f, 15.0f, 0.0f),	// position (visual only)
		glm::vec3(0, -1.0f, 0.0f),		// direction (pointing down)
		glm::vec3(1, 0, 0),				// color
		1.0f,                           // intensity
		1.0f,							// range
		"Light_1"
	);

	gameScene->addObject(light);
	//light->addComponent(new lightSpin(light));

	gameObject* pointLight = makeLight(
		cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
		glm::vec3(0.0f, 3.0f, 0.0f),	// position (visual only)
		glm::vec3(1, 0, 1),				// color
		1.0f,                           // intensity
		5.0f,							// range
		"Light_2"
	);

	gameScene->addObject(pointLight);

	// --- Models ---
	ShaderPass* modelShader = new ShaderPass("src/Shaders/object/model.frag", "src/Shaders/object/model.vert");

	std::vector<gameObject*> sponza = ModelLoader::load("resources/models/sponza/Sponza.gltf", modelShader);
	std::vector<gameObject*> bunny = ModelLoader::load("resources/models/bunny/Scene.gltf", modelShader);

	for (gameObject* go : sponza) {
		gameScene->addObject(go);
	}

	for (gameObject* go : bunny) {
		go->getComponent<Transform>()->setRotationX(90);
		gameScene->addObject(go);
	}

	// --- END OF SCENE SETUP ---


	// --- START GAME LOOP ---
	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}