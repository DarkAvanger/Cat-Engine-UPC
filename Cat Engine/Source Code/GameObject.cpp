#include "GameObject.h" //Fix
#include "Globals.h"

#include "Imgui/imgui.h"

#include "Profiling.h"

GameObject::GameObject() : active(true), parent(nullptr), name("Game Object"), newComponent(false)
{
	boundingBox.SetNegativeInfinity();
}

GameObject::~GameObject()
{
	for (int i = 0; i < components.size(); ++i)
	{
		RELEASE(components[i]);
	}
	components.clear();

	for (int i = 0; i < children.size(); ++i)
	{
		RELEASE(children[i]);
	}
	children.clear();
}

bool GameObject::Update(float dt)
{
	return true;
}

void GameObject::Draw()
{
	for (int i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component->GetActive())
			component->Draw();
	}

	for (int i = 0; i < children.size(); ++i)
	{
		GameObject* go = children[i];
		if (go->GetActive())
			go->Draw();
	}
}

void GameObject::DrawEditor()
{
	ImGui::Checkbox("Active", &active);
	ImGui::SameLine();
	ImGui::InputText("Name", &name[0], 20);
	ImGui::Separator();
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->OnEditor();
	}

	ImGui::NewLine();
	float x = ImGui::GetWindowSize().x;
	ImGui::SameLine((x / 2) - 50);
	if (ImGui::Button("New Component", ImVec2(100, 20)))
	{
		newComponent = true;
	}

	if (newComponent)
	{
		ImGui::OpenPopup("New Component");
		if (ImGui::BeginPopup("New Component"))
		{
			if (ImGui::Button("Mesh Component"))
			{
				CreateComponent(ComponentType::MESH_RENDERER);
				newComponent = false;
			}
			if (ImGui::Button("Material Component"))
			{
				CreateComponent(ComponentType::MATERIAL);
				newComponent = false;
			}
			else if (!ImGui::IsAnyItemHovered() && ((ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1])))
			{
				newComponent = false;
			}
			ImGui::EndPopup();
		}
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new TransformComponent(this);
		break;
	case ComponentType::MESH_RENDERER:
		component = new MeshComponent(this, GetComponent<TransformComponent>());
		break;
	case ComponentType::MATERIAL:
		component = new MaterialComponent(this);
		break;
	}

	if (component != nullptr)
	{
		component->SetOwner(this);
		components.push_back(component);
	}

	return component;
}

void GameObject::AddComponent(Component* component)
{
	component->SetOwner(this);
	components.emplace_back(component);
}

void GameObject::AddChild(GameObject* object)
{
	children.emplace_back(object);
}

char* GameObject::GetNameBuffer()
{	
	return &name[0];
}

void GameObject::SetAABB(std::vector<float3>& vertices)
{
	boundingBox.Enclose(vertices.data(), vertices.size());
}

void GameObject::SetTotalAABB()
{
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->SetTotalAABB();
		boundingBox.Enclose(children[i]->GetAABB());
	}
}

void GameObject::MoveChildrenUp(GameObject* child)
{
	if (child == children[0]) return;

	int size = children.size();
	for (int i = 0; i < size; ++i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i - 1];
			children[i - 1] = aux;
			break;
		}
	}
}

void GameObject::MoveChildrenDown(GameObject* child)
{
	int size = children.size() - 1;
	if (child == children[size]) return;

	for (int i = size; i >= 0; --i)
	{
		if (children[i] == child)
		{
			GameObject* aux = child;

			children[i] = children[i + 1];
			children[i + 1] = aux;
			break;
		}
	}
}