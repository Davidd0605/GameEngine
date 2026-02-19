#ifndef  GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.h"

class gameObject {

public:
	
	std::string name;
	//std::vector<Functionality*>

	void start();
	void update();
	void end();

	void render();

	template<typename T> T* getComponent();
	void addComponent(Component* component);
	template<typename T> T* removeComponent();

	//add, remove, get functionalities, to be added when functionality sys is in place.

private:
	std::vector<gameObject*> children;
	std::vector<Component*> components;
};
#endif // ! GAME_OBJECT_H


