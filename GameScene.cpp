#include "GameScene.h"
#include <iostream>

void GameScene::update()
{
	for (auto go : gameObjects) {
		go->update();
	}
}

void GameScene::start()
{
	for (auto go : gameObjects) {
		go->start();
	}
}

void GameScene::end()
{
	for (auto go : gameObjects) {
		go->end();
	}
}

void GameScene::addObject(gameObject* go) {
	this->gameObjects.push_back(go);
}