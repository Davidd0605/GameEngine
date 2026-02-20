#ifndef  GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.h"
#include "Mesh.h"

class gameObject {

public:
	
	std::string name;

	gameObject(std::string name);
	void start();
	void update();
	void end();

	void render();

	template<typename T> T* getComponent();
	template<typename T> T* addComponent();
	template<typename T> T* removeComponent();

	//add, remove, get functionalities, to be added when functionality sys is in place.

private:
	std::vector<gameObject*> children;
	std::vector<Component*> components;
	//std::vector<Functionality*>
};
#endif // ! GAME_OBJECT_H


