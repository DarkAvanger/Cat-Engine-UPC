#include "AudioManager.h"

#include "Globals.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>

#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>  

#include <AK/SoundEngine/Common/AkSoundEngine.h>

#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/AkRoomVerbFXFactory.h>

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#include "Profiling.h"

AudioManager* AudioManager::instance = nullptr;

void AudioManager::Create()
{
	if (instance == nullptr) instance = new AudioManager();
}

void AudioManager::Release()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	instance->lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get()) AK::IAkStreamMgr::Get()->Destroy();

	AK::MemoryMgr::Term();

	RELEASE(instance);
}

bool AudioManager::Init()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't create the Memory Manager");
		return false;
	}

	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	if (!AK::StreamMgr::Create(streamSettings))
	{
		DEBUG_LOG("Couldn't create the Stream Manager");
		return false;
	}

	AkDeviceSettings devSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(devSettings);

	if (lowLevelIO.Init(devSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't create the streaming device and Low-Level I/O system");
		return false;
	}

	AkInitSettings initSettings;
	AkPlatformInitSettings platfInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platfInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platfInitSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the sound engine");
		return false;
	}

	AkMusicSettings musicSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicSettings);

	if (AK::MusicEngine::Init(&musicSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the music engine");
		return false;
	}

	AkSpatialAudioInitSettings spatialSettings;

	if (AK::SpatialAudio::Init(spatialSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize the spatial audio");
		return false;
	}


#ifndef AK_OPTIMIZED

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		DEBUG_LOG("Couldn't initialize communication");
		return false;
	}

#endif

	lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	AkBankID bankID = 0;
	if (AK::SoundEngine::LoadBank(L"Init.bnk", bankID) != AK_Success)
	{
		DEBUG_LOG("Couldn't find the bank: Init.bnk");
		return false;
	}
	if (AK::SoundEngine::LoadBank(L"Test.bnk", bankID) != AK_Success)
	{
		DEBUG_LOG("Couldn't find the bank: Test.bnk");
		return false;
	}

	return true;
}

bool AudioManager::Update()
{
	return true;
}

void AudioManager::Render()
{
	AK::SoundEngine::RenderAudio();
}

void AudioManager::RegisterGameObject(int uuid)
{
	AK::SoundEngine::RegisterGameObj(uuid);
}

void AudioManager::UnregisterGameObject(int uuid)
{
	AK::SoundEngine::UnregisterGameObj(uuid);
}

void AudioManager::SetDefaultListener(AkGameObjectID* uuid)
{
	AK::SoundEngine::SetDefaultListeners(uuid, 1);
}

void AudioManager::SetPosition(int uuid, AkSoundPosition position)
{
	AK::SoundEngine::SetPosition(uuid, position);
}

void AudioManager::PostEvent(const char* name, int uuid)
{
	if (AK::SoundEngine::PostEvent(name, uuid) == AK_INVALID_PLAYING_ID)
	{
		DEBUG_LOG("Post event %s failed", name);
	}
}

AudioManager::AudioManager()
{
}