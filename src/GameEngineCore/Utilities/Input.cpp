#include "Input.h"
#include <cstring>

// Static member definitions
GLFWwindow* Input::window = nullptr;
int Input::prevKeyStates[GLFW_KEY_LAST] = {};
int Input::currKeyStates[GLFW_KEY_LAST] = {};
int Input::prevMouseStates[GLFW_MOUSE_BUTTON_LAST] = {};
int Input::currMouseStates[GLFW_MOUSE_BUTTON_LAST] = {};
glm::vec2 Input::mousePosition = glm::vec2(0.0f);
glm::vec2 Input::prevMousePosition = glm::vec2(0.0f);
float Input::scrollDelta = 0.0f;
float Input::scrollBuffer = 0.0f;

void Input::update(GLFWwindow* win) {
	window = win;

	// Snapshot previous states
	memcpy(prevKeyStates, currKeyStates, sizeof(currKeyStates));
	memcpy(prevMouseStates, currMouseStates, sizeof(currMouseStates));
	prevMousePosition = mousePosition;

	// Poll current keyboard states
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		currKeyStates[i] = glfwGetKey(window, i);
	}

	// Poll current mouse button states
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		currMouseStates[i] = glfwGetMouseButton(window, i);
	}

	// Mouse position — skip delta on first frame to avoid giant snap
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	static bool firstFrame = true;
	if (firstFrame) {
		mousePosition = glm::vec2((float)x, (float)y);
		prevMousePosition = mousePosition;
		firstFrame = false;
	}
	else {
		mousePosition = glm::vec2((float)x, (float)y);
	}

	// Consume scroll buffer
	scrollDelta = scrollBuffer;
	scrollBuffer = 0.0f;
}

bool Input::getKey(int keyCode) {
	return currKeyStates[keyCode] == GLFW_PRESS;
}

bool Input::getKeyDown(int keyCode) {
	return currKeyStates[keyCode] == GLFW_PRESS && prevKeyStates[keyCode] == GLFW_RELEASE;
}

bool Input::getKeyUp(int keyCode) {
	return currKeyStates[keyCode] == GLFW_RELEASE && prevKeyStates[keyCode] == GLFW_PRESS;
}

bool Input::getMouseButton(int button) {
	return currMouseStates[button] == GLFW_PRESS;
}

bool Input::getMouseButtonDown(int button) {
	return currMouseStates[button] == GLFW_PRESS && prevMouseStates[button] == GLFW_RELEASE;
}

bool Input::getMouseButtonUp(int button) {
	return currMouseStates[button] == GLFW_RELEASE && prevMouseStates[button] == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() {
	return mousePosition;
}

glm::vec2 Input::getMouseDelta() {
	return mousePosition - prevMousePosition;
}

float Input::getScrollDelta() {
	return scrollDelta;
}

void Input::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	scrollBuffer += (float)yOffset;
}