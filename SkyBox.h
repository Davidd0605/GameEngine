#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <iostream>

#include "src/GameEngineCore/Rendering/GPUobjects/EBO.h"
#include "src/GameEngineCore/Rendering/GPUobjects/VAO.h"
#include "src/GameEngineCore/Rendering/GPUobjects/VBO.h"
#include "src/GameEngineCore/Rendering/ShaderPass.h"
class SkyBox
{
public:
	SkyBox(std::vector<std::string> paths);
	void render(glm::mat4 view, glm::mat4 projection);
private:

	std::vector<std::string> paths;
	VBO* vbo;
	VAO* vao;
	EBO* ebo;
	GLuint textureID; //cubemap texture, not supported by the rest of the engine rn.
	ShaderPass* sbShaderPass;
};

#endif

// Implement render method if needed
