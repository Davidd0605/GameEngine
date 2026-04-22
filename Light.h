#ifndef LIGHT_H
#define LIGHT_H
#include "src/GameEngineCore/Components/Component.h"
#include "LightTypes.h"
#include <glm/glm.hpp>

class Light : public Component {
private:
    LightType lightType;
    glm::vec3 lightColor;
    float intensity;
    float range;
    float radius;
    glm::vec3 direction;

public:
    void start() override;
    void update() override;
    void fixedUpdate() override;
    void end() override;

    glm::vec3 getColor();
    glm::vec3 getDirection();
    float getIntensity();
    float getRange();
    float getRadius();
    LightType getType();

    Light(LightType lightType, float intensity = 1.0f, glm::vec3 lightColor = glm::vec3(1),
        float range = 10.0f, glm::vec3 direction = glm::vec3(0, -1, 0), float radius = 1.0f);
	void setDirection(glm::vec3 newDirection);
	void setColor(glm::vec3 newColor);
};
#endif