#include "Transform.h"
Transform::Transform() {
    this->position = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
    computeModel();
}

void Transform::setPosition(glm::vec3 position) {
    this->position = position;
    computeModel();
}

void Transform::setScale(glm::vec3 scale) {
    this->scale = scale;
    computeModel();
}

void Transform::setRotation(glm::vec3 rotationAxis, float degrees) {
    this->rotation = rotationAxis * glm::radians(degrees);
    computeModel();
}

void Transform::setRotationX(float x) {
    this->rotation.x = glm::radians(x);
    computeModel();
}

void Transform::setRotationY(float y) {
    this->rotation.y = glm::radians(y);
    computeModel();
}

void Transform::setRotationZ(float z) {
    this->rotation.z = glm::radians(z);
    computeModel();
}

glm::mat4 Transform::getModel() {
    return model;
}

glm::vec3 Transform::getPosition() {
    return position;
}

glm::vec3 Transform::getForward() {
    glm::vec3 forward;
    forward.x = cos(rotation.x) * sin(rotation.y);
    forward.y = -sin(rotation.x);
    forward.z = -cos(rotation.x) * cos(rotation.y);
    return glm::normalize(forward);
}

void Transform::computeModel() {
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
}

void Transform::translate(glm::vec3 translation) {
    position += translation;
    computeModel();
}

void Transform::start() {}
void Transform::update() {}
void Transform::end() {}void Transform::fixedUpdate() {}
