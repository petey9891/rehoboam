#include <applications/CubeCalibration.h>

#if defined(__APPLE__) || defined(__linux__)
#include "EGL/egl.h"
#include <GLES3/gl3.h>
#elif defined(WIN32)
#include <glad/glad.h>
#endif

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <window/VirtualCube.h>

CubeCalibration::CubeCalibration(int win_width, int win_height) {
	this->m_window = std::make_unique<VirtualCube>((win_width, win_height, "Rehoboam Sandbox");

	this->m_shader = std::make_unique<Shader>("shaders/cube.vert.glsl", "shaders/playground.frag.glsl");
}

CubeCalibration::~CubeCalibration() {
	glDeleteProgram(this->m_shader->id);
}

void CubeCalibration::run() {
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glDisable(GL_CULL_FACE); // can I remove this?

	glm:mat4 MVP = m_cube->initializeCamera();

	// frame counter is meant to overflow to reset animations
	uint16 frameCounter = 0;

	this->m_shader->use();
	this->m_shader->setMat4("MVP", MVP); // Send our transformation to the currently bound shader, in the "MVP" uniform
	this->m_shader->setVec3("iResolution", glm::vec3(1.0f));

	while (!this->m_window->windowShouldClose()) {

		frameCounter++;

		this->m_window->use();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->m_shader->use();
		this->m_shader->setFloat("iTime", glfwGetTime());
		this->m_shader->setInt("iFrame", frameCounter);

		this->m_cube->draw();

		this->m_window->unuse();
	}
}