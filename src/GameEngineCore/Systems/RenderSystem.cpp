#include "RenderSystem.h"

float rectangleVertices[] =
{
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

ShaderPass* fsSP = nullptr;
GLuint rectVAO = 0;
GLuint rectVBO = 0;

void RenderSystem::start() {
	fsSP = new ShaderPass("src/Shaders/plainFBO.frag", "src/Shaders/plainFBO.vert");

	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderSystem::update() {
	this->draw();
}

void RenderSystem::fixedUpdate() {}

void RenderSystem::end() {
	this->clearCurrentScene();
}

void RenderSystem::blitToScreen(GLuint textureID, ShaderPass* sp) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sp->bind();
	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	sp->setFloat("time", glfwGetTime());
	sp->setInt("screenTexture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sp->unbind();
}

void RenderSystem::renderSceneObjects(gameObject* camGO) {
	const std::vector<gameObject*>& gos = this->currentScene->getGameObjects();
	Camera* cam = camGO->getComponent<Camera>();

	for (auto go : gos) {
		Mesh* ms = go->getComponent<Mesh>();
		if (ms == nullptr) continue;

		ShaderPass* sp = ms->getShaderPass();
		VAO* vao = ms->getVAO();
		if (vao == nullptr || sp == nullptr) {
			std::cerr << "ERROR :: NO VAO OR SHADERPASS, SKIPPING: " << go->name << "\n";
			continue;
		}

		sp->bind();
		vao->Bind();

		auto& textures = ms->getTextures();
		for (int i = 0; i < (int)textures.size(); i++) {
			textures[i]->bind(i);
			sp->setInt("tex" + std::to_string(i), i);
		}

		glm::mat4 model = glm::mat4(1);
		Transform* ts = go->getComponent<Transform>();
		if (ts != nullptr) {
			model = ts->getModel();
		}
		else {
			std::cerr << "[RenderSystem] ERROR :: NO TRANSFORM ON: " << go->name << " USING DEFAULT BEHAVIOR;\n";
		}

		sp->setFloat("time", glfwGetTime());
		sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
		sp->setMatrix4("model", model);
		sp->setMatrix4("VP", cam->getVPMatrix());

		Light* lightComp = go->getComponent<Light>();
		if (lightComp != nullptr)
			sp->setVec3("color", lightComp->getColor());

		std::vector<gameObject*> lights = currentScene->getLights();
		int count = std::min((int)lights.size(), 32);
		sp->setInt("lightCount", count);
		for (int i = 0; i < count; i++) {
			Light* light = lights[i]->getComponent<Light>();
			Transform* transform = lights[i]->getComponent<Transform>();
			if (!light || !transform) continue;
			std::string base = "lights[" + std::to_string(i) + "]";
			sp->setVec3(base + ".position", transform->getPosition());
			sp->setVec3(base + ".color", light->getColor());
			sp->setFloat(base + ".intensity", light->getIntensity());
		}

		ms->draw();
		sp->unbind();
		vao->Unbind();
	}
}

void RenderSystem::draw() {
	gameObject* mainCamGO = this->currentScene->getMainCamera();
	Camera* mainCam = mainCamGO->getComponent<Camera>();

	// FILL ALL FBOS
	for (auto cam : this->currentScene->getCameras()) {
		if (cam->getComponent<Camera>()->postProcessing) {

			cam->getComponent<Camera>()->getFBO()->Bind();
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			renderSceneObjects(cam);
			cam->getComponent<Camera>()->getFBO()->Unbind();
		}
	}

	// DECIDE WHAT WE ARE RENDERING
	if (mainCam->postProcessing) {
		blitToScreen(mainCam->getFBO()->getTexture()->ID, fsSP);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		renderSceneObjects(mainCamGO);
	}
}

void RenderSystem::setCurrentScene(GameScene* scene) {
	this->currentScene = scene;
}

void RenderSystem::clearCurrentScene() {
	this->currentScene = nullptr;
}