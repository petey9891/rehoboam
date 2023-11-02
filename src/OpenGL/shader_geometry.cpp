#include <iostream>
#include <opengl/shader_geometry.h>

ShaderGeometry::ShaderGeometry(bool isPerspective, bool isProduction) : m_isPerspective(isPerspective), m_isProduction(isProduction) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// time for some ugly code....
	if (this->m_isProduction) {
		std::vector<glm::vec3> positions = {
			// Left face
			// x, y, z
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),

			// Right face
			// x, y, z
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),

			glm::vec3(2.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),

			// Top face
			// x, y, z
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 1.0f, 0.0f),
			glm::vec3(2.0f, 0.0f, 0.0f),

			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(3.0f, 1.0f, 0.0f),
			glm::vec3(2.0f, 1.0f, 0.0f)
		};

		std::vector<glm::vec2> uvs = this->calculatePerspectiveUVs(positions, glm::vec3(30.0f, -45.0f, 0.0f));

		// I think this is what it needs....
		//glm::vec2 uvs[] = {
		//	// Left face
		//	glm::vec2(0.0f, 0.25f),
		//	glm::vec2(0.5f, 0.5f),
		//	glm::vec2(0.0f, 0.75f),

		//	glm::vec2(0.0f, 0.25f),
		//	glm::vec2(0.5f, 0.0f),
		//	glm::vec2(0.5f, 0.5f),

		//	// Right face
		//	glm::vec2(0.5f, 0.0f),
		//	glm::vec2(1.0f, 0.25f),
		//	glm::vec2(0.5f, 0.5f),

		//	glm::vec2(1.0f, 0.25f),
		//	glm::vec2(1.0f, 0.75f),
		//	glm::vec2(0.5f, 0.5f),

		//	// Top face
		//	glm::vec2(0.5f, 0.5f),
		//	glm::vec2(0.5f, 1.0f),
		//	glm::vec2(0.0f, 0.75f),

		//	glm::vec2(0.5f, 0.5f),
		//	glm::vec2(1.0f, 0.75f),
		//	glm::vec2(0.5f, 1.0f),
		//};

		for (int i = 0; i < positions.size(); i++) {
			m_vertices.push_back({ positions[i], uvs[i] });
		}
	}
	else if (this->m_isPerspective) {
		std::vector<glm::vec3> positions = {
			// Left face
			// x, y, z
			glm::vec3(-1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),

			glm::vec3(-1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),

			// Right face
			// x, y, z
			glm::vec3(1.0f, -1.0f, 1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),

			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),

			// Top face
			// x, y, z
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),

			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f)
		};

		std::vector<glm::vec2> uvs = this->calculatePerspectiveUVs(positions, glm::vec3(30.0f, -45.0f, 0.0f));

		for (int i = 0; i < positions.size(); i++) {
			m_vertices.push_back({ positions[i], uvs[i] });
		}
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ShaderGeometry::~ShaderGeometry() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
};

glm::mat4 ShaderGeometry::initializeCamera() const {

	if (this->m_isProduction) {
		// Projection matrix : 45� Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
		float aspectRatio = 1.0f / 1.0f;
		glm::mat4 Projection = glm::ortho(0.0f, 3.0f * aspectRatio, 0.0f, 1.0f, -1.0f, 1.0f);
		// Camera 
		float cameraDistance = 0.1f;
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, cameraDistance), // Camera is at (4,3,-3), in World Space
			glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
			glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View; // Remember, matrix multiplication is the other way around

		return MVP;
	}
	else {
		// Projection matrix : 45� Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 100.0f);
		// Camera 
		float cameraDistance = 10.0f;
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, cameraDistance), // Camera is at (4,3,-3), in World Space
			glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
			glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Model matrix : an identity matrix (model will be at the origin)
		float angleX = glm::radians(30.0f);
		float angleY = glm::radians(-45.0f);
		float angleZ = glm::radians(0.0f);

		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::rotate(Model, angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // rotate around y-axis
		Model = glm::rotate(Model, angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y-axis
		Model = glm::rotate(Model, angleZ, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around z-axis

		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

		return MVP;
	}
}

void ShaderGeometry::draw() const {
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);
}


std::vector<glm::vec2> ShaderGeometry::calculatePerspectiveUVs(std::vector<glm::vec3> verticies, glm::vec3 angles) const {
	// Define rotation angles in radians
	float angleX = glm::radians(angles.x); // rotation around X-axis
	float angleY = glm::radians(angles.y); // rotation around Y-axis
	float angleZ = glm::radians(angles.z); // rotation around Z-axis

	// Define rotation matrices
	glm::mat4 rotx = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1, 0, 0));
	glm::mat4 roty = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0, 1, 0));
	glm::mat4 rotz = glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0, 0, 1));

	// Combine the rotation matrices
	glm::mat4 rotMatrix = rotx * roty * rotz;

	// Apply the rotation matrix to the vertices
	std::vector<glm::vec2> rotatedVertices(18);
	for (size_t i = 0; i < verticies.size(); ++i) {
		glm::vec4 rotatedVertex = rotMatrix * glm::vec4(verticies.at(i), 1.0f);
		rotatedVertices[i] = glm::vec2(rotatedVertex.x, rotatedVertex.y);
	}

	// Calculate width and height of rotated coordinates
	int MAX_HEIGHT_INDEX = 13;
	int MIN_HEIGHT_INDEX = 4;

	int MAX_WIDTH_INDEX = 10;
	int MIN_WIDTH_INDEX = 2;

	float width = rotatedVertices[MAX_WIDTH_INDEX].x - rotatedVertices[MIN_WIDTH_INDEX].x;
	float height = rotatedVertices[MAX_HEIGHT_INDEX].y - rotatedVertices[MIN_HEIGHT_INDEX].y;

	float scaling = 1.0f / std::max(width, height);

	// Scale the vertices
	for (glm::vec2& vertex : rotatedVertices) {
		vertex *= scaling;
	}

	// Translate to correct origin 
	for (glm::vec2& vertex : rotatedVertices) {
		vertex.x += 0.5f;
		vertex.y += 0.5f;
	}

	return rotatedVertices;
}