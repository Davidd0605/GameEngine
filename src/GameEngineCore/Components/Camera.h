#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../GameObjects/GameObject.h"  
/// <summary>
/// Camera component.
/// Reads position and forward vector from the Transform on the same game object.
/// </summary>
class Camera : public Component {
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

    glm::mat4 getVPMatrix();
    void setAspectRatio(float aspectRatio);

    void start() override;
    void update() override;
    void end() override;

private:
    void recompute();

    Transform* transform;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float nearPlane;
    float farPlane;
    float fov;
    float aspectRatio;
};