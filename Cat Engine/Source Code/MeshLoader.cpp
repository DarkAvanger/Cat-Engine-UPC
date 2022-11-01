#include "MeshLoader.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "TextureLoader.h"

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

void MeshLoader::LoadingModel(std::string& path)
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
	GameObject* object = app->scene->CreateGameObject(nullptr);
	object->SetName(p.c_str());
	ProcessNode(scene->mRootNode, scene, object);
	object->SetTotalAABB();
}

void MeshLoader::ProcessNode(aiNode* node, const aiScene* scene, GameObject* obj)
{
	LoadingTransform(node, obj);
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshComponent* m = ProcessMesh(mesh, scene, obj);
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
		TextureLoader::GetInstance()->ImportTexture(material, &diffuse, aiTextureType_DIFFUSE, "texture_diffuse");

		DEBUG_LOG("Material loading completed!");
	}
	MeshComponent* m = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);

	if (mesh->HasNormals())
		m->SetMesh(vertices, indices, texCoords, norms);
	else
		m->SetMesh(vertices, indices, texCoords);

	if (diffuse)
	{
		m->SetMaterial(diffuse);
		object->AddComponent(diffuse);
	}

	DEBUG_LOG("Mesh loading completed!");
	return m;
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
		material = TextureLoader::GetInstance()->LoadTexture(path);
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