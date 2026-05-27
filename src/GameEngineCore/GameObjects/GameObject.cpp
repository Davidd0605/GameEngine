#include "GameObject.h"
#include <json/json.h>

gameObject::gameObject(std::string name, std::string modelPath) {
	this->modelPath = modelPath;
	this->name = name;
	this->isModelChild = false;
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

std::string gameObject::serialize() {
	nlohmann::json j;
	j["name"] = name;

	if (!modelPath.empty()) {
		j["modelPath"] = modelPath;
	}

	for (auto component : components) {
		std::string typeName = typeid(*component).name();
		std::string componentJson = component->serialize();

		// Only embed non-empty serializations as proper nested objects.
		// Empty string means the component opted out (e.g. Functionality subclasses).
		if (!componentJson.empty()) {
			try {
				j["components"][typeName] = nlohmann::json::parse(componentJson);
			}
			catch (...) {
				// If it somehow isn't valid JSON, store as plain string so we don't lose it.
				j["components"][typeName] = componentJson;
			}
		}
	}

	return j.dump(2);
}

