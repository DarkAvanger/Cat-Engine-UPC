#include "MenuConsole.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

ConsoleMenu::ConsoleMenu() : Menu(true)
{
}

ConsoleMenu::~ConsoleMenu()
{
	guiBuffer.clear();
}

bool ConsoleMenu::Update(float dt)
{
	bool ret = false;

	ImGui::Begin("Console", &active);
	if (ImGui::Button("Clear console"))
	{
		ClearConsole();
	}
	ImGui::Separator();
	ImGui::BeginChild("LOG");
	ImGui::TextUnformatted(guiBuffer.begin());
	ImGui::EndChild();
	ImGui::End();

	return true;
}

void ConsoleMenu::AddLog(const char* fmt)
{
	va_list args;
	va_start(args, fmt);
	guiBuffer.appendfv(fmt, args);
	va_end(args);
}

void ConsoleMenu::ClearConsole()
{
	guiBuffer.clear();
}