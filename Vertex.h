/*
 * Vertex.h
 *
 *  Created on: 09/03/2016
 *      Author: cesar
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/glm.hpp>

typedef struct sVertex {

	sVertex(){
		position=glm::vec4(0,0,0,1);
		normal=glm::vec4(0);
		tangent=glm::vec4(0);
		bitangent=glm::vec4(0);
		texCoord=glm::vec2(0);
	}

	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec4 bitangent;
	glm::vec2 texCoord;
} Vertex;

#endif /* VERTEX_H_ */
