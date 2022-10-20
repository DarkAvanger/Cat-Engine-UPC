#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

//Search HiPo for interesting information

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui::ShowDemoWindow(&demo);

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			return UPDATE_STOP;
		}
		if (ImGui::MenuItem("Open"))
		{
			return UPDATE_STOP;
		}
		if (ImGui::MenuItem("Quit"))
		{
			return UPDATE_STOP;
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Documentation"))
			App->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC");

		if (ImGui::MenuItem("Download latest"))
			App->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC/releases");

		if (ImGui::MenuItem("Report a bug"))
			App->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC/issues");

		if (ImGui::MenuItem("About"))
		{
			App->RequestBrowser("https://github.com/DarkAvanger/Cat-Engine-UPC/issues");
			/*ImGui::SetNextWindowSize({350,250});
			ImGuiWindowFlags inspectorFlags = 0;
			inspectorFlags |= ImGuiWindowFlags_NoResize;
			inspectorFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
			ImGui::Begin("");
			ImGui::Text("Cat Engine");
			ImGui::Spacing();
			ImGui::Text("Game engine created at CITM Terrassa ");
			ImGui::Separator();
			ImGui::Text("Created by Angel Consola Lopez ");*/
		}
			

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_Quit();

	return true;
}