#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "../GameObjects/GameObject.h"
#include <vector>
class Transform : public Component {
public:
    Transform();

    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    void setRotation(glm::vec3 rotationAxis, float degrees);
    void setRotationX(float x);
    void setRotationY(float y);
    void setRotationZ(float z);
    void translate(glm::vec3 translation);
    glm::mat4 getModel();
    glm::vec3 getPosition();
    glm::vec3 getForward();

    void start() override;
    void update() override;
    void fixedUpdate() override;
    void end() override;

	void addChild(Transform* child);
	void removeChild(Transform* child);
	std::vector<Transform*> getChildren();
	std::vector<gameObject*> getChildrenGameObjects();
    void setParent(Transform* parent);

	std::string serialize() override;
private:
    void computeModel();

	std::vector<Transform*> children;


    glm::mat4 originSystem; // origin system of the parent object, applied to model transformation
	// model matrix of parent = originSystem of children
    Transform* parent;

    glm::vec3 position;
    glm::vec3 rotation; // radians on X, Y, Z
    glm::vec3 scale;
    glm::mat4 model;

};

#endif