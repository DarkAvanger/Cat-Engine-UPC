#include "Globals.h"
#include "AudioSourceComponent.h"
#include "AudioManager.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"

#include "Imgui/imgui.h"

AudioSourceComponent::AudioSourceComponent(GameObject* own, TransformComponent* trans) : audioClip("ActivateBgMusic"), volume(50.0f), mute(false), transform(trans), pitch(0.0f), reverb(0.0f)
{
	owner = own;
	type = ComponentType::AUDIO_SOURCE;

	if (!owner->CheckAudioRegister())
	{
		AkGameObjectID cameraID = owner->GetUUID();
		AudioManager::Get()->RegisterGameObject(cameraID);
		owner->SetAudioRegister(true);
	}
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
		ImGui::InputText("##Audio", &audioClip[0], audioClip.size());

		ImGui::Text("Mute");
		ImGui::SameLine();
		if (ImGui::Checkbox("Mute", &mute))
		{
			if (mute) AK::SoundEngine::SetRTPCValue("Volume", 0, owner->GetUUID());
			else AK::SoundEngine::SetRTPCValue("Volume", volume, owner->GetUUID());
			DEBUG_LOG("THE ID IS %d", owner->GetUUID());

		}

		ImGui::Text("Volume");
		ImGui::SameLine();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 100.0f))
		{
			AK::SoundEngine::SetRTPCValue("Volume", volume, owner->GetUUID());
			mute = false;
		}
		ImGui::Text("Pitch");
		ImGui::SameLine();
		if (ImGui::SliderFloat("Pitch", &pitch, -2400.0f, 2400.0f))
		{
			AK::SoundEngine::SetRTPCValue("Pitch", pitch, owner->GetUUID());
		}
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
	AudioManager::Get()->CheckReverbGameObject(owner->GetUUID());
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