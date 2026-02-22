#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include "Scene.h"
#include "vector"
#include "GameObject.h"
/// <summary>
/// Class for the game scene.
/// Responsible for storing the game objects in the scene, as well as their update and render order.
/// </summary>
class GameScene : public Scene
{
	using Scene::Scene;
	void update();
	void start();
	void end();

public:
	void addObject(gameObject* go);
	void removeObject(gameObject* go);
	std::vector<gameObject*> getGameObjects();
private:
	std::vector<gameObject*> gameObjects;
	//Add systems for game scene, such as physics, rendering, input, collision etc..
};

#endif // !GAME_SCENE_H



