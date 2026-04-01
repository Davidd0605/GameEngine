#include "GameScene.h"
#include <iostream>

void GameScene::update()
{
	for (auto go : gameObjects) {
		go->update();
	}

	for (auto sy : systems) {
		sy->update();
	}
}
void GameScene::fixedUpdate() {
	for (auto go : gameObjects) {
		go->fixedUpdate();
	}
	for (auto sy : systems) {
		sy->fixedUpdate();
	}
}
void GameScene::start()
{
	for (auto go : gameObjects) {
		go->start();
	}
	for (auto sy : systems) {
		sy->start();
	}
}

void GameScene::end()
{
	for (auto go : gameObjects) {
		go->end();
	}

	for (auto sy : systems) {
		sy->end();
	}
}

void GameScene::addObject(gameObject* go) {
	this->gameObjects.push_back(go);

	// Auto-register camera
	Camera* cam = go->getComponent<Camera>();
	if (cam != nullptr) {
		cameras.push_back(go);
		sortCameras();
	}

	// Auto-register light
	Light* light = go->getComponent<Light>();
	if (light != nullptr) {
		lights.push_back(go);
	}
}

void GameScene::removeObject(gameObject* go) {
	this->gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
}

std::vector<gameObject*> GameScene::getGameObjects() {
	return this->gameObjects;
}

std::vector<GameSystem*> GameScene::getSystems() {
	return this->systems;
}

void GameScene::addSystem(GameSystem* system) {
	this->systems.push_back(system);
	system->clearCurrentScene();
	system->setCurrentScene(this);
}

void GameScene::removeSystem(GameSystem* system) {
	this->systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
}

void GameScene::sortCameras() {
	std::sort(cameras.begin(), cameras.end(), [](gameObject* a, gameObject* b) {
		return a->getComponent<Camera>()->priority > b->getComponent<Camera>()->priority;
		});
	this->setMainCamera(cameras.front());
}