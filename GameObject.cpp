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
		std::cerr << "Tried rendering without mesh";
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

void gameObject::addComponent(Component* component) {
	for (auto comp : components) {
		if (typeid(*comp) == typeid(*component)) {
			throw std::runtime_error("Component of this type");
		}
	}
	components.push_back(component);
}

//TODO
template <typename T> T* gameObject::removeComponent() {
	return;
}
