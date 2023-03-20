// GLSL_Test.cpp : Defines the entry point for the console application.
//

#include "gl_core_4_3.hpp"
#include <GLFW\glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLWindow.h"
#include "Shader.h"

using glm::mat4;
using glm::vec3;

const int WIDTH = 1000, HEIGHT = 1000;


int main()
{
	

	bool success;
	GLWindow window(WIDTH, HEIGHT, &success);
	if (!success)
		return 1;

	

	window.printVersion();

	//Setup vertices
	/*float positionData[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f };
	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f};*/
	float positionData2[] = {
		-0.7f, -0.7f, 0.0f,
		0.7f, -0.7f, 0.0f,
		-0.7f, 0.7f, 0.0f,
		-0.7f, 0.7f, 0.0f,
		0.7f, -0.7f, 0.0f,
		0.7f, 0.7f, 0.0f };
	/*float colorData2[] = {
		0.7f, 0.0f, 0.0f,
		0.0f, 0.7f, 0.0f,
		0.0f, 0.0f, 0.7f,
		0.0f, 0.0f, 0.7f,
		0.0f, 0.7f, 0.0f,
		0.5f, 0.5f, 0.5f };*/
	float texData[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f };
	//Create and populate buffer objects
	GLuint vboHandles[2];
	gl::GenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint texBufferHandle = vboHandles[1];

	//Populate the position buffer
	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, 18 * sizeof(float), positionData2, gl::STATIC_DRAW);

	//Populate the tex buffer
	gl::BindBuffer(gl::ARRAY_BUFFER, texBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, 18 * sizeof(float), texData, gl::STATIC_DRAW);

	// Create and set-up the vertex array object
	GLuint vaoHandle;
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);

	//Enable the vertex attribute arrays
	gl::EnableVertexAttribArray(0); //Vertex position
	gl::EnableVertexAttribArray(1); //Vertex color

	//Modified code
	gl::BindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
	gl::BindVertexBuffer(1, texBufferHandle, 0, sizeof(GLfloat) * 3);

	gl::VertexAttribFormat(0, 3, gl::FLOAT, gl::FALSE_, 0);
	gl::VertexAttribBinding(0, 0);
	gl::VertexAttribFormat(1, 3, gl::FLOAT, gl::FALSE_, 0);
	gl::VertexAttribBinding(1, 1);


	//Create and use shader
	Shader shader("uniform_block.vert", "uniform_block.frag");

	GLuint blockIndex = gl::GetUniformBlockIndex(shader.programHandle, "BlobSettings");
	GLint blockSize;
	gl::GetActiveUniformBlockiv(shader.programHandle, blockIndex, gl::UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	GLubyte* blockBuffer;
	blockBuffer = (GLubyte*)malloc(blockSize);
	const GLchar* names[] = { "InnerColor", "OuterColor", "RadiusInner", "RadiusOuter" };
	GLuint indices[4];
	gl::GetUniformIndices(shader.programHandle, 4, names, indices);
	GLint offset[4];
	gl::GetActiveUniformsiv(shader.programHandle, 4, indices, gl::UNIFORM_OFFSET, offset);

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
	GLfloat innerRadius = 0.25f, outerRadius = 0.45f;

	memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

	GLuint uboHandle;
	gl::GenBuffers(1, &uboHandle);
	gl::BindBuffer(gl::UNIFORM_BUFFER, uboHandle);
	gl::BufferData(gl::UNIFORM_BUFFER, blockSize, blockBuffer, gl::DYNAMIC_DRAW);
	gl::BindBufferBase(gl::UNIFORM_BUFFER, 0, uboHandle);

	shader.Use();

	//Print attributes
	shader.printActiveAttributes();
	shader.printActiveUniforms();

	//Draw

	//Clear buffers
	gl::ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	float angle = 0.0f;
	mat4 rotationMatrix = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));

	GLuint location = gl::GetUniformLocation(shader.programHandle, "RotationMatrix");
	if (location >= 0) {
		gl::UniformMatrix4fv(location, 1, gl::FALSE_, &rotationMatrix[0][0]);
	}

	gl::BindVertexArray(vaoHandle);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);

	glfwSwapBuffers(window.window);
	//Sleep(1000);

	std::getchar();
}

