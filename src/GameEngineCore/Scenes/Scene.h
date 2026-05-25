#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "../Components/Camera.h"
#include "../GameObjects/GameObject.h"
#include "../Utilities/Interfaces/ISerializable.h"
#include <vector>
struct GLFWwindow;
class Scene {
public:
    Scene(std::string name, GLFWwindow* window);
    Scene(std::string name, GLFWwindow* window, gameObject* mainCamera);

    void virtual update() = 0;
    void virtual start() = 0;
    void virtual end() = 0;
    void virtual fixedUpdate() = 0;

    std::string name;

    void setMainCamera(gameObject* mainCamera);
    gameObject* getMainCamera();
    std::vector<gameObject*> getCameras();
    std::vector<gameObject*> getLights();

    GLFWwindow* getWindow() { return this->window; }

	std::string virtual serialize() = 0;
	void virtual deserialize(std::string data) = 0;
protected:
    gameObject* mainCamera;
    std::vector<gameObject*> cameras;
    std::vector<gameObject*> lights;

    GLFWwindow* window = nullptr;
};

#endif