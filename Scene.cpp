#include "Scene.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <iostream>

using namespace std;
using namespace glm;

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::LoadSceneFile(std::string& scenefile) {
	//import scene with assimp
	const aiScene * assimp_scene = aiImportFile(scenefile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FixInfacingNormals | aiProcess_PreTransformVertices);
	//find working directory
	string working_directory;
	unsigned int slash_i=scenefile.find_last_of("\\/");
	if(slash_i==scenefile.npos){
		working_directory="";
	}
	else{
		working_directory=scenefile.substr(0, slash_i+1);
	}
	cerr << "Working directory: " <<  working_directory << endl;
	//load materials
	for(unsigned int i=0; i<assimp_scene->mNumMaterials; i++){
		aiMaterial *assimp_material = assimp_scene->mMaterials[i];

		aiString assimp_matName;
		aiGetMaterialString(assimp_material, AI_MATKEY_NAME, &assimp_matName);
		string matName = string(assimp_matName.C_Str());

		cerr << "Loading Material " << matName << endl;

		aiColor4D diffuse, specular;
		float Ns;
		aiGetMaterialColor(assimp_material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		aiGetMaterialColor(assimp_material, AI_MATKEY_COLOR_SPECULAR, &specular);
		aiGetMaterialFloat(assimp_material, AI_MATKEY_SHININESS, &Ns);

		vec4 Kd(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		vec4 Ks(specular.r, specular.g, specular.b, specular.a);

		aiString assimp_diffuseTexPath;
		Texture MapKd;
		if (assimp_material->GetTexture(aiTextureType_DIFFUSE, 0, &assimp_diffuseTexPath) == AI_SUCCESS){
			string diffuseTexPath(assimp_diffuseTexPath.C_Str());
			MapKd.LoadFromFile(working_directory + diffuseTexPath, TEX_FORMAT_RGBA, WRAP_MODE_REPEAT);
		}
		
		aiString assimp_specTexPath;
		Texture MapKs;
		if (assimp_material->GetTexture(aiTextureType_SPECULAR, 0, &assimp_specTexPath) == AI_SUCCESS){
			string specTexPath(assimp_specTexPath.C_Str());
			MapKs.LoadFromFile(working_directory + specTexPath, TEX_FORMAT_RGBA, WRAP_MODE_REPEAT);
		}

		aiString assimp_bumpTexPath;
		Texture MapBump;
		if (assimp_material->GetTexture(aiTextureType_HEIGHT, 0, &assimp_bumpTexPath) == AI_SUCCESS){
			string bumpTexPath(assimp_bumpTexPath.C_Str());
			MapBump.LoadFromFile(working_directory + bumpTexPath, TEX_FORMAT_RG, WRAP_MODE_REPEAT);
		}
		
		Material mat;
		mat.Init(Kd, Ks, Ns, MapKd, MapKs, MapBump);
		
		materials[matName] = mat;

		cerr << "Loaded Material " << matName << endl;
	}

	for (int i = 0; i < assimp_scene->mNumMeshes; i++){
		aiMesh *assimp_mesh = assimp_scene->mMeshes[i];
		
		cerr << "Loading Mesh " << i << endl;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		
		for(int v=0; v<assimp_mesh->mNumVertices; v++){
			Vertex vertex;
			vertex.position=glm::vec4(assimp_mesh->mVertices[v].x,assimp_mesh->mVertices[v].y,assimp_mesh->mVertices[v].z,1.f);
			vertex.normal=glm::vec4(assimp_mesh->mNormals[v].x,assimp_mesh->mNormals[v].y,assimp_mesh->mNormals[v].z,0.f);
			if(assimp_mesh->HasTangentsAndBitangents()){
				vertex.tangent=glm::vec4(assimp_mesh->mTangents[v].x,assimp_mesh->mTangents[v].y,assimp_mesh->mTangents[v].z,0.f);
				vertex.bitangent=glm::vec4(assimp_mesh->mBitangents[v].x,assimp_mesh->mBitangents[v].y,assimp_mesh->mBitangents[v].z,0.f);
			}
			if(assimp_mesh->HasTextureCoords(0)){
				vertex.texCoord=glm::vec2(assimp_mesh->mTextureCoords[0][v].x,assimp_mesh->mTextureCoords[0][v].y);
			}
			vertices.push_back(vertex);
		}

		for(int f=0; f<assimp_mesh->mNumFaces; f++){
			indices.push_back(assimp_mesh->mFaces[f].mIndices[0]);
			indices.push_back(assimp_mesh->mFaces[f].mIndices[1]);
			indices.push_back(assimp_mesh->mFaces[f].mIndices[2]);
		}

		aiString assimp_mat_name;
		aiGetMaterialString(assimp_scene->mMaterials[assimp_mesh->mMaterialIndex], AI_MATKEY_NAME, &assimp_mat_name);
		string mat_name = string(assimp_mat_name.C_Str());
		Mesh mesh;
		mesh.LoadMash(vertices, indices, materials[mat_name]);
		
		meshes.push_back(mesh);

		cerr << "Loaded Mesh " << i << endl;
	}
}

void Scene::Draw(GLSLShader &shader){
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].DrawMesh(shader);
	}
}
