#ifndef SPIN_H
#define SPIN_H
#include "src/GameEngineCore/Functionalities/Functionality.h"
#include "src/GameEngineCore/GameObjects/GameObject.h"
#include "src/GameEngineCore/Components/Transform.h"
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

class Spin : public Functionality {
private:
    float speed;
public:
    Spin(float speed = 1.0f) : speed(speed) {}
    void start() override {}
    void update() override {
        owner->getComponent<Transform>()->setRotationY(glfwGetTime() * speed * 50.0f);
    }
    void end() override {}
};
#endif