#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "../Components/Camera.h"
#include "../GameObjects/GameObject.h"
#include <vector>
/// <summary>
/// Abstract class for the scene.
/// </summary>
class Scene {

public:
	Scene(std::string name);
	Scene(std::string name, gameObject* mainCamera);

	//Must be overriden
	void virtual update() = 0;
	void virtual start() = 0;
	void virtual end() = 0;
	void virtual fixedUpdate() = 0;

	std::string name;

	void setMainCamera(gameObject* mainCamera);

	gameObject* getMainCamera();
	std::vector<gameObject*> getCameras();
	std::vector<gameObject*> getLights();
	
protected:
	gameObject* mainCamera;
	std::vector<gameObject*> cameras;
	std::vector<gameObject*> lights;
};

#endif // !SCENE_H
