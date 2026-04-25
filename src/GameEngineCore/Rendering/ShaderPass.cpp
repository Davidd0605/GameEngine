#include "ShaderPass.h"

ShaderPass::ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath, const char* geometryShaderPath) {

	bool defaultGeometry;
	if (geometryShaderPath == "") {
		defaultGeometry = true;
	}
	std::string vertexCode;
	std::string fragmentCode;
	std::string geomCode;

	std::fstream vShaderFile;
	std::fstream fShaderFile;
	std::fstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		//Open the files with fstream
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		gShaderFile.open(geometryShaderPath);
		//Create string stream to better parse strings
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		//Read char buffer into the string stream
		fShaderStream << fShaderFile.rdbuf();
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();

		//read the string in the string stream
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geomCode = gShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geomCode.c_str();

		//Vertex shader init
		GLuint vertexShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);

		int success;
		char infoLog[512];

		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::VERTEX SHADER COMPILATION ERROR:: \n" << infoLog;
		}

		//Fragment shader init
		GLuint fragmentShader;

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::FRAGMENT SHADER COMPILATION ERROR:: \n" << infoLog;
		}

		//Geometry shader init
		GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		glCompileShader(geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::GEOMETRY SHADER COMPILATION ERROR:: \n" << infoLog;
		}

		//Create shader program and bind it
		ID = glCreateProgram();

		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glAttachShader(ID, geometryShader);
		glLinkProgram(ID);

		//Free up memory
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER_PROGRAM::FILE COULD NOT BE READ";
	}
}
ShaderPass::ShaderPass(const char* fragmentShaderPath, const char* vertexShaderPath) {
	// Same behaviour as the three-shader constructor but without geometry shader handling.

	std::string vertexCode;
	std::string fragmentCode;

	std::fstream vShaderFile;
	std::fstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);

		std::stringstream vShaderStream, fShaderStream;

		// Read into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Convert to strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// Compile vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::VERTEX SHADER COMPILATION ERROR:: \n" << infoLog;
		}

		// Compile fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::FRAGMENT SHADER COMPILATION ERROR:: \n" << infoLog;
		}

		// Create program, attach shaders and link
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		// Delete shader objects after linking
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER_PROGRAM::FILE COULD NOT BE READ";
	}
}
GLuint ShaderPass::getID() {
	return ID;
}

void ShaderPass::bind() const{
	glUseProgram(ID);
}

void ShaderPass::unbind() const{
	glUseProgram(0);
}
void ShaderPass::setVec3(const std::string& name, const glm::vec3& value) const
{
	bind();
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderPass::setVec4(const std::string& name, const glm::vec4& value) const
{
	bind();
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void ShaderPass::setBool(const std::string& name, bool value) const
{
	bind();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderPass::setInt(const std::string& name, int value) const
{
	bind();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderPass::setFloat(const std::string& name, float value) const
{
	bind();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderPass::setMatrix4(const std::string& name, glm::mat4 value) const
{
	bind();	
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::vector<UniformInfo> ShaderPass::getMaterialUniforms() const {
	std::vector<UniformInfo> result;

	GLint uniformCount;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);

	for (int i = 0; i < uniformCount; i++) {
		char name[256];
		GLint size;
		GLenum type;
		glGetActiveUniform(ID, i, sizeof(name), nullptr, &size, &type, name);

		// only material uniforms
		if (name[0] != '_') continue;

		UniformInfo info;
		info.name = std::string(name);
		info.type = type;
		info.location = glGetUniformLocation(ID, name);
		result.push_back(info);
	}

	return result;
}