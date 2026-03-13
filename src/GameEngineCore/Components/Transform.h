#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

#include "Component.h"
 
class Transform : public Component
{	
private:
	Transform();
	glm::vec3 position;
	glm::vec3 rotation; // store float angles of rotation on X,Y,Z
	glm::vec3 scale;
public:
	void setPosition(glm::vec3 position);
	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);

	glm::mat4 getModel();
	glm::mat4 getPosition();
	glm::mat4 getRotation();
	glm::mat4 getScale();

	void start() override;
	void update() override;
	void end() override;
};

#endif

