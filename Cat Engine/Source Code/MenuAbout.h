#pragma once

#include "Menu.h"

class MenuAbout : public Menu
{
public:
	MenuAbout();
	~MenuAbout();

	bool Update(float dt) override;
};