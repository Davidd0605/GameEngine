#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) {
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->transform = nullptr;
    this->viewMatrix = glm::mat4(1.0f);
    this->projectionMatrix = glm::mat4(1.0f);
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    recompute();
}

void Camera::start() {
    transform = owner->getComponent<Transform>();
    if (transform == nullptr) {
        // No transform on this game object, default to origin looking at -Z
        viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }
    recompute();
}

void Camera::update() {
    recompute();
}

void Camera::end() {}

void Camera::recompute() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

    if (transform != nullptr) {
        glm::vec3 position = transform->getPosition();
        glm::vec3 forward = transform->getForward();
        viewMatrix = glm::lookAt(position, position + forward, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else {
        std::cerr << "[Camera] ERROR :: CAMERA TRANSFORM NULL; \n";
    } 
}

glm::mat4 Camera::getVPMatrix() {
    return projectionMatrix * viewMatrix;
}
void Camera::setFov(float fov) {
    this->fov = glm::clamp(fov, 10.0f, 120.0f);
    recompute();
}

float Camera::getFov() {
    return fov;
}
void Camera::fixedUpdate() {}
