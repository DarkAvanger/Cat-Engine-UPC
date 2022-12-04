#include "MenuInspector.h" 
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Resource.h"

#include "GameObject.h"

#include "Profiling.h"

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
		if (app->editor->GetGO())
		{
			app->editor->GetGO()->DrawEditor();
		}
		else if (app->editor->GetResource())
		{
			app->editor->GetResource()->DrawOnEditor();
		}
	}
	ImGui::End();

	return true;
}