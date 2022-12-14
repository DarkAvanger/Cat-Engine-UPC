#pragma once

#include "AudioSourceComponent.h"
#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>
#include "AudioReverbZoneComponent.h"
#include <vector>

struct WwiseData
{
	std::vector<std::string> events;
	std::vector<std::string> busses;
	std::vector<std::string> auxBusses;
	std::vector<std::string> banks;
};

class TransformComponent;

class AudioManager
{
public:
	static void Create();
	static void Release();

	~AudioManager() {}

	void ReadIDs();

	static inline AudioManager* Get() { return instance; }

	bool Init();

	bool Update();

	void Render();

	void RegisterGameObject(int uuid);
	void UnregisterGameObject(int uuid);

	void AddReverbZone(AudioReverbZoneComponent* reverbZone);
	void DeleteReverbZone(AudioReverbZoneComponent* reverbZone);
	void CheckReverbGameObject(unsigned int UUID);

	void AddAudioSource(AudioSourceComponent* audioSource);
	void DeleteAudioSource(AudioSourceComponent* audioSource);
	void StopAllAudioSources();
	void PlayAllAudioSources();
	void ResumeAllAudioSources();
	void PauseAllAudioSources();

	void SetDefaultListener(AkGameObjectID* uuid, TransformComponent* listenerPosition);
	void SetPosition(int uuid, AkSoundPosition position);

	AkPlayingID PostEvent(const char* name, int uuid);

	inline std::vector<std::string> GetEventsList() const { return wwiseInfo.events; }
	inline std::vector<std::string> GetBussesList() const { return wwiseInfo.busses; }

private:
	AudioManager();

	std::vector<AudioReverbZoneComponent*> reverbZones;

	std::vector<AudioSourceComponent*> audioSources;


	TransformComponent* currentListenerPosition;

	static AudioManager* instance;
	CAkFilePackageLowLevelIOBlocking lowLevelIO;

	WwiseData wwiseInfo;
};