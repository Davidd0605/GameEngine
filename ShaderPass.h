#ifndef SHADER_PASS_H
#define SHADER_PASS_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
class ShaderPass {

public:
	ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath, const char* geometryShaderPath);
	ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath);
	void bind();
	void unbind();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	GLuint getID();

private:
	GLuint ID;
};

#endif