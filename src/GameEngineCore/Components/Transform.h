#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"

class Transform : public Component {
public:
    Transform();

    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    void setRotation(glm::vec3 rotationAxis, float degrees);
    void setRotationX(float x);
    void setRotationY(float y);
    void setRotationZ(float z);

    glm::mat4 getModel();
    glm::vec3 getPosition();
    glm::vec3 getForward();

    void start() override;
    void update() override;
    void end() override;

private:
    void computeModel();

    glm::vec3 position;
    glm::vec3 rotation; // radians on X, Y, Z
    glm::vec3 scale;
    glm::mat4 model;
};

#endif