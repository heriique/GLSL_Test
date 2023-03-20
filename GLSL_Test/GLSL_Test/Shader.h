#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "gl_core_4_3.hpp"

class Shader {
public:
	GLuint programHandle;
	GLuint vertexShaderHandle, fragmentShaderHandle;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void Use();
	void printActiveAttributes();
	void printActiveUniforms();
};