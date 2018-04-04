#include "Mesh.h"

#include <iostream>

using namespace std;

Mesh::Mesh() {
	vao = 0;
	buffers[VERTEX_BUFFER] = 0;
	buffers[INDEX_BUFFER] = 0;
}

Mesh::~Mesh() {

}

void Mesh::LoadMash(std::vector<Vertex> &vertices,
		std::vector<unsigned int> &indices, Material &mat) {
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(BUFFER_COUNT, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER]);
	//glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
	//		this->vertices.data(), GL_STATIC_DRAW);

	glBufferStorage(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
			this->vertices.data(), 0);

	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, position));

	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, normal));

	glEnableVertexAttribArray(TANGENT);
	glVertexAttribPointer(TANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, tangent));

	glEnableVertexAttribArray(BITANGENT);
	glVertexAttribPointer(BITANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, bitangent));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*) offsetof(Vertex, texCoord));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int),
	//		this->indices.data(), GL_STATIC_DRAW);

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER,
			this->indices.size() * sizeof(unsigned int), this->indices.data(),
			0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m = mat;
}

void Mesh::DrawMesh(GLSLShader &shader) {
	m.Set(shader);
	shader.Use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DeleteMesh() {
	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(1, &vao);
}

