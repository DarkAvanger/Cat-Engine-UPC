#include "MenuHierarchy.h"

#include "Application.h"
#include "ModuleScene.h"

HierarchyMenu::HierarchyMenu() : Menu(true)
{
	gameObjectOptions = false;
	createGameObject = false;
}

HierarchyMenu::~HierarchyMenu()
{
}

bool HierarchyMenu::Update(float dt)
{
	return true;
}

void HierarchyMenu::ShowChildren(GameObject* parent)
{

}