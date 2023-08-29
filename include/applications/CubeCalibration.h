#pragma once
#include <memory>

class IWindow;
class Shader;
class Cube;

class CubeCalibration {
public:
	CubeCalibration(int win_width, int win_height);
	~CubeCalibration();

	void run();

	std::unique_ptr<IWindow> m_window;

	std::unique_ptr <Shader> m_shader;

	std::unique_ptr <Cube> m_cube;
};
