#pragma once

#include <string>
#include <windows.h>
#include <commdlg.h>
#include "Application.h"
#include "ModuleWindow.h"
#include <SDL/include/SDL_syswm.h>

namespace Dialogs
{
	static std::string OpenFile(const char* path)
	{
		OPENFILENAMEA openFile;
		char sizeFile[260] = {};
		ZeroMemory(&openFile, sizeof(OPENFILENAMEA));
		openFile.lStructSize = sizeof(OPENFILENAMEA);

		openFile.hwndOwner = GetActiveWindow();
		openFile.lpstrFile = sizeFile;
		openFile.nMaxFile = sizeof(openFile);
		openFile.lpstrFilter = path;
		openFile.lpstrDefExt = "cat";
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}

	static std::string SaveFile(const char* path)
	{
		OPENFILENAMEA openFile;
		char sizeFile[260] = {};
		ZeroMemory(&openFile, sizeof(OPENFILENAMEA));
		openFile.lStructSize = sizeof(OPENFILENAMEA);

		openFile.hwndOwner = GetActiveWindow();
		openFile.lpstrFile = sizeFile;
		openFile.nMaxFile = sizeof(openFile);
		openFile.lpstrFilter = path;
		openFile.lpstrDefExt = "cat";
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}
}