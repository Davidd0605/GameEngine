#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include "Scene.h"
#include <vector>
#include <algorithm>
#include "../GameObjects/GameObject.h"
#include "../Systems/GameSystem.h"
#include "../../Light.h"

/// <summary>
/// Class for the game scene.
/// Responsible for storing the game objects in the scene, as well as their update and render order.
/// </summary>
class GameScene : public Scene
{
public:
	using Scene::Scene;
	using Scene::setMainCamera;
	using Scene::getMainCamera;
	using Scene::getCameras;
	using Scene::getLights;

	void fixedUpdate();
	void update();
	void start();
	void end();


	void addObject(gameObject* go);
	void removeObject(gameObject* go);

	void addSystem(GameSystem* system);
	void removeSystem(GameSystem* system);

	std::vector<gameObject*> getGameObjects();
	std::vector<GameSystem*> getSystems();

	gameObject* findObjectByName(const std::string& name);
	template<typename T>
	T* getSystem() {
		for (GameSystem* system : systems) {
			T* result = dynamic_cast<T*>(system);
			if (result != nullptr) return result;
		}
		return nullptr;
	}

private:
	std::vector<gameObject*> gameObjects;
	std::vector<GameSystem*> systems;

	void sortCameras();
};

#endif // !GAME_SCENE_H



