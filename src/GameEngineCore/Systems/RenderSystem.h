#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "GameSystem.h"
#include "../Scenes/GameScene.h"
#include "../GameObjects/GameObject.h"
#include "../Rendering/GPUobjects/VAO.h"
#include "../Rendering/ShaderPass.h"
#include "../Components/Mesh.h"
#include "../Components/Transform.h"
#include "../Rendering/ShaderPass.h"
#include <glad/glad.h>
#include <vector>

/// <summary>
/// RenderSystem is responsible for rendering the game objects in the current scene. 
/// It uses the main camera to determine the view and projection matrices, and renders all objects with a Mesh component. 
/// It also handles post-processing effects by applying a series of ShaderPasses to the rendered image before blitting it to the screen.
/// 
/// Has default post-processing pass that simply blits the camera FBO texture to the screen, 
/// but users can add their own custom ShaderPasses for effects like bloom, color grading, etc.
/// 
/// Post processing is enabled by main camera's postProcessing boolean. 
/// If disabled, the system will render directly to the default framebuffer without applying post-processing effects.
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

	bool postProcessingEnabled = false;

	void addPostProcessingShaderPass(ShaderPass* sp);
	void clearPostProcessingShaderPasses();
    std::vector<ShaderPass*> getPostProcessingShaderPasses();
protected:
    using GameSystem::currentScene;
    void draw();

private:
    void renderSceneObjects(gameObject* camGO);
    void blitToScreen(GLuint textureID, GLuint depthID, ShaderPass* sp);
	std::vector<ShaderPass*> postProcessingShaderPasses;
};

#endif