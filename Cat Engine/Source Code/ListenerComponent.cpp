#include "Globals.h"
#include "ListenerComponent.h"
#include "GameObject.h"
#include "AudioManager.h"
#include "ComponentTransform.h"

#include "Imgui/imgui_internal.h"

ListenerComponent::ListenerComponent(GameObject* own, TransformComponent* trans) : activeListener(true), transform(trans)
{
	owner = own;
	type = ComponentType::AUDIO_LISTENER;

	AkGameObjectID cameraID = owner->GetUUID();
	AudioManager::Get()->RegisterGameObject(cameraID);
}

ListenerComponent::~ListenerComponent()
{
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void ListenerComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Listener"))
	{
		ImGui::Text("AudioClip");
		ImGui::SameLine(ImGui::GetWindowWidth() * 0.65f);
		ImGui::Checkbox(" ", &activeListener);
		ImGui::SameLine();
		ImGui::Text("Listen");
	}
	ImGui::PopID();
}

bool ListenerComponent::Update(float dt)
{
	float3 position = transform->GetPosition();
	AkSoundPosition audioSourcePos;
	float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
	audioSourcePos.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	audioSourcePos.SetPosition(position.x, position.y, position.z);
	AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);

	return true;
}

bool ListenerComponent::OnLoad(JsonParsing& node)
{
	return true;
}

bool ListenerComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return true;
}