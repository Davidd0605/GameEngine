#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/// <summary>
/// Static input class, Unity-style.
/// Call Input::update(window) once per frame before scene update.
/// </summary>
class Input {
public:
	static void update(GLFWwindow* window);

	// Keyboard
	static bool getKey(int keyCode);        // held down
	static bool getKeyDown(int keyCode);    // first frame pressed
	static bool getKeyUp(int keyCode);      // first frame released

	// Mouse buttons
	static bool getMouseButton(int button);
	static bool getMouseButtonDown(int button);
	static bool getMouseButtonUp(int button);

	// Mouse position and delta
	static glm::vec2 getMousePosition();
	static glm::vec2 getMouseDelta();
	static float getScrollDelta();

	// Callbacks — register these with GLFW
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
	static GLFWwindow* window;

	static int prevKeyStates[GLFW_KEY_LAST];
	static int currKeyStates[GLFW_KEY_LAST];

	static int prevMouseStates[GLFW_MOUSE_BUTTON_LAST];
	static int currMouseStates[GLFW_MOUSE_BUTTON_LAST];

	static glm::vec2 mousePosition;
	static glm::vec2 prevMousePosition;
	static float scrollDelta;
	static float scrollBuffer;
};