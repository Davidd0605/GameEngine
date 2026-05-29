#include "SkyBox.h"

SkyBox::SkyBox(std::vector<std::string> paths) {

	float vertices[] =
	{
		-1.0f, -1.0f, 1.0f,			//        7--------6
		1.0f, -1.0f, 1.0f,			//       /|       /|
		1.0f, -1.0f, -1.0f,			//      4--------5 |
		-1.0f, -1.0f, -1.0f,		//      | |      | |
		-1.0f, 1.0f, 1.0f,			//      | 3------|-2
		1.0f, 1.0f, 1.0f,			//      |/       |/
		1.0f, 1.0f, -1.0f,			//      0--------1
		-1.0f, 1.0f, -1.0f
	};
	int indices[] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};


	this->paths = paths;
	vbo = nullptr;
	ebo = nullptr;
	vao = new VAO();
	vao->Bind();
	vbo = new VBO(vertices, sizeof(vertices), 24, GL_STATIC_DRAW, 3, 1, new int[1] { 3 });
	ebo = new EBO(indices, 36);
	vao->Unbind();
	if (vbo) vbo->Unbind();
	if (ebo) ebo->Unbind();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Load each face of the cubemap
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "[SKY BOX]Failed to load texture: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	this->sbShaderPass = new ShaderPass("src/Shaders/object/skybox.frag", "src/Shaders/object/skybox.vert");
}

void SkyBox::render(glm::mat4 view, glm::mat4 projection) {
	std::cout << "[SKYBOX] Rendering skybox...\n";
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	this->sbShaderPass->bind();
	
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix

	sbShaderPass->setMatrix4("view", view);
	sbShaderPass->setMatrix4("projection", projection);
	sbShaderPass->setInt("skybox", 0);
	
	vao->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
}
