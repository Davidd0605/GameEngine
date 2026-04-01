#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../GameObjects/GameObject.h"
#include "../Rendering/GPUobjects/FBO.h"
/// <summary>
/// Camera component.
/// Reads position and forward vector from the Transform on the same game object.
/// </summary>
class Camera : public Component {
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane, int priority = 0, bool postprocessing = false);

    glm::mat4 getVPMatrix();
    void setAspectRatio(float aspectRatio);

    void start() override;
    void update() override;
    void fixedUpdate() override;
    void end() override;
    void setFov(float fov);
    float getFov();
    int priority;
	
    bool postProcessing = false; // whether this camera should be rendered to a framebuffer for post processing effects. If true, will be rendered after all non-post-processing cameras regardless of priority.
	FBO* getFBO() { return this->fbo; }
private:
    void recompute();

    Transform* transform;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float nearPlane;
    float farPlane;
    float fov;
    float aspectRatio;

    FBO* fbo;
};