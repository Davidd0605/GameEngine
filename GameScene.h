#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include "Scene.h"
#include "vector"
#include "GameObject.h"
class GameScene : public Scene
{
	using Scene::Scene;
	void update();
	void start();
	void end();

public:
	void addObject(gameObject* go);
private:
	std::vector<gameObject*> gameObjects;
};

#endif // !GAME_SCENE_H



