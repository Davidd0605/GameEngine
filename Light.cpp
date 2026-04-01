#include "Light.h"

void Light::update() {};
void Light::start() {};
void Light::end() {};
void Light::fixedUpdate() {};

Light::Light(LightType lightType, int intensity, glm::vec3 lightColor) {
	this->lightType = lightType;
	this->lightColor = lightColor;
	this->intensity = intensity;
}


float Light::getIntensity() {
	return this->intensity;
}

glm::vec3 Light::getColor() {
	return this->lightColor;
}

LightType Light::getType() {
	return this->lightType;
}