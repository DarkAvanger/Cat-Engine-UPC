#include "ResourceManager.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ModelImporter.h"

#include "MathGeoLib/src/Algorithm/Random/LCG.h"

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"

#include <stack>

#include "Profiling.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance) instance = new ResourceManager();

	return instance;
}

void ResourceManager::ReleaseInstance()
{
	RELEASE(instance);
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < textures.size(); ++i)
	{
		RELEASE(textures[i]);
	}
	textures.clear();

	for (int i = 0; i < meshes.size(); ++i)
	{
		RELEASE(meshes[i]);
	}
	meshes.clear();
}

void ResourceManager::CheckForNewResources()
{

}

uint ResourceManager::CreateResource(ResourceType type, std::string& assets, std::string& library)
{
	std::shared_ptr<Resource> resource = nullptr;

	std::map<uint, std::shared_ptr<Resource>>::iterator it;

	for (it = map.begin(); it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == assets)
		{
			library = (*it).second->GetLibraryPath();
			return (*it).first;
		}
	}

	LCG random;
	uint uid = random.IntFast();

	switch (type)
	{
	case ResourceType::NONE:
		break;
	case ResourceType::TEXTURE:
		library = TEXTURES_FOLDER + std::string("texture_") + std::to_string(uid) + ".cattexture";
		resource = std::make_shared<Texture>(uid, assets, library);
		break;
	case ResourceType::MESH:
		library = MESHES_FOLDER + std::string("mesh_") + std::to_string(uid) + ".catmesh";
		resource = std::make_shared<Mesh>(uid, assets, library);
		break;
	case ResourceType::MODEL:
		library = MODELS_FOLDER + std::string("model_") + std::to_string(uid) + ".catmodel";
		resource = std::make_shared<Model>(uid, assets, library);
		break;
	}

	if (resource != nullptr) map[uid] = resource;

	return uid;
}

void ResourceManager::CreateResourceCreated(ResourceType type, uint uid, std::string& assets, std::string& library)
{
	std::shared_ptr<Resource> resource = nullptr;
	switch (type)
	{
	case ResourceType::TEXTURE:
		resource = std::make_shared<Texture>(uid, assets, library);
		break;
	case ResourceType::MESH:
		resource = std::make_shared<Mesh>(uid, assets, library);
		break;
	case ResourceType::MODEL:
		resource = std::make_shared<Model>(uid, assets, library);
		break;
	default:
		break;
	}

	if (resource != nullptr && resource) map[uid] = resource;
}

std::shared_ptr<Resource> ResourceManager::LoadResource(uint uid)
{
	std::shared_ptr<Resource> res = nullptr;
	std::map<uint, std::shared_ptr<Resource>>::iterator it;
	it = map.find(uid);
	if (it != map.end())
	{
		res = map[uid];
		if (res != nullptr) res->Load();
	}

	return res;
}

std::shared_ptr<Resource> ResourceManager::LoadResource(std::string& path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		std::shared_ptr<Resource> res = (*it).second;
		if (res->GetAssetsPath() == path)
		{
			res->Load();
			return res;
		}
	}
}

bool ResourceManager::CheckResource(std::string& path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == path)
			return true;
	}
	return false;
}

void ResourceManager::ImportResourcesFromLibrary()
{
	std::vector<std::string> dirs;

	std::stack<std::string> directories;

	app->fs->DiscoverDirs("Library/", dirs);

	for (int i = 0; i < dirs.size(); ++i)
	{
		directories.push(dirs[i]);
	}

	dirs.clear();

	while (!directories.empty())
	{
		std::string dir = directories.top();

		std::vector<std::string> files;
		app->fs->DiscoverFiles(dir.c_str(), files);

		for (int i = 0; i < files.size(); ++i)
		{
			if (files[i].find(".rg") != std::string::npos)
			{
				std::string extension = files[i].substr(files[i].find_last_of("."), files[i].length());
				std::string metaFile = dir + files[i].substr(0, files[i].find_last_of(".")) + ".meta";

				char* buffer = nullptr;
				if (app->fs->Load(metaFile.c_str(), &buffer) > 0)
				{
					JsonParsing metaInfo(buffer);

					std::string assets(metaInfo.GetJsonString("Assets Path"));
					uint uid = metaInfo.GetJsonNumber("Uuid");

					if (files[i].find(".catmodel") != std::string::npos) CreateResourceCreated(ResourceType::MODEL, uid, assets, dir + files[i]);
					else if (files[i].find(".cattexture") != std::string::npos) CreateResourceCreated(ResourceType::TEXTURE, uid, assets, dir + files[i]);
					else if (files[i].find(".catmesh") != std::string::npos) CreateResourceCreated(ResourceType::MESH, uid, assets, dir + files[i]);

					RELEASE_ARRAY(buffer);
				}
			}
		}

		directories.pop();
	}
}

void ResourceManager::ImportAllResources()
{
	std::stack<std::string> dirsStack;
	dirsStack.push("Assets/");

	while (!dirsStack.empty())
	{
		std::string dir = dirsStack.top();

		std::vector<std::string> files;
		std::vector<std::string> dirs;

		app->fs->DiscoverFilesAndDirs(dir.c_str(), files, dirs);

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			ResourceType type = app->fs->CheckExtension(*it);
			switch (type)
			{
			case ResourceType::MODEL:
				ModelImporter::ImportModel(*it);
				break;
			case ResourceType::TEXTURE:
				TextureImporter::ImportTexture(*it);
				break;
			}
		}

		dirsStack.pop();

		for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
		{
			dirsStack.push(*it);
		}
	}
}

std::shared_ptr<Resource> ResourceManager::GetResource(uint uid)
{
	return map[uid];
}

std::shared_ptr<Resource> ResourceManager::GetResource(std::string path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it = map.begin();

	for (; it != map.end(); ++it)
	{
		if ((*it).second->GetAssetsPath() == path) return (*it).second;
	}

	return nullptr;
}

void ResourceManager::DeleteResource(std::string& path)
{
	std::map<uint, std::shared_ptr<Resource>>::iterator it;

	for (it = map.begin(); it != map.end(); ++it)
	{
		std::shared_ptr<Resource> resource = (*it).second;
		if (resource->GetAssetsPath() == path)
		{
			app->fs->RemoveFile(path.c_str());
			app->fs->RemoveFile(resource->GetLibraryPath().c_str());
			std::string meta = resource->GetLibraryPath();
			meta = meta.substr(0, meta.find_last_of(".")) + ".meta";
			app->fs->RemoveFile(meta.c_str());
			map.erase(it);
			return;
		}
	}
}

void ResourceManager::AddTexture(Texture* tex)
{
	textures.emplace_back(tex);
}

Texture* ResourceManager::IsTextureLoaded(std::string path)
{
	std::string p = path;
	if (p.find(".dds") == std::string::npos)
	{
		app->fs->GetFilenameWithoutExtension(p);
		p = TEXTURES_FOLDER + p + ".dds";
	}

	for (int i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->GetPath() == p)
			return textures[i];
	}

	return nullptr;
}

void ResourceManager::RemoveTexture(Texture* tex)
{
	for (std::vector<Texture*>::const_iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if ((*it) == tex)
		{
			RELEASE(tex);
			textures.erase(it);
			textures.shrink_to_fit();
			break;
		}
	}
}

void ResourceManager::AddMesh(Mesh* mesh)
{
	meshes.emplace_back(mesh);
}

Mesh* ResourceManager::IsMeshLoaded(std::string path)
{
	std::string p = path;
	if (p.find(".catmesh") == std::string::npos)
	{
		app->fs->GetFilenameWithoutExtension(p);
		p = MESHES_FOLDER + p + ".dds";
	}

	for (int i = 0; i < meshes.size(); ++i)
	{
		if (meshes[i]->GetPath() == p)
			return meshes[i];
	}
	return nullptr;
}

void ResourceManager::RemoveMesh(Mesh* mesh)
{
	for (std::vector<Mesh*>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if ((*it) == mesh)
		{
			RELEASE(mesh);
			meshes.erase(it);
			meshes.shrink_to_fit();
			break;
		}
	}
}