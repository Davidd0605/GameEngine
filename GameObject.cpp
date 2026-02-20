#include "GameObject.h"

gameObject::gameObject(std::string name) {
	this->name = name;
}
void gameObject::start() {
	
	for (auto component : components) {
		component->start();
	}

	//TODO: start all children game objects
	//TODO: start all functionalities
}

void gameObject::update() {
	this->render();
	for (auto component : components) {
		component->update();
	}
}

void gameObject::end()
{
	for (auto component : components) {
		component->end();
	}
}

void gameObject::render()
{

	//Check if we have a mesh to render
	if (this->getComponent<Mesh>()) {
		Mesh* mesh = this->getComponent<Mesh>();
		mesh->Draw();
	}
	else {
		std::cout << "Tried rendering without mesh";
	}
}

template<typename T> T* gameObject::getComponent() {
	for (Component* comp : components) {
		T* casted = dynamic_cast<T*>(comp); // is component of type T?
		if (casted != nullptr) {
			return casted;
		}
	}
	return nullptr;
}

//TODO
template<typename T> T* gameObject::addComponent()
{
	return;
	// Check if component already exists
	if (getComponent<T>() != nullptr)
	{
		throw std::runtime_error("Component already exists on GameObject.");
	}

	T* newComponent = new T();
	components.push_back(newComponent);

	return newComponent;
}

//TODO
template <typename T> T* gameObject::removeComponent() {
	return;
}
