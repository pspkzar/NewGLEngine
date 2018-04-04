/*
 * Material.h
 *
 *  Created on: 20/02/2015
 *      Author: cesar
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "GLSLShader.h"
#include "Texture.h"

class Material {
public:
	Material();
	virtual ~Material();

	void Init(glm::vec4 Kd, glm::vec4 Ks, float Ns, Texture MapKd, Texture MapKs, Texture MapBump);
	void Set(GLSLShader& shader);

private:
	glm::vec4 _Kd, _Ks;
	float _Ns;
	Texture _MapKd, _MapKs, _MapBump;
};

#endif /* MATERIAL_H_ */
