#include "Light.h"
#include <json/json.h>

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

std::string Light::serialize() {
    nlohmann::json j;
    j["lightType"] = static_cast<int>(lightType); // 0=Directional, 1=Point, 2=Spot
    j["color"] = { lightColor.r, lightColor.g, lightColor.b };
    j["intensity"] = intensity;
    j["range"] = range;
    j["radius"] = radius;
    j["direction"] = { direction.x, direction.y, direction.z };
    return j.dump();
}
