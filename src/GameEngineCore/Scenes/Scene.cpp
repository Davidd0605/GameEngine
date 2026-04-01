#include "Scene.h"

Scene::Scene(std::string name) {
	this->name = name;
	mainCamera = nullptr;
}

Scene::Scene(std::string name, gameObject* mainCamera) {
	this->name = name;
	if (mainCamera->getComponent<Camera>() == nullptr) {
		std::cerr << "[Scene] ERROR :: CAMERA CANDIDATE DOES NOT CONTAIN CAMERA COMPONENT";
	}
	else {
		this->mainCamera = mainCamera;
	}
}

void Scene::setMainCamera(gameObject* mainCamera) {
	if (mainCamera->getComponent<Camera>() == nullptr) {
		std::cerr << "[Scene] ERROR :: CAMERA CANDIDATE DOES NOT CONTAIN CAMERA COMPONENT";
		return;
	}
	this->mainCamera = mainCamera;
}

std::vector<gameObject*> Scene::getCameras() {
	return this->cameras;
}
gameObject* Scene::getMainCamera() {
	return this->mainCamera;
}

std::vector<gameObject*> Scene::getLights() {
	return this->lights;
}