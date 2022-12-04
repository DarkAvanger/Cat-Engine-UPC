#pragma once

#include "Module.h"
#include "MenuBar.h"
#include "MenuViewport.h"
#include "GameView.h"

#include "Imgui/ImGuizmo.h"

#include <vector>
#include <string>

class GameObject;
class Resource;

class ModuleEditor : public Module
{
public:
	ModuleEditor();

	~ModuleEditor();

	bool Start() override;
	bool Update(float dt) override;
	bool Draw(Framebuffer* editorBuffer, Framebuffer* gameBuffer);
	bool CleanUp() override;

	inline GameObject* GetGO() { return selected; }
	inline Resource* GetResource() { return resource; }
	inline GameObject* GetSelectedParent() { return selectedParent; }
	
	void SetResource(Resource* res);
	void SetGO(GameObject* obj);
	inline void SetSelectedParent(GameObject* obj) { selectedParent = obj; }

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void LogConsole(const char* string);
	inline std::string& GetCurrentDir() { return mainMenuBar.GetCurrentDir(); }

	inline MenuViewport* GetViewport() { return viewport; }

private:
	MenuBar mainMenuBar;

	GameObject* selected;
	GameObject* selectedParent;
	Resource* resource;

	MenuViewport* viewport;
	GameView* gameView;
	ImGuizmo::OPERATION currentOperation;
};