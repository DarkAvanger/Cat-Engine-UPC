#include "Globals.h"
#include "glmath.h"
#include "Module.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();
	bool Start();

public:

	bool demo = false;
};