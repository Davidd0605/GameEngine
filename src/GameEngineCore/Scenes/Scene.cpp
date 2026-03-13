#include "Scene.h"

Scene::Scene(std::string name) {
	this->name = name;
	mainCamera = nullptr;
}

Scene::Scene(std::string name, Camera* mainCamera) {
	this->name = name;
	this->mainCamera = mainCamera;
}

void Scene::setMainCamera(Camera* mainCamera) {
	this->mainCamera = mainCamera;
}

Camera* Scene::getMainCamera() {
	return this->mainCamera;
}