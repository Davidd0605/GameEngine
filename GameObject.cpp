#include "GameObject.h"


void gameObject::start()
{
}

void gameObject::update()
{
}

void gameObject::end()
{
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
