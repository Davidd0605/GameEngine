#pragma once
#include "../Functionalities/Functionality.h"
#include "../Components/Transform.h"
#include "../Utilities/Input.h"
#include "../Utilities/Time.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/// <summary>
/// First person camera controller.
/// WASD to move, Space/Shift for up/down, mouse to look.
/// ESC to toggle cursor lock.
/// Attach to a game object that has a Transform component.
/// </summary>
class CameraController : public Functionality {
public:
    CameraController(float speed = 5.0f, float sensitivity = 0.1f, float zoomSpeed = 2.0f)
        : speed(speed), sensitivity(sensitivity) {
    }

    void start() override {
        transform = owner->getComponent<Transform>();
        if (transform == nullptr) {
            std::cerr << "[CameraController] ERROR :: No Transform found on owner\n";
            return;
        }
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstFrame = true;
        locked = true;
    }

    void update() override {
        if (transform == nullptr) return;
        float scroll = Input::getScrollDelta();

        if (scroll != 0.0f) {
            Camera* cam = owner->getComponent<Camera>();
            if (cam) {
                cam->setFov(cam->getFov() - scroll * zoomSpeed);
            }
        }
        if (Input::getKeyDown(GLFW_KEY_ESCAPE)) {
            locked = !locked;
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR,
                locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            firstFrame = true;
        }
        if (locked) {
            glm::vec2 delta = Input::getMouseDelta();

            if (firstFrame) {
                firstFrame = false;
                delta = glm::vec2(0.0f);
            }

            yaw += delta.x * sensitivity;
            pitch += delta.y * sensitivity;

            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            transform->setRotationY(yaw);
            transform->setRotationX(pitch);
        }

        glm::vec3 forward = transform->getForward();
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float step = speed * Time::deltaTime;

        if (Input::getKey(GLFW_KEY_W))          transform->translate(forward * step);
        if (Input::getKey(GLFW_KEY_S))          transform->translate(-forward * step);
        if (Input::getKey(GLFW_KEY_D))          transform->translate(right * step);
        if (Input::getKey(GLFW_KEY_A))          transform->translate(-right * step);
        if (Input::getKey(GLFW_KEY_SPACE))      transform->translate(up * step);
        if (Input::getKey(GLFW_KEY_LEFT_SHIFT)) transform->translate(-up * step);
    }

private:
    Transform* transform = nullptr;
    float speed;
    float sensitivity;
    float yaw = 0.0f;
    float pitch = 0.0f;
    bool firstFrame = true;
    bool locked = true;
    float zoomSpeed = 2.0f;
};