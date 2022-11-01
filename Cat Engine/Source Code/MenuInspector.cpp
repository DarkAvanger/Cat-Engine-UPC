#include "MenuInspector.h" //To Do
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"

MenuInspector::MenuInspector() : Menu(true)
{
}

MenuInspector::~MenuInspector()
{
}

bool MenuInspector::Update(float dt)
{
	ImGui::Begin("Inspector", &active);
	if (!app->scene->GetRoot()->GetChilds().empty())
	{
		if (app->editor->GetSelected())
		{
			app->editor->GetSelected()->DrawEditor();
		}
	}
	ImGui::End();

	return true;
}