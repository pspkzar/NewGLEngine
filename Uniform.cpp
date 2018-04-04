/*
 * Uniform.cpp
 *
 *  Created on: 21/02/2016
 *      Author: cesar
 */

#include "Uniform.h"

Uniform::Uniform() {
	// TODO Auto-generated constructor stub

}

Uniform::~Uniform() {
	// TODO Auto-generated destructor stub
}

void Uniform::Set(glm::vec3 v) {
	glProgramUniform3fv(program, location, 1, & v[0]);
}

void Uniform::Set(glm::vec4 v) {
	glProgramUniform4fv(program, location, 1, & v[0]);
}

void Uniform::Set(glm::mat4 m) {
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, & m[0][0]);
}

void Uniform::Set(float f) {
	glProgramUniform1f(program, location, f);
}

void Uniform::Set(int i) {
	glProgramUniform1i(program, location, i);
}

void Uniform::Set(glm::mat3 m) {
	glProgramUniformMatrix3fv(program, location, 1, GL_FALSE, & m[0][0]);
}

void Uniform::Set(unsigned int ui) {
	glProgramUniform1ui(program, location, ui);
}

