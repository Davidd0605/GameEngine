#ifndef SHADER_PASS_H
#define SHADER_PASS_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/// <summary>
/// Class for the shader pass.
/// Responsible for storing the shader program ID, as well as the paths to the vertex, fragment and geometry shaders.
/// </summary>
class ShaderPass {

public:
	ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath, const char* geometryShaderPath);
	ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath);
	void bind() const;
	void unbind() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4(const std::string& name, glm::mat4 value) const;
	GLuint getID();

private:
	GLuint ID;
};

#endif