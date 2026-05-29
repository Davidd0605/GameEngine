//Standard headers
#include <iostream>

//OpenGL headers
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
#include "Components/Light.h"
#include "Rendering/Material.h"
#include "../../SceneSerializer.h"

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

	glViewport(0, 0, globalWidth, globalHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSetWindowSizeCallback(window, windowResize);
	glfwSetScrollCallback(window, Input::scrollCallback);
}

void globalStart(GameScene* scene) { scene->start(); }

void globalUpdate(GameScene* scene) {
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
		std::string title = "Kartof: " + FPS + " FPS";
		glfwSetWindowTitle(window, title.c_str());

		glfwSwapBuffers(window);
		glfwPollEvents();

		
	}
	scene->serialize();
}

void globalEnd(GameScene* scene) {
	scene->end();
}

// --- UTILITY FUNCTIONS ---

gameObject* makeCube(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[],
	glm::vec3 position, glm::vec3 rotation, std::string name)
{
	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());

	ShaderPass* sp = new ShaderPass("src/Shaders/object/basic.frag", "src/Shaders/object/basic.vert");
	Material* mat = new Material(sp);
	mat->setTexture(0, "resources/textures/damian.jpeg");

	Mesh* mesh = new Mesh(vertices, vertexCount, verticesBytes, mat, 8, 3, attributeSizes);
	mesh->isInlineMesh = true;
	go->addComponent(mesh);

	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setRotationX(rotation.x);
	go->getComponent<Transform>()->setRotationY(rotation.y);
	go->getComponent<Transform>()->setRotationZ(rotation.z);

	return go;
}

gameObject* makeLight(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[],
	glm::vec3 position, glm::vec3 color, float intensity, float range, std::string name)
{
	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setScale(glm::vec3(0.3f, 0.3f, 0.3f));
	go->addComponent(new Light(LightType::Point, intensity, color, range));

	ShaderPass* sp = new ShaderPass("src/Shaders/object/light.frag", "src/Shaders/object/light.vert");
	Material* mat = new Material(sp);
	Mesh* mesh = new Mesh(vertices, vertexCount, verticesBytes, mat, 8, 3, attributeSizes);
	mesh->isInlineMesh = true;
	go->addComponent(mesh);

	return go;
}

gameObject* makeDirectionalLight(float* vertices, int vertexCount, int verticesBytes, int attributeSizes[],
	glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, float range, std::string name)
{
	gameObject* go = new gameObject(name);
	go->addComponent(new Transform());
	go->getComponent<Transform>()->setPosition(position);
	go->getComponent<Transform>()->setScale(glm::vec3(0.3f, 0.3f, 0.3f));
	go->addComponent(new Light(LightType::Directional, intensity, color, range, direction));

	ShaderPass* sp = new ShaderPass("src/Shaders/object/light.frag", "src/Shaders/object/light.vert");
	Material* mat = new Material(sp);
	Mesh* mesh = new Mesh(vertices, vertexCount, verticesBytes, mat, 8, 3, attributeSizes);
	mesh->isInlineMesh = true;
	go->addComponent(mesh);

	return go;
}

int main() {
	setupGL(window);

	bool loadFromFile = false; // <-- flip to true to load from saved scene

	GameScene* gameScene = nullptr;

	if (loadFromFile) {
		gameScene = SceneSerializer::deserializeFromFile("scenes/Test game scene 1.json", window);
		gameObject* cam = gameScene->getMainCamera();
		if (cam) cam->addComponent(new CameraController(5.0f));
	}
	else {
		gameScene = new GameScene("Test game scene 67", window);

		// x, y, z,   nx, ny, nz,   u, v
		float cubeVertices[] = {
			// Front
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			// Back
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,
			 // Left
			 -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
			 -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			 -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			 // Right
			  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			  0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
			  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
			  // Top
			  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
			   0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			   0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
			  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
			  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			  // Bottom
			  -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,
			   0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f,
			   0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,
			   0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f,
			  -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,
			  -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f,
		};
		int attributeSizes[] = { 3, 3, 2 }; // pos, normal, uv

		// some random objects
		/*gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Cube_Center"));

		gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(-3.0f, 0.0f, -5.0f), glm::vec3(30.0f, 45.0f, 0.0f), "Cube_Left"));

		gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(3.0f, 0.0f, -5.0f), glm::vec3(-20.0f, 60.0f, 0.0f), "Cube_Right"));

		gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(0.0f, 2.5f, -7.0f), glm::vec3(45.0f, 15.0f, 0.0f), "Cube_Top"));

		gameScene->addObject(makeCube(cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 90.0f, 0.0f), "Cube_Bottom"));*/

		// main camera
		gameObject* cameraGO = new gameObject("MainCamera");
		cameraGO->addComponent(new Transform());
		cameraGO->addComponent(new Camera(45.0f, (float)globalWidth / (float)globalHeight, 0.1f, 100.0f, 0, true));
		cameraGO->getComponent<Transform>()->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
		cameraGO->addComponent(new CameraController(5.0f));
		gameScene->addObject(cameraGO);

		// render system and post-processing passes
		RenderSystem* rs = new RenderSystem();
		gameScene->addSystem(rs);
		rs->postProcessingEnabled = true;
		rs->addPostProcessingShaderPass(new ShaderPass("src/Shaders/postprocessing/edgedetection.frag", "src/Shaders/utility/plainFBO.vert"));
		//rs->addPostProcessingShaderPass(new ShaderPass("src/Shaders/postprocessing/volumetricFog.frag", "src/Shaders/utility/plainFBO.vert"));
		//rs->addPostProcessingShaderPass(new ShaderPass("src/Shaders/postprocessing/painting.frag", "src/Shaders/utility/plainFBO.vert"));
		rs->AddSkyBox({
			"resources/textures/skyboxes/sb1/px.png",  // +X right
			"resources/textures/skyboxes/sb1/nx.png",  // -X left
			"resources/textures/skyboxes/sb1/py.png",  // +Y up
			"resources/textures/skyboxes/sb1/ny.png",  // -Y down
			"resources/textures/skyboxes/sb1/pz.png",  // +Z front
			"resources/textures/skyboxes/sb1/nz.png"   // -Z back
			});

		// lights
		gameObject* dirLight = makeDirectionalLight(
			cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(-1.0f, 5.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f, 5.0f, "Light_1"
		);
		gameScene->addObject(dirLight);

		gameObject* pointLight = makeLight(
			cubeVertices, 36, sizeof(cubeVertices), attributeSizes,
			glm::vec3(0.0f, 3.0f, 3.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f, 5.0f, "Light_2"
		);
		gameScene->addObject(pointLight);

		// models
		ShaderPass* modelShader = new ShaderPass("src/Shaders/object/model.frag", "src/Shaders/object/model.vert");
		Material* modelMaterial = new Material(modelShader);

		gameObject* sponza = ModelLoader::load("resources/models/sponza/Sponza.gltf", modelMaterial);
		gameObject* bunny = ModelLoader::load("resources/models/bunny/Scene.gltf", modelMaterial);
		gameObject* room = ModelLoader::load("resources/models/room/room.gltf", modelMaterial);

		gameScene->addObject(sponza);
		for (gameObject* go : sponza->getComponent<Transform>()->getChildrenGameObjects())
			gameScene->addObject(go);

		gameScene->addObject(room);
		for (gameObject* go : room->getComponent<Transform>()->getChildrenGameObjects())
			gameScene->addObject(go);
	}

	// --- START GAME LOOP ---
	globalStart(gameScene);
	globalUpdate(gameScene);
	globalEnd(gameScene);

	return 0;
}