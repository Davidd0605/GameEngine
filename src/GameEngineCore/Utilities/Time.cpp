#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::time = 0.0f;
float Time::timeScale = 1.0f;
float Time::lastFrame = 0.0f;
float Time::fpsTimer = 0.0f;
int Time::fpsCounter = 0;
int Time::fps = 0;
float Time::avgFps = 0.0f;

void Time::update() {
	float current = (float)glfwGetTime();
	deltaTime = (current - lastFrame) * timeScale;
	lastFrame = current;
	time = current;

	fpsCounter++;
	fpsTimer += deltaTime;
	if (fpsTimer >= 1.0f) {
		fps = fpsCounter;
		if (avgFps == 0) avgFps = fps;
		else avgFps = avgFps * 0.9f + fps * 0.1f;
		fpsCounter = 0;
		fpsTimer = 0.0f;
	}
}