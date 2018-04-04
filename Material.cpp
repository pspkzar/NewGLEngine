#include "Material.h"
#include <iostream>

using namespace std;
using namespace glm;

Material::Material() {

}

Material::~Material() {

}

void Material::Init(glm::vec4 Kd, glm::vec4 Ks, float Ns, Texture MapKd,
		Texture MapKs, Texture MapBump) {
	_Kd = Kd;
	_Ks = Ks;
	_Ns = Ns;
	_MapKd = MapKd;
	_MapKs = MapKs;
	_MapBump = MapBump;
}

void Material::Set(GLSLShader& shader) {

//	glUniform4fv(shader("Kd"), 1, &_Kd[0]);
//	glUniform4fv(shader("Ks"), 1, &_Ks[0]);
//	glUniform1f(shader("Ns"), _Ns);
//

	shader.getUniform("Kd",FRAGMENT_SHADER).Set(_Kd);
	shader.getUniform("Ks",FRAGMENT_SHADER).Set(_Ks);
	shader.getUniform("Ns",FRAGMENT_SHADER).Set(_Ns);

	glActiveTexture(GL_TEXTURE0);
	_MapKd.Bind();
//	shader["MapKd"].Set(0);
	//shader.getUniform("MapKd",FRAGMENT_SHADER).Set(_MapKd.getTexture());
//
	glActiveTexture(GL_TEXTURE1);
	_MapKs.Bind();
//	shader["MapKs"].Set(1);
	//shader.getUniform("MapKs",FRAGMENT_SHADER).Set(_MapKs.getTexture());

	glActiveTexture(GL_TEXTURE2);
	_MapBump.Bind();
//	shader["MapBump"].Set(2);
	//shader.getUniform("MapBump",FRAGMENT_SHADER).Set(_MapBump.getTexture());
}
