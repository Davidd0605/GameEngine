#pragma once
#include <glm/glm.hpp>

/// <summary>
/// Class for camera obejct. 
/// Camera will be used to determine the view of the scene.
/// It will be used to determine the position and orientation of the camera in the scene, 
/// as well as the projection type (perspective or orthographic) and other parameters such as field of view and aspect ratio.
/// </summary>
class Camera
{
public:
	Camera();

	glm::mat4 getVPMatrix();
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 position;
	glm::vec3 target;

	float nearPlane;
	float farPlane;

	float fieldOfView;
	float aspectRatio;


};

