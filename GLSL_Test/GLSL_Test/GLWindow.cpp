#include "GLWindow.h"
#include "gl_core_4_3.hpp"
#include <GLFW\glfw3.h>
#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>



GLWindow::GLWindow(int width, int height, bool* success)
{
	*success = false;
	glfwInit();

	//Create a window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, gl::TRUE_);

	window = glfwCreateWindow(width, height, "Heidu", nullptr, nullptr);

	

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		printf("Failed to create GLFW Window.\n");
		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(window);

	//Load gl functions with GLLoadGen
	//printf("Loading OpenGL functions...\n");
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	if (!didLoad) {
		printf("Failed to load functions.\n");
		//Cleanup
		glfwTerminate();
		return;
	}
	//printf("Number of functions that failed to load: %i.\n", didLoad.GetNumMissing());

	//Setup debug callback function
	gl::DebugMessageCallback(debugCallback, NULL);
	gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, NULL, gl::TRUE_);

	// Set viewport
	gl::Viewport(0, 0, screenWidth, screenHeight);

	gl::Enable(gl::DEPTH_TEST);

	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);

	*success = true;
}

GLWindow::~GLWindow()
{
	glfwTerminate();
}


// Prints the version of OpenGl and GLSL, and the video card information
void GLWindow::printVersion()
{
	const GLubyte *renderer = gl::GetString(gl::RENDERER);
	const GLubyte* vendor = gl::GetString(gl::VENDOR);
	const GLubyte* version = gl::GetString(gl::VERSION);
	const GLubyte* glslVersion = gl::GetString(gl::SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	gl::GetIntegerv(gl::MAJOR_VERSION, &major);
	gl::GetIntegerv(gl::MINOR_VERSION, &minor);

	printf("GL Vendor   : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version  : %d.%d\n", major, minor);
	printf("GLSL Version: %s\n", glslVersion);
}

void APIENTRY GLWindow::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param) {
	char *sourceStr, *typeStr, *severityStr;
	switch (source) {
	case gl::DEBUG_SOURCE_API: sourceStr = "API"; break;
	case gl::DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Window System"; break;
	case gl::DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader compiler"; break;
	case gl::DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Third Party"; break;
	case gl::DEBUG_SOURCE_APPLICATION: sourceStr = "Application"; break;
	case gl::DEBUG_SOURCE_OTHER: sourceStr = "Other source"; break;
	default: "Unknown source";
	}

	switch (type) {
	case gl::DEBUG_TYPE_ERROR: typeStr = "Error"; break;
	case gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated behavior"; break;
	case gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined behavior"; break;
	case gl::DEBUG_TYPE_PORTABILITY: typeStr = "Unportability"; break;
	case gl::DEBUG_TYPE_PERFORMANCE: typeStr = "Performance issue"; break;
	case gl::DEBUG_TYPE_MARKER: typeStr = "Command stream annotation"; break;
	case gl::DEBUG_TYPE_PUSH_GROUP: typeStr = "Group pushing"; break;
	case gl::DEBUG_TYPE_POP_GROUP: typeStr = "Group popping"; break;
	case gl::DEBUG_TYPE_OTHER: typeStr = "Other message type"; break;
	default: typeStr = "Unknown message type";
	}

	switch (severity) {
	case gl::DEBUG_SEVERITY_HIGH: severityStr = "Severity: high"; break;
	case gl::DEBUG_SEVERITY_MEDIUM: severityStr = "Severity: medium"; break;
	case gl::DEBUG_SEVERITY_LOW: severityStr = "Severity: low"; break;
	case gl::DEBUG_SEVERITY_NOTIFICATION: severityStr = "Severity: notification"; break;
	default: severityStr = "Unknown severity";
	}

	printf("%s:%s[%s](%d): %s\n", sourceStr, typeStr, severityStr, id, message);
}

