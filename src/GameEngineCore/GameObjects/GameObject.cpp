#include "GameObject.h"

gameObject::gameObject(std::string name) {
	this->name = name;
}

void gameObject::start() {
	for (auto component : components) {
		component->start();
	}
}

void gameObject::update() {
	for (auto component : components) {
		component->update();
	}
}

void gameObject::fixedUpdate() {
	for (auto component : components) {
		component->fixedUpdate();
	}
}

void gameObject::end() {
	for (auto component : components) {
		component->end();
	}
	free(this);
}

void gameObject::render() {
	// deprecated
}

void gameObject::addComponent(Component* component) {
	for (auto comp : components) {
		if (typeid(*comp) == typeid(*component)) {
			throw std::runtime_error("Component of this type already exists");
		}
	}
	component->owner = this;
	components.push_back(component);
}