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
	const std::vector<gameObject*>& gos = this->currentScene->getGameObjects();
	for (auto go : gos) {
		Mesh* ms = go->getComponent<Mesh>();
		if (ms == nullptr) continue;

		ShaderPass* sp = ms->getShaderPass();
		VAO* vao = ms->getVAO();
		if (vao == nullptr || sp == nullptr) {
			std::cerr << "ERROR :: NO VAO OR SHADERPASS, SKIPPING: " << go->name;
			continue;
		}

		sp->bind();
		vao->Bind();
		Transform* ts = go->getComponent<Transform>();

		//Default behavior
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		glm::mat4 VP = projection * view;
		
		gameObject* camera = currentScene->getMainCamera();
		if (camera == nullptr) {
			std::cerr << "ERROR :: ATTEMPTING TO RENDER WITHOUT MAIN CAMERA; \n";
		}
		else {
			VP = camera->getComponent<Camera>()->getVPMatrix();
		}
		
		if (ts != nullptr) {
			model = ts->getModel();
		}
		else {
			std::cerr << "ERROR :: NO TRANSFORM COMPONENT FOUND WHEN ATTEMPTING TO DRAW GAMEOBEJCT: " << go->name << " USING DEFAULT BEHAVIOR;\n";
			model = glm::mat4(1);
		}

		sp->setFloat("time", glfwGetTime());
		sp->setMatrix4("model", model);
		sp->setMatrix4("VP", VP);

		EBO* ebo = ms->getEBO();
		if (ebo) ms->drawEBO();
		else ms->draw();

		sp->unbind();
		vao->Unbind();
	}
}

void RenderSystem::setCurrentScene(GameScene* scene) {
	this->currentScene = scene;
}

void RenderSystem::clearCurrentScene() {
	this->currentScene = nullptr;
}

