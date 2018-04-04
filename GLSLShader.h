/*
 * GLSLShader.h
 *
 *  Created on: 19/02/2015
 *      Author: cesar
 */

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

//#include "gl_core_4_4.hpp"
#include "gl_core_4_5.h"
#include <map>
#include <string>
#include "Uniform.h"

enum ShaderType {
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER,
	TESS_CONTROL_SHADER,
	TESS_EVALUATION_SHADER,
	SHADER_TYPE_COUNT
};

class Uniform;

class GLSLShader {
public:
	GLSLShader();
	virtual ~GLSLShader();

	void LoadFromString(GLenum shader_type, const std::string& source);
	void LoadFromFile(GLenum shader_type, const std::string& filename);
	void CreateAndLinkProgram();
	void Use();
	void UnUse();
	Uniform getUniform(std::string uniform_name, ShaderType type);
	void DeleteShaderProgram();

private:

	GLuint _program;
	int _totalShaders;
	GLuint _shaders[SHADER_TYPE_COUNT];
};

#endif /* GLSLSHADER_H_ */
