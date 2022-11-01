#pragma once

#include "Menu.h"

#include <vector>

class MenuConfig : public Menu
{
public:

	MenuConfig();
	~MenuConfig();

	bool Update(float dt) override;

private:
	std::vector<float> fps;
	std::vector<float> ms;
	std::vector<float> memory;

	int memoryCount;

	bool activeWindow;
	bool openOptions;
	bool activeInput;
	bool activeHardware;
};