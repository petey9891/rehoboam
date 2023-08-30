#pragma once
#include "shader_application.h"

class CubeCalibration : public ShaderApplication {
public:
	CubeCalibration();
	~CubeCalibration();

	void activate() override;
	void run() override;
};
