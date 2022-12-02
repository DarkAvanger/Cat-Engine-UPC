#pragma once

#include "Menu.h"
#include <vector>
#include <string>

class MenuConsole;

enum class Menus
{
	CONSOLE = 0,
	CONFIGURATION = 1,
	ABOUT = 2,
	INSPECTOR = 3,
	HIERARCHY = 4,
	CONTENT_BROWSER = 5,
};

class MenuBar : public Menu
{
public:
	MenuBar();
	~MenuBar();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	MenuConsole* GetConsole();
	std::string& GetCurrentDir();

private:
	bool showMenu;

	std::vector<Menu*> menus;
};