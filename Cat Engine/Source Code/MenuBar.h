#pragma once

#include "Menu.h"
#include <vector>

class MenuConsole;

enum class Menus
{
	CONSOLE = 0,
	CONFIGURATION = 1,
	ABOUT = 2,
	INSPECTOR = 3,
	HIERARCHY = 4
};

class MenuBar : public Menu
{
public:
	MenuBar();
	~MenuBar();

	bool Update(float dt) override;
	bool CleanUp() override;

	MenuConsole* GetConsole();

private:
	bool showMenu;

	std::vector<Menu*> menus;
};