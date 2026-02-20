#include "GameScene.h"
#include <iostream>

void GameScene::update()
{
	std::cout << "Updating game scene " + name << std::endl;
	for (auto go : gameObjects) {
		go->update();
	}
}

void GameScene::start()
{
	std::cout << "Starting game scene " + name << std::endl;

	for (auto go : gameObjects) {
		go->start();
	}
}

void GameScene::end()
{
	std::cout << "Ending game scene " + name << std::endl;
	for (auto go : gameObjects) {
		go->end();
	}
}

void GameScene::addObject(gameObject* go) {
	this->gameObjects.push_back(go);
}