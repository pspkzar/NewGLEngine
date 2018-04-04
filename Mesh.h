#ifndef MESH_H_
#define MESH_H_

//#include "gl_core_4_4.hpp"
#define GLM_FORCE_RADIANS
#include "gl_core_4_5.h"
#include <glm/glm.hpp>
#include <vector>
#include "Material.h"
#include "Vertex.h"

class Mesh {
public:
	Mesh();
	virtual ~Mesh();

	void LoadMash(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Material &mat);
	void DrawMesh(GLSLShader &shader);
	void DeleteMesh();

private:
	enum Attributes{
		POSITION,
		NORMAL,
		TANGENT,
		BITANGENT,
		UV,
		ATTRIBUTE_COUNT
	};

	enum Buffers{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		BUFFER_COUNT
	};

	GLuint vao;
	GLuint buffers[BUFFER_COUNT];
	Material m;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

#endif /* MESH_H_ */
