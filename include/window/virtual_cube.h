#pragma once

#ifdef _WIN32  // If on Windows

#include <string>

#include "window.h"
#include <opengl/opengl_includes.h>

class VirtualCube : public Window {
public:
	VirtualCube(int width, int height, std::string name);
	virtual ~VirtualCube();

	virtual void use() override;
	virtual void unuse() override;

	int windowShouldClose() const override { return glfwWindowShouldClose(this->m_window); }

private:
	void processInput();

	GLFWwindow* m_window;
};

#endif // _WIN32