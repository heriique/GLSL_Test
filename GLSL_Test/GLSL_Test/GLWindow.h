#pragma once
#include "gl_core_4_3.hpp"
#include <GLFW\glfw3.h>
#include <cstdio>

class GLWindow
{
public:
	GLWindow(int width, int height, bool* success);
	~GLWindow();

	// Prints the version of OpenGl and GLSL, and the video card information
	void printVersion();

	GLFWwindow* window;
	int screenWidth, screenHeight;

	static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param);

};

