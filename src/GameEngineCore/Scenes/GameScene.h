#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include "Scene.h"
#include <vector>
#include "../GameObjects/GameObject.h"
#include "../Systems/GameSystem.h"

/// <summary>
/// Class for the game scene.
/// Responsible for storing the game objects in the scene, as well as their update and render order.
/// </summary>
class GameScene : public Scene
{
	using Scene::Scene;
	using Scene::setMainCamera;
	using Scene::getMainCamera;

	void update();
	void start();
	void end();

public:
	void addObject(gameObject* go);
	void removeObject(gameObject* go);

	void addSystem(GameSystem* system);
	void removeSystem(GameSystem* system);

	std::vector<gameObject*> getGameObjects();
	std::vector<GameSystem*> getSystems();
private:
	std::vector<gameObject*> gameObjects;
	std::vector<GameSystem*> systems;
};

#endif // !GAME_SCENE_H



