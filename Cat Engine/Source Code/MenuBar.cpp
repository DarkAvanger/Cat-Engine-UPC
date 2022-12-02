#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "MenuConsole.h"
#include "MenuConfig.h"
#include "ModuleScene.h"
#include "MenuBar.h"
#include "MenuAbout.h"
#include "MenuInspector.h"
#include "MenuHierarchy.h"
#include "MenuContentBrowser.h"

#include "Profiling.h"

MenuBar::MenuBar() : Menu(true)
{
	showMenu = false;

	menus.reserve(6);
	menus.emplace_back(new MenuConsole());
	menus.emplace_back(new MenuConfig());
	menus.emplace_back(new MenuAbout());
	menus.emplace_back(new MenuInspector());
	menus.emplace_back(new MenuHierarchy());
	menus.emplace_back(new ContentBrowserMenu());
}

MenuBar::~MenuBar()
{
}

bool MenuBar::Start()
{
	for (int i = 0; i < menus.size(); ++i)
	{
		menus[i]->Start();
	}

	return true;
}

bool MenuBar::Update(float dt)
{

	if (ImGui::BeginMainMenuBar())
	{
		bool ret = false;

		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Project", NULL, &ret);
			ImGui::MenuItem("Open Project", NULL, &ret);

			ImGui::Separator();

			if (ImGui::MenuItem("Load", "Ctrl + L", &ret))
			{
				app->scene->LoadScene("Assets/Scenes/scene.json");
			}
			if (ImGui::MenuItem("Save", "Ctrl + S", &ret))
			{
				app->scene->SaveScene();
			}
			if (ImGui::MenuItem("Exit", "ESC", &ret))
			{
				return false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the file menu");
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl + Z", &ret);
			ImGui::MenuItem("Redo", "Ctrl + Y", &ret);
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the edit menu");
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Console", NULL, &GetConsole()->active);
			ImGui::MenuItem("Configuration", NULL, &menus[(int)Menus::CONFIGURATION]->active);
			ImGui::MenuItem("Hierarchy", NULL, &menus[(int)Menus::HIERARCHY]->active);
			ImGui::MenuItem("Inspector", NULL, &menus[(int)Menus::INSPECTOR]->active);

			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the window menu");
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Depth Test", NULL, app->renderer3D->GetDepthTest()))
			{
				app->renderer3D->SetDepthTest();
			}
			if (ImGui::MenuItem("Cull Face", NULL, app->renderer3D->GetCullFace()))
			{
				app->renderer3D->SetCullFace();
			}
			if (ImGui::MenuItem("Lighting", NULL, app->renderer3D->GetLighting()))
			{
				app->renderer3D->SetLighting();
			}
			if (ImGui::MenuItem("Color Material", NULL, app->renderer3D->GetColorMaterial()))
			{
				app->renderer3D->SetColorMaterial();
			}
			if (ImGui::MenuItem("Texture 2D", NULL, app->renderer3D->GetTexture2D()))
			{
				app->renderer3D->SetTexture2D();
			}
			if (ImGui::MenuItem("Stencil", NULL, app->renderer3D->GetStencil()))
			{
				app->renderer3D->SetStencil();
			}
			if (ImGui::MenuItem("Blending", NULL, app->renderer3D->GetBlending()))
			{
				app->renderer3D->SetBlending();
			}
			if (ImGui::MenuItem("Wire", NULL, app->renderer3D->GetWireMode()))
			{
				app->renderer3D->SetWireMode();
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the view menu");
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Demo Menu", NULL, &showMenu);
			ImGui::MenuItem("About Cat Engine", "", &menus[(int)Menus::ABOUT]->active);
			if (ImGui::MenuItem("Documentation", "F1", &ret))
			{
				app->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC");
			}
			if (ImGui::MenuItem("Report a Bug", "", &ret))
			{
				app->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC/issues");
			}
			if (ImGui::MenuItem("Download latest", "", &ret))
			{
				app->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC/releases");
			}
			ImGui::EndMenu();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens the help menu");
		}

		ImGui::EndMainMenuBar();
	}

	if (showMenu)
	{
		ImGui::ShowDemoWindow(&showMenu);
		ImGui::ShowMetricsWindow(&showMenu);
	}

	for (unsigned int i = 0; i < menus.size(); ++i)
	{
		if (menus[i]->active) menus[i]->Update(dt);
	}

	return true;
}

bool MenuBar::CleanUp()
{
	for (unsigned int i = 0; i < menus.size(); ++i)
	{
		RELEASE(menus[i]);
	}

	menus.clear();

	return true;
}

MenuConsole* MenuBar::GetConsole()
{
	if (!menus.empty())
	{
		MenuConsole* test = (MenuConsole*)menus[(int)Menus::CONSOLE];

		return test;
	}

	return nullptr;
}