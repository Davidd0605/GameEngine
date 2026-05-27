#include "GameScene.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include <json/json.h>

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
	Camera* cam = go->getComponent<Camera>();
	if (cam != nullptr) {
		cameras.push_back(go);
		sortCameras();
	}
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

gameObject* GameScene::findObjectByName(const std::string& name) {
	for (auto go : gameObjects) {
		if (go->name == name) return go;
	}
	return nullptr;
}

std::string GameScene::serialize() {
	nlohmann::json j;
	j["name"] = name;

	nlohmann::json goArray = nlohmann::json::array();
	for (auto go : gameObjects) {
		if (go->isModelChild) continue;
		try {
			goArray.push_back(nlohmann::json::parse(go->serialize()));
		}
		catch (...) {
			goArray.push_back(go->serialize());
		}
	}
	j["gameObjects"] = goArray;

	nlohmann::json sysArray = nlohmann::json::array();
	for (auto sy : systems) {
		std::string sysJson = sy->serialize();
		if (!sysJson.empty()) {
			try {
				sysArray.push_back(nlohmann::json::parse(sysJson));
			}
			catch (...) {
				sysArray.push_back(sysJson);
			}
		}
	}
	j["systems"] = sysArray;

	std::string result = j.dump(2);

	_mkdir("scenes");
	std::string filePath = "scenes/" + name + ".json";
	std::ofstream out(filePath);
	if (out.is_open()) {
		out << result;
		out.close();
		std::cout << "[GameScene] Serialized to " << filePath << "\n";
	}
	else {
		std::cerr << "[GameScene] WARNING :: Could not write to " << filePath << "\n";
	}

	return result;
}