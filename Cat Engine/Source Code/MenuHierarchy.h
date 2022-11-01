#pragma once

#include "Menu.h"

class GameObject;

class MenuHierarchy : public Menu
{
public:
	MenuHierarchy();

	~MenuHierarchy();

	bool Update(float dt) override;

	void ShowChildren(GameObject* parent);

private:
	bool gameObjectOptions;

	bool createGameObject;
};