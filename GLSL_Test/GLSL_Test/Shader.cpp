#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "gl_core_4_3.hpp"
#include "Shader.h"

//Constructor generates shader
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//Ensure ifstream can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//Read files into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error reading shader file." << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	//Compile shaders
	GLint success;
	GLchar infoLog[512];

	//Create vertex shader
	vertexShaderHandle = gl::CreateShader(gl::VERTEX_SHADER);
	gl::ShaderSource(vertexShaderHandle, 1, &vShaderCode, NULL);
	gl::CompileShader(vertexShaderHandle);
	gl::GetShaderiv(vertexShaderHandle, gl::COMPILE_STATUS, &success);
	if (!success) {
		gl::GetShaderInfoLog(vertexShaderHandle, 512, NULL, infoLog);
		std::cout << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
	}

	//Create fragment shader
	fragmentShaderHandle = gl::CreateShader(gl::FRAGMENT_SHADER);
	gl::ShaderSource(fragmentShaderHandle, 1, &fShaderCode, NULL);
	gl::CompileShader(fragmentShaderHandle);
	gl::GetShaderiv(fragmentShaderHandle, gl::COMPILE_STATUS, &success);
	if (!success) {
		gl::GetShaderInfoLog(fragmentShaderHandle, 512, NULL, infoLog);
		std::cout << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
	}

	// Create shader program
	this->programHandle = gl::CreateProgram();

	gl::AttachShader(this->programHandle, vertexShaderHandle);
	gl::AttachShader(this->programHandle, fragmentShaderHandle);
	gl::LinkProgram(this->programHandle);
	gl::GetProgramiv(this->programHandle, gl::LINK_STATUS, &success);
	if (!success) {
		gl::GetProgramInfoLog(this->programHandle, 512, NULL, infoLog);
		std::cout << "Error: Shader program linking failed\n" << infoLog << std::endl;
	}
}

Shader::~Shader() {
	//Delete shaders
	gl::DeleteShader(vertexShaderHandle);
	gl::DeleteShader(fragmentShaderHandle);

	//Delete program
	gl::DeleteShader(programHandle);
}

void Shader::Use() {
	gl::UseProgram(this->programHandle);
}

void Shader::printActiveAttributes()
{
	GLint written, size, location, maxLength, nAttribs;
	GLenum type;
	GLchar * name;

	gl::GetProgramiv(programHandle, gl::ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	gl::GetProgramiv(programHandle, gl::ACTIVE_ATTRIBUTES, &nAttribs);

	name = (GLchar *)malloc(maxLength);

	printf("\n<<<Active attributes>>>\n");
	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++) {
		gl::GetActiveAttrib(programHandle, i, maxLength, &written, &size, &type, name);
		location = gl::GetAttribLocation(programHandle, name);
		printf(" %-5d | %s\n", location, name);
	}

	free(name);
}

void Shader::printActiveUniforms() {

	GLint nUniforms, size, location, maxLen;
	GLchar * name;
	GLsizei written;
	GLenum type;

	gl::GetProgramiv(programHandle, gl::ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	gl::GetProgramiv(programHandle, gl::ACTIVE_UNIFORMS, &nUniforms);

	name = (GLchar *)malloc(maxLen);

	printf("\n<<<Active uniforms>>>\n");
	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nUniforms; ++i) {
		gl::GetActiveUniform(programHandle, i, maxLen, &written, &size, &type, name);
		location = gl::GetUniformLocation(programHandle, name);
		printf(" %-8d | %s\n", location, name);
	}

	free(name);
}
