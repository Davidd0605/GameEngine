#include "RenderSystem.h"

void RenderSystem::start() {

}

void RenderSystem::update() {
	this->draw();
}

void RenderSystem::end() {
	this->clearCurrentScene();
}

void RenderSystem::draw() {
	std::vector<gameObject*> gos = this->currentScene->getGameObjects();
	for (auto go : gos) {

		Mesh* ms = go->getComponent<Mesh>();
		ShaderPass* sp = ms->getShaderPass();
		VAO* vao = ms->getVAO();

		ms->Draw();
	}
}

void RenderSystem::setCurrentScene(GameScene* scene) {
	this->currentScene = scene;
}

void RenderSystem::clearCurrentScene() {
	this->currentScene = nullptr;
}

