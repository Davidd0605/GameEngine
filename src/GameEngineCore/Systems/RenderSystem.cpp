#include "RenderSystem.h"


//hardcoded rectangle vertices for post-processing blit (NDC coords + UVs)
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

// pingpong FBOs for post processing
FBO* fbos[2];

void RenderSystem::start() {
	ShaderPass* fsSP = new ShaderPass("src/Shaders/plainFBO.frag", "src/Shaders/plainFBO.vert");

	// Add a default post-processing pass (can be overridden by user)
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

void RenderSystem::update() {
	this->draw();
}

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
	sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
	sp->setMatrix4("cameraVP", currentScene->getMainCamera()->getComponent<Camera>()->getVPMatrix());
	sp->setFloat("time", glfwGetTime());


	// Bind light values (needs lights and currently using SP)
	std::vector<gameObject*> lights = currentScene->getLights();
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
			sp->setVec3(base + ".direction", light->getDirection());
			sp->setVec3(base + ".color", light->getColor());
			sp->setFloat(base + ".intensity", light->getIntensity());
			sp->setFloat(base + ".range", light->getRange());
			dirCount++;
			break;
		default:
			std::cout << "Other light not recognized yet: " << base << "\n";
			break;
		}
	}


	sp->setInt("pointLightCount", pointCount);
	sp->setInt("dirLightCount", dirCount);

	//draw fullscreen quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sp->unbind();
}

// render all objects with their own shaders
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
		sp->setVec3("viewPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
		sp->setMatrix4("model", model);
		sp->setMatrix4("VP", cam->getVPMatrix());

		Light* lightComp = go->getComponent<Light>();
		if (lightComp != nullptr)
			sp->setVec3("color", lightComp->getColor());

		std::vector<gameObject*> lights = currentScene->getLights();
		int total = std::min((int)lights.size(), 32);
		int pointCount = 0, dirCount = 0;

		for (int i = 0; i < total; i++) {
			Light* light = lights[i]->getComponent<Light>();
			Transform* transform = lights[i]->getComponent<Transform>();
			if (!light || !transform) continue;

			if (light->getType() == LightType::Point) {
				std::string base = "pointLights[" + std::to_string(pointCount) + "]";
				sp->setVec3(base + ".position", transform->getPosition());
				sp->setVec3(base + ".color", light->getColor());
				sp->setFloat(base + ".intensity", light->getIntensity());
				sp->setFloat(base + ".range", light->getRange());
				pointCount++;
			}
			else if (light->getType() == LightType::Directional) {
				std::string base = "dirLights[" + std::to_string(dirCount) + "]";
				sp->setVec3(base + ".direction", light->getDirection());
				sp->setVec3(base + ".color", light->getColor());
				sp->setFloat(base + ".intensity", light->getIntensity());
				sp->setFloat(base + ".range", light->getRange());
				dirCount++;
			}
		}

		sp->setInt("pointLightCount", pointCount);
		sp->setInt("dirLightCount", dirCount);

		ms->draw();
		sp->unbind();
		vao->Unbind();
	}
}

// Main draw function that handles rendering the scene with or without post-processing based on the main camera's settings
void RenderSystem::draw() {
	//fetch main camera
	gameObject* mainCamGO = this->currentScene->getMainCamera();
	Camera* mainCam = mainCamGO->getComponent<Camera>();

	// render scene for all cameras that have an FBO (post-processing enabled) to their FBOs
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

	// How we render
	if (mainCam->postProcessing) {
		// ping-pong between fbos[0] and fbos[1]

		// first pass reads from the camera FBO
		GLuint srcTexture = mainCam->getFBO()->getTexture()->ID;
		GLuint dpthTexture = mainCam->getFBO()->getDepthTexture()->ID;

		int passCount = (int)postProcessingShaderPasses.size();
		for (int i = 0; i < passCount; i++) {
			bool isLast = (i == passCount - 1);
			int dstIndex = i % 2;

			if (isLast) {
				// Final pass: blit straight to the default framebuffer
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

				//set textures
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, srcTexture);
				sp->setInt("screenTexture", 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, dpthTexture);
				sp->setInt("depthTexture", 1);

				//set basic uniforms
				sp->setFloat("time", glfwGetTime());
				sp->setVec3("mainCameraPos", currentScene->getMainCamera()->getComponent<Transform>()->getPosition());
				sp->setMatrix4("cameraVP", currentScene->getMainCamera()->getComponent<Camera>()->getVPMatrix());

				// Upload directional lights for post-processing shaders (e.g. volumetric fog)
				std::vector<gameObject*> lights = currentScene->getLights();
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
							sp->setVec3(base + ".direction", light->getDirection());
							sp->setVec3(base + ".color", light->getColor());
							sp->setFloat(base + ".intensity", light->getIntensity());
							sp->setFloat(base + ".range", light->getRange());
							dirCount++;
							break;
						default:
							std::cout << "Other light not recognized yet: " << base << "\n";
							break;
					}
				}


				sp->setInt("pointLightCount", pointCount);
				sp->setInt("dirLightCount", dirCount);
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
		// no post processing just render directly to screen
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

void RenderSystem::addPostProcessingShaderPass(ShaderPass* sp) {
	this->postProcessingShaderPasses.push_back(sp);
}

std::vector<ShaderPass*> RenderSystem::getPostProcessingShaderPasses() {
	return this->postProcessingShaderPasses;
}

void RenderSystem::clearPostProcessingShaderPasses() {
	this->postProcessingShaderPasses.clear();
	ShaderPass* fsSP = new ShaderPass("src/Shaders/plainFBO.frag", "src/Shaders/plainFBO.vert");

	this->addPostProcessingShaderPass(fsSP);
}