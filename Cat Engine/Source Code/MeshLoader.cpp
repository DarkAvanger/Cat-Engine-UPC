#include "MeshLoader.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include "FileSystem.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "MathGeoLib/src/MathGeoLib.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "Profiling.h"

MeshLoader* MeshLoader::instance = nullptr;

MeshLoader* MeshLoader::GetInstance()
{
	if (instance == nullptr) instance = new MeshLoader();

	return instance;
}

void MeshLoader::ReleaseInstance()
{
	RELEASE(instance);
}

MeshLoader::~MeshLoader()
{
}

void MeshLoader::ImportModel(std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("ERROR ASSIMP %s", import.GetErrorString());
		return;
	}
	directory = path.substr(5, path.find_last_of('\\'));

	std::string p = path.substr(0, path.find_last_of('.'));
	p = p.substr(path.find_last_of('\\') + 1, p.size());
	p = p.substr(path.find_last_of('/') + 1, p.size());

	JsonParsing json = JsonParsing();
	JsonParsing child = json.SetChild(json.GetRootValue(), "Model");
	child.SetNewJsonString(child.ValueToObject(child.GetRootValue()), "Name", p.c_str());
	std::string root = "Childs" + p;
	JSON_Array* array = child.SetNewJsonArray(child.GetRootValue(), root.c_str());

	ProcessNode2(scene->mRootNode, scene, child, array);

	char* buffer = nullptr;
	size_t size = json.Save(&buffer);

	std::string fileName = MODELS_FOLDER + p + ".rgmodel";

	app->fs->Save(fileName.c_str(), buffer, size);
}

void MeshLoader::LoadingModel(std::string& path)
{
	char* buffer = nullptr;

	path = path.substr(0, path.find_last_of("."));
	path = path.substr(path.find_last_of("/") + 1, path.length());
	path = MODELS_FOLDER + path + ".rgmodel";

	app->fs->Load(path.c_str(), &buffer);

	if (buffer != nullptr)
	{
		JsonParsing json = JsonParsing(buffer);

		json = json.GetChild(json.GetRootValue(), "Model");

		GameObject* go = new GameObject();
		std::string name = json.GetJsonString("Name");
		go->SetName(name.c_str());

		name = "Childs" + name;
		CreatingModel(json, json.GetJsonArray(json.ValueToObject(json.GetRootValue()), name.c_str()), app->scene->GetRoot());
	}
}

void MeshLoader::CreatingModel(JsonParsing& json, JSON_Array* array, GameObject* go)
{
	size_t size = json.GetJsonArrayCount(array);
	for (int i = 0; i < size; ++i)
	{
		GameObject* newGo = new GameObject();
		newGo->CreateComponent(ComponentType::TRANSFORM);
		newGo->SetParent(go);
		go->AddChild(newGo);

		JsonParsing parsing = json.GetJsonArrayValue(array, i);
		std::string name = parsing.GetJsonString("Name");

		newGo->SetName(name.c_str());
		JSON_Array* arr = parsing.GetJsonArray(parsing.ValueToObject(parsing.GetRootValue()), "Components");
		size_t s = parsing.GetJsonArrayCount(arr);

		for (int j = 0; j < s; ++j)
		{
			JsonParsing components = parsing.GetJsonArrayValue(arr, j);
			MeshComponent* mesh = (MeshComponent*)newGo->CreateComponent(ComponentType::MESH_RENDERER);
			LoadMesh(components.GetJsonString("Mesh Path"), mesh);
			MaterialComponent* material = (MaterialComponent*)newGo->CreateComponent(ComponentType::MATERIAL);
			TextureLoader::GetInstance()->LoadTexture(std::string(components.GetJsonString("Texture Path")), material);
		}

		name = "Childs" + name;
		CreatingModel(parsing, parsing.GetJsonArray(parsing.ValueToObject(parsing.GetRootValue()), name.c_str()), newGo);
	}
}

void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj)
{
	LoadingTransform(node, obj);
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		obj->CreateComponent(ComponentType::MESH_RENDERER);
		MeshComponent* component = obj->GetComponent<MeshComponent>();
		LoadMesh(mesh->mName.C_Str(), component);
		aiString str;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		obj->CreateComponent(ComponentType::MATERIAL);
		MaterialComponent* material = obj->GetComponent<MaterialComponent>();
		TextureLoader::GetInstance()->LoadTexture(std::string(str.C_Str()), material);
		obj->SetName(node->mName.C_Str());
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		if (node->mChildren[i]->mNumMeshes > 0)
		{
			GameObject* object = app->scene->CreateGameObject(obj);
			ProcessNode(node->mChildren[i], scene, object);
		}
		else
		{
			ProcessNode(node->mChildren[i], scene, obj);
		}
	}
}

MeshComponent* MeshLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* object)
{
	RG_PROFILING_FUNCTION("Process Mesh");
	DEBUG_LOG("Processing mesh...");
	std::vector<float3> vertices;
	std::vector<float3> norms;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 3);
	texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		float3 vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		float3 normals;
		if (mesh->HasNormals())
		{
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
		}

		float2 coords;
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;

		norms.push_back(normals);
		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	MaterialComponent* diffuse = nullptr;
	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//TextureLoader::GetInstance()->ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");

		DEBUG_LOG("Material loading completed!");
	}
	MeshComponent* m = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);

	if (diffuse)
	{
		m->SetMaterial(diffuse);
		object->AddComponent(diffuse);
	}

	DEBUG_LOG("Mesh loading completed!");
	return m;
}

void MeshLoader::ProcessNode2(aiNode* node, const aiScene* scene, JsonParsing& nodeJ, JSON_Array* json)
{
	JsonParsing jsonValue = JsonParsing();
	jsonValue.SetNewJsonString(jsonValue.ValueToObject(jsonValue.GetRootValue()), "Name", node->mName.C_Str());
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh2(mesh, scene, jsonValue);
	}

	std::string name = "Childs" + std::string(node->mName.C_Str());
	JSON_Array* array = jsonValue.SetNewJsonArray(jsonValue.GetRootValue(), name.c_str());

	// Repeat the process until there's no more children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode2(node->mChildren[i], scene, jsonValue, array);
	}
	nodeJ.SetValueToArray(json, jsonValue.GetRootValue());
}

void MeshLoader::ProcessMesh2(aiMesh* mesh, const aiScene* scene, JsonParsing& json)
{
	RG_PROFILING_FUNCTION("Process Mesh");
	DEBUG_LOG("Processing mesh...");
	std::vector<float3> vertices;
	std::vector<float3> norms;
	std::vector<unsigned int> indices;
	std::vector<float2> texCoords;

	int numVertices = mesh->mNumVertices;
	int numFaces = mesh->mNumFaces;

	vertices.reserve(numVertices);
	indices.reserve(numFaces * 3);
	texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i)
	{
		float3 vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		float3 normals;
		if (mesh->HasNormals())
		{
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
		}

		float2 coords;
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;

		norms.push_back(normals);
		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < numFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::string meshName = MESHES_FOLDER;
	meshName += mesh->mName.C_Str();
	meshName += ".rgmesh";

	SaveMesh(meshName.c_str(), vertices, indices, norms, texCoords);

	JSON_Array* array = json.SetNewJsonArray(json.GetRootValue(), "Components");
	JsonParsing parse = JsonParsing();
	parse.SetNewJsonString(parse.ValueToObject(parse.GetRootValue()), "Mesh Path", meshName.c_str());

	if (mesh->mMaterialIndex >= 0)
	{
		DEBUG_LOG("Processing material...");

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureLoader::GetInstance()->ImportTexture(material, aiTextureType_DIFFUSE, "texture_diffuse", parse);
		json.SetValueToArray(array, parse.GetRootValue());

		DEBUG_LOG("Material loading completed!");
	}
	json.SetValueToArray(array, parse.GetRootValue());
}


MaterialComponent* MeshLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
	MaterialComponent* material = nullptr;

	for (unsigned int i = 0; i < 1; ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string aux = str.C_Str();
		aux = aux.substr(aux.find_last_of("\\") + 1, aux.length());
		std::string path = ASSETS_FOLDER;
		path += aux;
		//material = TextureLoader::GetInstance()->LoadTexture(path);
	}

	return material;
}

void MeshLoader::LoadingTransform(aiNode* node, GameObject* obj)
{
	aiVector3D pos;
	aiQuaternion quat;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, quat, pos);

	float3 p = { pos.x, pos.y, pos.z };
	Quat q = { quat.x, quat.y, quat.z, quat.w };
	float3 s = { scale.x, scale.y, scale.z };

	obj->GetComponent<TransformComponent>()->SetTransform(p, q, s);
}

Uint64 MeshLoader::SaveMesh(const char* name, std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float3>& normals, std::vector<float2>& texCoords)
{
	unsigned int header[4] = { vertices.size(), indices.size(), normals.size(), texCoords.size() };

	uint size = sizeof(header) + sizeof(float3) * vertices.size() + sizeof(unsigned int) * indices.size() + sizeof(float3) * normals.size() + sizeof(float2) * texCoords.size();

	char* buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	bytes = sizeof(float3) * vertices.size();
	memcpy(cursor, vertices.data(), bytes);
	cursor += bytes;

	bytes = sizeof(unsigned int) * indices.size();
	memcpy(cursor, indices.data(), bytes);
	cursor += bytes;

	bytes = sizeof(float3) * normals.size();
	memcpy(cursor, normals.data(), bytes);
	cursor += bytes;

	bytes = sizeof(float2) * texCoords.size();
	memcpy(cursor, texCoords.data(), bytes);
	cursor += bytes;

	if (app->fs->Save(name, buffer, size) > 0)
		DEBUG_LOG("Mesh %s saved succesfully", name);

	RELEASE_ARRAY(buffer);

	return size;
}

void MeshLoader::LoadMesh(const char* name, MeshComponent* mesh)
{
	char* buffer = nullptr;

	std::string meshPath(name);
	if (meshPath.find(".rgmesh") == std::string::npos)
	{
		meshPath = MESHES_FOLDER;
		meshPath += name;
		meshPath += ".rgmesh";
	}

	if (app->fs->Load(meshPath.c_str(), &buffer) > 0)
	{
		std::vector<float3> vertices;
		std::vector<unsigned int> indices;
		std::vector<float3> normals;
		std::vector<float2> texCoords;

		char* cursor = buffer;
		unsigned int* header = new unsigned int[4];

		// Loading header information
		uint bytes = sizeof(header) * sizeof(unsigned int);
		memcpy(header, buffer, bytes);
		cursor += bytes;

		// Setting information
		vertices.resize(header[0]);
		indices.resize(header[1]);
		normals.resize(header[2]);
		texCoords.resize(header[3]);

		// Loading vertices
		bytes = sizeof(float3) * vertices.size();
		memcpy(&vertices[0], cursor, bytes);
		cursor += bytes;

		// Loading indices
		bytes = sizeof(unsigned int) * indices.size();
		memcpy(indices.data(), cursor, bytes);
		cursor += bytes;

		// Loading normals
		bytes = sizeof(float3) * normals.size();
		memcpy(normals.data(), cursor, bytes);
		cursor += bytes;

		// Loading texture coordinates
		bytes = sizeof(float2) * texCoords.size();
		memcpy(texCoords.data(), cursor, bytes);

		Mesh* m = new Mesh(vertices, indices, normals, texCoords, std::string(name));
		mesh->SetMesh(m);

		RELEASE_ARRAY(header);
	}
	else
		DEBUG_LOG("Mesh file not found!");

	RELEASE_ARRAY(buffer);
}