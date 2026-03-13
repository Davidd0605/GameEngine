#ifndef  GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "../Components/Component.h"
#include "../Components/Mesh.h"

/// <summary>
/// Class for the game object.
/// Responsible for storing the name of the game object, as well as its components, functionalities and children.
/// 
/// The game object can be used to represent any kind of object in the game, such as a player, an enemy, a projectile, etc.
/// In order to avoid excessive complexity introduced by inheritance, the GO system makes use of the decorator pattern via composition.
/// </summary>
class gameObject {

public:
	
	std::string name;

	gameObject(std::string name);
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
	//std::vector<Functionality*>
};
#endif // ! GAME_OBJECT_H


