#pragma once
#include <GLFW/glfw3.h>

/// <summary>
/// Static time class, Unity-style.
/// Call Time::update() once per frame before scene update.
/// </summary>
class Time {
public:
	static void update();

	static float deltaTime;     // time between frames in seconds
	static float time;          // total time since start in seconds
	static float timeScale;     // multiplier, set to 0 to pause
	static int fps;             // frames per second
	static float avgFps;

private:
	static float lastFrame;
	static float fpsTimer;
	static int fpsCounter;
};