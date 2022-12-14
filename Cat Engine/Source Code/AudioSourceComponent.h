#pragma once

#include "Component.h"


class TransformComponent;
class IndexBuffer;
class VertexBuffer;
class AudioSourceComponent : public Component
{
public:
	AudioSourceComponent(GameObject* own, TransformComponent* trans);
	~AudioSourceComponent();

	void PlayClipOnAwake();
	void StopClip();
	void PauseClip();
	void ResumeClip();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void PlayClip();
private:

	TransformComponent* transform;

	std::string audioClip;
	unsigned int playingID;
	bool mute;
	float volume;
	float pitch;
	bool playOnAwake;
};