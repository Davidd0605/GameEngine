#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "GameSystem.h"
#include "../Scenes/GameScene.h"
#include "../GameObjects/GameObject.h"
#include "../Rendering/GPUobjects/VAO.h"
#include "../Rendering/ShaderPass.h"
#include "../Components/Mesh.h"
#include "../Components/Transform.h"
#include <vector>
/// <summary>
/// System responsible for drawing all meshes in a game scene
/// </summary>
class RenderSystem : public GameSystem
{
public:
	void start() override;
	void update() override;
	void fixedUpdate() override;
	void end() override;

	void setCurrentScene(GameScene* scene) override;
	void clearCurrentScene() override;
protected:
	using GameSystem::currentScene;
	void draw();
};
#endif

