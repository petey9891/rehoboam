#include <applications/cube_calibration.h>

CubeCalibration::CubeCalibration() {
	this->m_shader = std::make_unique<Shader>("shaders/main.vert.glsl", "shaders/rehoboam/rehoboam.frag.glsl");

	this->m_geometry = std::make_unique<ShaderGeometry>(true, false);
}

CubeCalibration::~CubeCalibration() {
	glDeleteProgram(this->m_shader->id);
}

void CubeCalibration::activate() {
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glDisable(GL_CULL_FACE); // can I remove this?

	glm::mat4 MVP = this->m_geometry->initializeCamera();
	 
	// frame counter is meant to overflow to reset animations
	this->m_shader->use();
	this->m_shader->setMat4("MVP", MVP); // Send our transformation to the currently bound shader, in the "MVP" uniform
	this->m_shader->setVec3("iResolution", glm::vec3(1.0f));
}

void CubeCalibration::run() {
	this->m_frameCounter++;

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->m_shader->use();
	this->m_shader->setFloat("iTime", glfwGetTime());
	this->m_shader->setInt("iFrame", this->m_frameCounter);

	this->m_geometry->draw();
}