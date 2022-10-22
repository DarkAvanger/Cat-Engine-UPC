#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "MenuConfig.h"

#include <Glew/include/glew.h>
#include <SDL/include/SDL.h>


ConfigurationMenu::ConfigurationMenu() : Menu(true)
{
	memoryCount = 0;

	activeWindow = true;
	openOptions = false;
	activeInput = true;
	activeHardware = true;
}

ConfigurationMenu::~ConfigurationMenu()
{
}

bool ConfigurationMenu::Update(float dt)
{
	return true;
}
