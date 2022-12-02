#include "MenuConsole.h"
#include "Application.h"

#include "Profiling.h"

MenuConsole::MenuConsole() : Menu(false)
{
}

MenuConsole::~MenuConsole()
{
	buf.clear();
}

bool MenuConsole::Update(float dt)
{
	bool ret = false;

	ImGui::Begin("Console", &active);
	if (ImGui::Button("Clear console"))
	{
		ClearConsole();
	}
	ImGui::Separator();
	ImGui::BeginChild("LOG");
	ImGui::TextUnformatted(buf.begin());
	ImGui::EndChild();
	ImGui::End();

	return true;
}

void MenuConsole::AddLog(const char* fmt)
{
	va_list args;
	va_start(args, fmt);
	buf.appendfv(fmt, args);
	va_end(args);
}

void MenuConsole::ClearConsole()
{
	buf.clear();
}