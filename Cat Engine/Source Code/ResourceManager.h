#pragma once
#include "Globals.h"

#include <vector>
#include <string>

#include <map>
#include <memory>

enum class ResourceType;

class Texture;
class Mesh;
class Resource;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	static void ReleaseInstance();
	~ResourceManager();

	std::shared_ptr<Resource> CreateResource(ResourceType type, uint uid);

	void LoadResource();

	void AddTexture(Texture* tex);
	Texture* IsTextureLoaded(std::string path);
	void RemoveTexture(Texture* tex);

	void AddMesh(Mesh* mesh);
	Mesh* IsMeshLoaded(std::string path);
	void RemoveMesh(Mesh* mesh);

private:
	ResourceManager();

	static ResourceManager* instance;

	std::map<uint, std::shared_ptr<Resource>> map;
	std::vector<Texture*> textures;
	std::vector<Mesh*> meshes;
};