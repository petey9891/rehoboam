#ifdef _WIN32  // If on Windows

#include <iostream>
#include <window/virtual_cube.h>

VirtualCube::VirtualCube(int width, int height, std::string name) : Window(width, height) {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create a windowed mode window and its OpenGL context
	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (m_window == nullptr) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);

	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD.\n";
	}
}

VirtualCube::~VirtualCube() {
	glfwTerminate();
}

void VirtualCube::use() {
	float currFrame = (float) glfwGetTime();
	m_deltaTime = currFrame - this->m_prevFrame;
	m_prevFrame = currFrame;

	this->processInput();
}

void VirtualCube::unuse() {
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void VirtualCube::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);
	if (glfwGetKey(m_window, GLFW_KEY_F1) == GLFW_PRESS)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(m_window, GLFW_KEY_F2) == GLFW_PRESS)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	if (glfwGetKey(m_window, GLFW_KEY_F3) == GLFW_PRESS)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

#endif // _WIN32