#include "Globals.h"
#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "ComponentTransform.h"
#include "GameObject.h"

#include "Imgui/imgui.h"

AudioSourceComponent::AudioSourceComponent(GameObject* own, TransformComponent* trans) : audioClip("Sonido"), volume(1.0f), mute(false), transform(trans)
{
	owner = own;
	type = ComponentType::AUDIO_SOURCE;

	AkGameObjectID cameraID = owner->GetUUID();
	AudioManager::Get()->RegisterGameObject(cameraID);
}

AudioSourceComponent::~AudioSourceComponent()
{
	AudioManager::Get()->UnregisterGameObject(owner->GetUUID());
}

void AudioSourceComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("AudioSource"))
	{
		ImGui::Text("AudioClip");
		ImGui::SameLine();
		ImGui::InputText("Audio", &audioClip[0], audioClip.size());

		ImGui::Text("Mute");
		ImGui::SameLine();
		ImGui::Checkbox("Mute", &mute);

		ImGui::Text("Volume");
		ImGui::SameLine();
		ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
	}
	ImGui::PopID();
}

bool AudioSourceComponent::Update(float dt)
{
	float3 position = transform->GetPosition();
	AkSoundPosition audioSourcePos;
	audioSourcePos.SetPosition(position.x, position.y, position.z);
	float3 orientation = transform->GetRotation().ToEulerXYZ().Normalized();
	audioSourcePos.SetOrientation({ 0, 0, -1 }, { 0,1,0 });
	AudioManager::Get()->SetPosition(owner->GetUUID(), audioSourcePos);
	return true;
}

bool AudioSourceComponent::OnLoad(JsonParsing& node)
{
	return true;
}

bool AudioSourceComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return true;
}

void AudioSourceComponent::PlayClip()
{
	AudioManager::Get()->PostEvent(audioClip.c_str(), owner->GetUUID());
}