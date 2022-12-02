#pragma once

#include <string>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"

#include "JsonParse.h"

class GameObject;

namespace ModelImporter
{
	void ImportModel(std::string& path);
	void SaveModel(std::string& path, JsonParsing& json);
	void LoadModel(std::string& path);

	void ProcessNode(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json);
	void CreatingModel(JsonParsing& json, JSON_Array* array, GameObject* go);
}