#pragma once

#include <string>
#include "assimp/material.h"

class JsonParsing;
class Texture;

typedef uint64_t Uint64;


class TextureLoader
{
public:
	static TextureLoader* GetInstance();
	static void ReleaseInstance();

	void ImportTexture(const aiMaterial* material, aiTextureType type, const char* typeName, JsonParsing& json);
	void ImportTexture(std::string& path);
	Uint64 SaveTexture(std::string& fileName);
	Texture* LoadTexture(std::string& path);

private:
	TextureLoader() {}
	TextureLoader(const TextureLoader&) {}
	static TextureLoader* instance;
};