#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MenuConsole.h"
#include "MenuConfig.h"
#include "MenuBar.h"
#include "MenuAbout.h"
#include "MenuINspector.h"
#include "MenuHierarchy.h"


MainMenuBar::MainMenuBar() : Menu(true)
{
	showMenu = false;
}

MainMenuBar::~MainMenuBar()
{
}

bool MainMenuBar::Update(float dt)
{

	return true;
}

bool MainMenuBar::CleanUp()
{
	return true;
}

ConsoleMenu* MainMenuBar::GetConsole()
{
	return nullptr;
}