#pragma once

#include "Menu.h"

class MenuInspector : public Menu
{
public:
	MenuInspector();

	~MenuInspector();

	bool Update(float dt);
};