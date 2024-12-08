#pragma once

#include <vector>

#include "opengl_includes.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
};

class ShaderGeometry {
public:
	ShaderGeometry(bool isPerspective, bool isProduction);
	~ShaderGeometry();

	glm::mat4 initializeCamera() const;

	void draw() const;

private:
	std::vector<glm::vec2> calculatePerspectiveUVs(std::vector<glm::vec3> verticies, glm::vec3 angles) const;

protected:
	GLuint m_vbo = 0, m_vao = 0;
	
	std::vector<Vertex> m_vertices;

	bool m_isPerspective, m_isProduction;
};