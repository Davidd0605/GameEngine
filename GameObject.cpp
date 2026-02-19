#include "GameObject.h"


void gameObject::start() {

	for (auto component : components) {
		component->start();
	}

	//TODO: start all children game objects
	//TODO: start all functionalities
}

void gameObject::update() {
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

void gameObject::addComponent(Component* component) {
	//TODO: check if component of same type already exists in the components vector, if so, throw error
	components.push_back(component);
}
