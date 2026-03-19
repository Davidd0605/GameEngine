#include "Scene.h"

Scene::Scene(std::string name) {
	this->name = name;
	mainCamera = nullptr;
}

Scene::Scene(std::string name, gameObject* mainCamera) {
	this->name = name;
	if (mainCamera->getComponent<Camera>() == nullptr) {
		std::cerr << "ERROR :: CAMERA CANDIDATE DOES NOT CONTAIN CAMERA COMPONENT";
	}
	else {
		this->mainCamera = mainCamera;
	}
}

void Scene::setMainCamera(gameObject* mainCamera) {
	if (mainCamera->getComponent<Camera>() == nullptr) {
		std::cerr << "ERROR :: CAMERA CANDIDATE DOES NOT CONTAIN CAMERA COMPONENT";
		return;
	}
	this->mainCamera = mainCamera;
}

gameObject* Scene::getMainCamera() {
	return this->mainCamera;
}