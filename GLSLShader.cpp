/*
 * GLSLShader.cpp
 *
 *  Created on: 19/02/2015
 *      Author: cesar
 */

#include "GLSLShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

GLSLShader::GLSLShader() {
	_program=0;
	_totalShaders=0;
	for(int i=0; i<SHADER_TYPE_COUNT; i++){
		_shaders[i]=0;
	}
}

GLSLShader::~GLSLShader() {

}

void GLSLShader::LoadFromString(GLenum shader_type, const std::string& source) {
	//GLuint shader = glCreateShader(shader_type);
	const char *src = source.c_str();
	//glShaderSource(shader, 1, &src, NULL);
	//glCompileShader(shader);
	GLuint shader = glCreateShaderProgramv(shader_type, 1, &src);

	int length;
	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
	char *infolog = new char[length];
	glGetProgramInfoLog(shader, length, &length, infolog);
	std::cerr << infolog << std::endl;
	delete [] infolog;


	ShaderType s=SHADER_TYPE_COUNT;
	switch(shader_type){
	case GL_VERTEX_SHADER:
		s = VERTEX_SHADER; break;
	case GL_FRAGMENT_SHADER:
		s = FRAGMENT_SHADER; break;
	case GL_GEOMETRY_SHADER:
		s = GEOMETRY_SHADER; break;
	case GL_TESS_CONTROL_SHADER:
		s = TESS_CONTROL_SHADER; break;
	case GL_TESS_EVALUATION_SHADER:
		s = TESS_EVALUATION_SHADER; break;
	}
	_shaders[s]=shader;
	_totalShaders++;
}

void GLSLShader::LoadFromFile(GLenum shader_type, const std::string& filename) {
	ifstream file(filename.c_str());
	stringstream sstr;
	sstr << file.rdbuf();
	string source = sstr.str();
	LoadFromString(shader_type, source);
}

void GLSLShader::CreateAndLinkProgram() {
	GLuint programStages[6];
	programStages[VERTEX_SHADER]=GL_VERTEX_SHADER_BIT;
	programStages[FRAGMENT_SHADER]=GL_FRAGMENT_SHADER_BIT;
	programStages[TESS_CONTROL_SHADER]=GL_TESS_CONTROL_SHADER_BIT;
	programStages[TESS_EVALUATION_SHADER]=GL_TESS_EVALUATION_SHADER_BIT;
	programStages[GEOMETRY_SHADER]=GL_GEOMETRY_SHADER_BIT;

	glCreateProgramPipelines(1, &_program);
	for(int i=0; i<SHADER_TYPE_COUNT; i++){
		if(_shaders[i]!=0){
			//glAttachShader(_program, _shaders[i]);
			glUseProgramStages(_program, programStages[i], _shaders[i]);
		}
	}
}

void GLSLShader::Use() {
	glBindProgramPipeline(_program);
}

void GLSLShader::UnUse() {
	glBindProgramPipeline(0);
}

Uniform GLSLShader::getUniform(std::string uniform_name, ShaderType type) {
	GLuint uniform_location = glGetUniformLocation(_shaders[type], uniform_name.c_str());
	Uniform u;
	u.location=uniform_location;
	u.program=_shaders[type];
	return u;
}

void GLSLShader::DeleteShaderProgram() {
	glDeleteProgramPipelines(1,&_program);
}
