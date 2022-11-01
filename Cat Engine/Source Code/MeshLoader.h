#pragma once

#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

class GameObject;
class TransformComponent;

class MeshLoader
{
public:
	static MeshLoader* GetInstance();
	static void ReleaseInstance();
	~MeshLoader();

	void LoadingModel(std::string& path);
	void LoadingTransform(aiNode* node, GameObject* obj);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* object);
	MeshComponent* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object);
	MaterialComponent* LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

private:
	MeshLoader() {}
	MeshLoader(const MeshLoader&) {}

	static MeshLoader* instance;

	std::string directory;
};