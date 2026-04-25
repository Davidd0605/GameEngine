#ifndef LIGHTSPIN_H
#define LIGHTSPIN_H

#include "../Functionalities/Functionality.h"
#include "../GameObjects/GameObject.h"
#include "../Components/Transform.h"
#include "../Components/Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// just to see smoke in action
class lightSpin : public Functionality {

private:
    gameObject* owner;
    float speed = 1.0f; // rotation speed

public:
    lightSpin(gameObject* owner) {
        this->owner = owner;
    }

    void start() override {}

    void update() override {
        Light* light = owner->getComponent<Light>();
        if (!light) return;

        // Get current direction
        glm::vec3 dir = light->getDirection();

        // Rotate around Y axis
        float time = glfwGetTime();
        float angle = time * speed / 1000;

        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

        glm::vec3 newDir = glm::normalize(glm::vec3(rot * glm::vec4(dir, 0.0)));

        light->setDirection(newDir);
    }

    void end() override {}
};

#endif // LIGHTSPIN_H