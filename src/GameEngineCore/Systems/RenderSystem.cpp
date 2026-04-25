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

GLuint rectVAO = 0;
GLuint rectVBO = 0;
FBO* fbos[2];

static void uploadLightsToShader(ShaderPass* sp, const std::vector<gameObject*>& lights) {
	int dirCount = 0;
	int pointCount = 0;

	for (int k = 0; k < (int)lights.size() && dirCount < 32; k++) {
		Light* light = lights[k]->getComponent<Light>();
		Transform* transform = lights[k]->getComponent<Transform>();
		if (!light || !transform) continue;

		std::string base;
		switch (light->getType()) {
		case LightType::Point:
			base = "pointLights[" + std::to_string(pointCount) + "]";
			sp->setVec3(base + ".position", transform->getPosition());
			sp->setVec3(base + ".color", light->getColor());
			sp->setFloat(base + ".intensity", light->getIntensity());
			sp->setFloat(base + ".range", light->getRange());
			pointCount++;
			break;
		case LightType::Directional:
			base = "dirLights[" + std::to_string(dirCount) + "]";
			sp->setVec3(base + ".position", transform->getPosition());
			sp->setVec3(base + ".direction", light->getDirection());
			sp->setVec3(base + ".color", light->getColor());
			sp->setFloat(base + ".intensity", light->getIntensity());
			sp->setFloat(base + ".range", light->getRange());
			dirCount++;
			break;
		default:
			std::cout << "[RenderSystem] WARNING :: unrecognized light type on: " << lights[k]->name << "\n";
			break;
		}
	}

	sp->setInt("pointLightCount", pointCount);
	sp->setInt("dirLightCount", dirCount);
}

void RenderSystem::start() {
	ShaderPass* fsSP = new ShaderPass("src/Shaders/utility/plainFBO.frag", "src/Shaders/utility/plainFBO.vert");
	this->addPostProcessingShaderPass(fsSP);

	fbos[0] = new FBO();
	fbos[1] = new FBO();

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

void RenderSystem::update() { this->draw(); }
void RenderSystem::fixedUpdate() {}

void RenderSystem::end() {
	this->clearCurrentScene();
	glDeleteVertexArrays(1, &rectVAO);
	glDeleteBuffers(1, &rectVBO);
	fbos[0]->Delete();
	fbos[1]->Delete();
}

void RenderSystem::blitToScreen(GLuint textureID, GLuint depthID, ShaderPass* sp) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sp->bind();
	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	sp->setInt("screenTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthID);
	sp->setInt("depthTexture", 1);

	// system uniforms
	sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
	sp->setMatrix4("cameraVP", currentScene->getMainCamera()->getComponent<Camera>()->getVPMatrix());
	sp->setFloat("time", glfwGetTime());
	uploadLightsToShader(sp, currentScene->getLights());

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sp->unbind();

	//deactivate textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderSystem::renderSceneObjects(gameObject* camGO) {
	const std::vector<gameObject*>& gos = this->currentScene->getGameObjects();
	Camera* cam = camGO->getComponent<Camera>();

	for (auto go : gos) {
		Mesh* ms = go->getComponent<Mesh>();
		if (ms == nullptr) continue;

		Material* mat = ms->getMaterial();
		VAO* vao = ms->getVAO();

		if (vao == nullptr || mat == nullptr) {
			std::cerr << "[RenderSystem] ERROR :: NO VAO OR MATERIAL, SKIPPING: " << go->name << "\n";
			continue;
		}

		// bind material — uploads all material uniforms and textures, binds shader
		mat->bind();

		vao->Bind();

		// system uniforms — set after material bind, these are engine-owned
		ShaderPass* sp = mat->getShaderPass();

		Transform* ts = go->getComponent<Transform>();
		glm::mat4 model = glm::mat4(1.0f);
		if (ts != nullptr) {
			model = ts->getModel();
		}
		else {
			std::cerr << "[RenderSystem] WARNING :: NO TRANSFORM ON: " << go->name << ", USING IDENTITY\n";
		}

		//system uniforms
		sp->setMatrix4("model", model);
		sp->setMatrix4("VP", cam->getVPMatrix());
		sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
		sp->setVec3("viewPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
		sp->setFloat("time", glfwGetTime());

		// light objects expose their color to the light shader
		Light* lightComp = go->getComponent<Light>();
		if (lightComp != nullptr)
			sp->setVec3("color", lightComp->getColor());

		uploadLightsToShader(sp, currentScene->getLights());

		ms->draw();
		mat->unbind();
		vao->Unbind();
	}
}

void RenderSystem::draw() {
	gameObject* mainCamGO = this->currentScene->getMainCamera();
	Camera* mainCam = mainCamGO->getComponent<Camera>();

	// render to FBO for all post-processing cameras
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

	if (mainCam->postProcessing) {
		GLuint srcTexture = mainCam->getFBO()->getTexture()->ID;
		GLuint dpthTexture = mainCam->getFBO()->getDepthTexture()->ID;

		int passCount = (int)postProcessingShaderPasses.size();
		for (int i = 0; i < passCount; i++) {
			bool isLast = (i == passCount - 1);
			int dstIndex = i % 2;

			if (isLast) {
				blitToScreen(srcTexture, dpthTexture, postProcessingShaderPasses[i]);
			}
			else {
				fbos[dstIndex]->Bind();
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				ShaderPass* sp = postProcessingShaderPasses[i];
				sp->bind();
				glBindVertexArray(rectVAO);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, srcTexture);
				sp->setInt("screenTexture", 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, dpthTexture);
				sp->setInt("depthTexture", 1);

				sp->setFloat("time", glfwGetTime());
				sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
				sp->setMatrix4("cameraVP", currentScene->getMainCamera()->getComponent<Camera>()->getVPMatrix());
				uploadLightsToShader(sp, currentScene->getLights());

				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
				sp->unbind();
				fbos[dstIndex]->Unbind();

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				srcTexture = fbos[dstIndex]->getTexture()->ID;
			}
		}
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		renderSceneObjects(mainCamGO);
	}
}

void RenderSystem::setCurrentScene(GameScene* scene) { this->currentScene = scene; }
void RenderSystem::clearCurrentScene() { this->currentScene = nullptr; }

void RenderSystem::addPostProcessingShaderPass(ShaderPass* sp) {
	this->postProcessingShaderPasses.push_back(sp);
}

std::vector<ShaderPass*> RenderSystem::getPostProcessingShaderPasses() {
	return this->postProcessingShaderPasses;
}

void RenderSystem::clearPostProcessingShaderPasses() {
	this->postProcessingShaderPasses.clear();
	this->addPostProcessingShaderPass(
		new ShaderPass("src/Shaders/utility/plainFBO.frag", "src/Shaders/utility/plainFBO.vert")
	);
}