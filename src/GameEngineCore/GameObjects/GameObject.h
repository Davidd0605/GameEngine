#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include <stdexcept>
#include "../Components/Component.h"
#include "../Components/Mesh.h"
#include "../Utilities/Interfaces/ISerializable.h"

/// <summary>
/// Class for the game object.
/// Responsible for storing the name of the game object, as well as its components, functionalities and children.
/// 
/// The game object can be used to represent any kind of object in the game, such as a player, an enemy, a projectile, etc.
/// In order to avoid excessive complexity introduced by inheritance, the GO system makes use of the decorator pattern via composition.
/// </summary>
class gameObject : public ISerializable {
public:
	std::string name;

	gameObject(std::string name, std::string modelPath = "");
	void start();
	void update();
	void fixedUpdate();
	void end();
	void render();

	std::string modelPath = "";

	// Set to true for mesh children spawned by ModelLoader so they are
	// excluded from scene serialization (the parent's modelPath is enough
	// to reconstruct the whole hierarchy on load).
	bool isModelChild = false;

	void addComponent(Component* component);

	template<typename T> T* getComponent() {
		for (Component* comp : components) {
			T* casted = dynamic_cast<T*>(comp);
			if (casted != nullptr) return casted;
		}
		return nullptr;
	}

	template<typename T> T* removeComponent() {
		for (auto it = components.begin(); it != components.end(); ++it) {
			T* casted = dynamic_cast<T*>(*it);
			if (casted != nullptr) {
				components.erase(it);
				return casted;
			}
		}
		return nullptr;
	}

	std::string serialize() override;


private:
	std::vector<Component*> components;
};

#endif // !GAME_OBJECT_H
