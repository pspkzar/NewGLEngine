/*
 * Uniform.h
 *
 *  Created on: 21/02/2016
 *      Author: cesar
 */

#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "gl_core_4_5.h"
#include <glm/glm.hpp>
#include "GLSLShader.h"

class Uniform {
	friend class GLSLShader;
public:
	Uniform();
	virtual ~Uniform();

	void Set(glm::vec3 v);
	void Set(glm::vec4 v);
	void Set(glm::mat3 m);
	void Set(glm::mat4 m);
	void Set(float f);
	void Set(int i);
	void Set(unsigned int ui);
private:
	GLuint program, location;
};

#endif /* UNIFORM_H_ */
