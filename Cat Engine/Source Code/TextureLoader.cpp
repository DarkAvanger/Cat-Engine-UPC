#include "TextureLoader.h"
#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "GameObject.h"
#include "FileSystem.h"
#include "Texture.h"

#include "IL/il.h"
#include "glew/include/GL/glew.h"

#include "Profiling.h"

TextureLoader* TextureLoader::instance = nullptr;

TextureLoader* TextureLoader::GetInstance()
{
	if (instance == nullptr) instance = new TextureLoader();

	return instance;
}

void TextureLoader::ReleaseInstance()
{
	RELEASE(instance);
}

void TextureLoader::ImportTexture(const aiMaterial* material, aiTextureType type, const char* typeName)
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string aux = str.C_Str();
		aux = aux.substr(aux.find_last_of("\\") + 1, aux.length());
		std::string path = ASSETS_FOLDER;
		path += aux;

		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(path.c_str());
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		path = path.substr(path.find_last_of("/") + 1, path.length());
		path = path.substr(0, path.find_last_of("."));
		path = path.insert(0, MATERIALS_FOLDER);
		path += ".dds";
		//*component = new MaterialComponent(image, w, h, path);
		Uint64 size = SaveTexture(path);
		ilDeleteImages(1, &image);
	}
}

void TextureLoader::ImportTexture(std::string& path)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilLoadImage(path.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	path = path.substr(path.find_last_of("/") + 1, path.length());
	path = path.substr(0, path.find_last_of("."));
	path = path.insert(0, MATERIALS_FOLDER);
	path += ".dds";

	Uint64 size = SaveTexture(path);
	ilDeleteImages(1, &image);
}

Uint64 TextureLoader::SaveTexture(std::string& fileName)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			if (app->fs->Save(fileName.c_str(), data, size) > 0)
				DEBUG_LOG("Texture saved!");
		}

		RELEASE_ARRAY(data);
	}

	return size;
}

void TextureLoader::LoadTexture(std::string& path, MaterialComponent* material)
{
	char* buffer = nullptr;

	path = path.substr(path.find_last_of("\\") + 1, path.length());
	path = path.substr(path.find_last_of("/") + 1, path.length());
	path = path.substr(0, path.find_last_of("."));

	std::string p = MATERIALS_FOLDER + path;
	p += ".dds";

	unsigned int size = app->fs->Load(p.c_str(), &buffer);

	if (size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadL(IL_DDS, buffer, size);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);

		GLubyte* data = ilGetData();

		material->SetTexture(new Texture(image, w, h, data, p));

		ilDeleteImages(1, &image);
	}

	RELEASE_ARRAY(buffer);
}

void TextureLoader::LoadTexture(std::string& path, Texture** checker)
{
	char* buffer = nullptr;

	std::string p = MATERIALS_FOLDER + path;
	p += ".dds";

	unsigned int size = app->fs->Load(p.c_str(), &buffer);

	if (size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadL(IL_DDS, buffer, size);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);

		GLubyte* data = ilGetData();

		*checker = new Texture(image, w, h, data, p);

		ilDeleteImages(1, &image);
	}

	RELEASE_ARRAY(buffer);
}