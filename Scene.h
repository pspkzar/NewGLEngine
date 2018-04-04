/*
 * Scene.h
 *
 *  Created on: 20/02/2015
 *      Author: cesar
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include <map>
#include <vector>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "GLSLShader.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	void LoadSceneFile(std::string &scenefile);

	void Draw(GLSLShader &shader);

private:
	std::vector<Mesh> meshes;
	std::map<std::string, Material> materials;

};

#endif /* SCENE_H_ */
