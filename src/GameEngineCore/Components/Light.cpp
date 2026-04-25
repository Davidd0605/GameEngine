#include "Light.h"

void Light::update() {}
void Light::start() {}
void Light::end() {}
void Light::fixedUpdate() {}

Light::Light(LightType lightType, float intensity, glm::vec3 lightColor, float range, glm::vec3 direction, float radius) {
    this->lightType = lightType;
    this->lightColor = lightColor;
    this->intensity = intensity;
    this->range = range;
    this->direction = direction;
    this->radius = radius;
}

float Light::getIntensity() { return this->intensity; }
float Light::getRange() { return this->range; }
float Light::getRadius() { return this->radius; }
glm::vec3 Light::getColor() { return this->lightColor; }
glm::vec3 Light::getDirection() { return glm::normalize(this->direction); }

void Light::setDirection(glm::vec3 newDirection) {
    this->direction = glm::normalize(newDirection);
}
void Light::setColor(glm::vec3 newColor) {
    this->lightColor = newColor;
}
LightType Light::getType() { return this->lightType; }