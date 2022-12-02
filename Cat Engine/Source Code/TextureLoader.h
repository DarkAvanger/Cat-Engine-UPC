#pragma once

#include <string>
#include "assimp/material.h"

class JsonParsing;

typedef uint64_t Uint64;

class MaterialComponent;
class Texture;

class TextureLoader
{
public:
	static TextureLoader* GetInstance();
	static void ReleaseInstance();

	void ImportTexture(const aiMaterial* material, aiTextureType type, const char* typeName, JsonParsing& json);
	void ImportTexture(std::string& path);
	Uint64 SaveTexture(std::string& fileName);
	void LoadTexture(std::string& path, MaterialComponent* material);
	void LoadTexture(std::string& path, Texture** checker);

private:
	TextureLoader() {}
	TextureLoader(const TextureLoader&) {}
	static TextureLoader* instance;
};