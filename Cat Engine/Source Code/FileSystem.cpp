#include "FileSystem.h"
#include "Application.h"
#include "Globals.h"
#include "MeshLoader.h"
#include "TextureLoader.h"

#include "SDL/include/SDL_filesystem.h"
#include "assimp/cimport.h"
#include "Assimp.h"
#include "IL/il.h"

#include <vector>

#include "Profiling.h"

FileSystem::FileSystem(const char* assetsPath) : name("FileSystem")
{
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	AddPath(".");

	if (0 && assetsPath != nullptr)
		AddPath(assetsPath);

	DEBUG_LOG("FileSystem Operations base is [%s] plus:", GetBasePath());
	DEBUG_LOG(GetReadPaths());


	if (PHYSFS_setWriteDir(".") == 0)
		DEBUG_LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	const char* dirs[] = {
		ASSETS_FOLDER, SETTINGS_FOLDER, LIBRARY_FOLDER, LIBRARY_FOLDER MATERIALS_FOLDER, LIBRARY_FOLDER MESHES_FOLDER
	};

	for (uint i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}

	CreateAssimpIO();

	ilInit();
	
	texExtension = { ".png", ".jpg", ".dds" };
	modelExtension = { ".obj", ".fbx", ".3DS", ".FBX"};
}

FileSystem::~FileSystem()
{
	RELEASE(assimpIO);
	aiDetachAllLogStreams();
	PHYSFS_deinit();
}

bool FileSystem::Init(JsonParsing& node)
{
	DEBUG_LOG("Loading File System");
	bool ret = true;

	char* writePath = SDL_GetPrefPath(app->GetOrganizationName(), app->GetAppName());



	SDL_free(writePath);

	return ret;
}

bool FileSystem::CleanUp()
{
	return true;
}

bool FileSystem::AddPath(const char* path)
{
	bool ret = false;

	if (PHYSFS_mount(path, nullptr, 1) == 0)
		DEBUG_LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

uint FileSystem::Load(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint)PHYSFS_read(fsFile, *buffer, 1, size);
			if (readed != size)
			{
				DEBUG_LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
				ret = readed;
		}

		if (PHYSFS_close(fsFile) == 0)
			DEBUG_LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		DEBUG_LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

uint FileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append)
{
	unsigned int ret = 0;

	bool overWrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fsFile = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fsFile != nullptr)
	{
		uint written = (uint)PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
			DEBUG_LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		else
		{
			if (append == true)
				DEBUG_LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			else if (overWrite == false)
				DEBUG_LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);

			ret = written;
		}

		if (PHYSFS_close(fsFile) == 0)
			DEBUG_LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		DEBUG_LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

const char* FileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char** path;
	for (path = PHYSFS_getSearchPath(); *path != nullptr; ++path)
	{
		strcat_s(paths, 512, *path);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

void FileSystem::LoadFile(std::string& path)
{
	std::string extension = path.substr(path.find_last_of(".", path.length()));
	std::list<std::string>::iterator s;
	std::list<std::string>::iterator end = modelExtension.end();
	
	for (s = modelExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Loading Model");
			MeshLoader::GetInstance()->LoadingModel(path);
			return;
		}
	}

	end = texExtension.end();

	for (s = texExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Loading Texture");
			TextureLoader::GetInstance()->LoadTextureToSelected(path);
			return;
		}
	}
}

void FileSystem::CreateAssimpIO()
{
	assimpIO = new aiFileIO;
	assimpIO->OpenProc = AssimpOpen;
	assimpIO->CloseProc = AssimpClose;

	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}