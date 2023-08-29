#pragma once

#include <string>

#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


class VirtualCube : public Window<GLFWwindow> {
public:
	VirtualCube(int width, int height, std::string name);
	virtual ~VirtualCube();

	virtual void use() override;
	virtual void unuse() override;

	int windowShouldClose() const { return glfwWindowShouldClose(this->m_window); }

private:
	void processInput();
};