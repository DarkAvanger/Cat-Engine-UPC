#pragma once

#include "Menu.h"

#include "Imgui/imgui.h"

class MenuConsole : public Menu
{
public:
	MenuConsole();

	~MenuConsole();

	bool Update(float dt) override;

	void AddLog(const char* fmt);

	void ClearConsole();

	inline bool& GetActive() { return active; }

private:
	ImGuiTextBuffer buf;

	ImVec2 size;
};