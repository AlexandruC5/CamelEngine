#include "ModuleAudio.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Application.h"
#include "Time.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	name = "audio";
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Start()
{
	InitSoundEngine();
	LoadAudioBank("Warriors.bnk");
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	if (Time::gameClock.started)
	{
		if (!isAudioPlayed)
		{
			PlayOnAwake();
			isAudioPlayed = true;
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	ProcessAudio();
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	sources.clear();
	listeners.clear();
	UnLoadAudioBank("Warriors.bnk");

	TermSoundEngine();
	return true;
}

const uint ModuleAudio::GetListenerID() const
{
	return listener->GetID();
}

void ModuleAudio::LoadAudioBank(char* name)
{
	AkBankID bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(name, AK_DEFAULT_POOL_ID, bankID);

	if (eResult == AK_Success)
	{
		LOG("Bank created");
	}
}

void ModuleAudio::UnLoadAudioBank(char* name)
{
	AKRESULT eResult = AK::SoundEngine::UnloadBank(name, NULL);
	if (eResult == AK_Success)
	{
		LOG("Bank unloaded");
	}
}

void ModuleAudio::PlayOnAwake() const
{
	std::vector<AudioSource*>::const_iterator it;
	for (it = sources.begin(); it != sources.end(); ++it)
	{
		if ((*it)->GetPlayOnAwake() == true)
		{
			(*it)->PlayAudioByEvent((*it)->GetAudioToPlay());
		}
	}
}

void ModuleAudio::StopAudio() const
{
	AK::SoundEngine::StopAll();
}

void ModuleAudio::PauseAudio() const
{
	AK::SoundEngine::PostEvent("Pause_All", AK_INVALID_GAME_OBJECT);
}

void ModuleAudio::ResumeAudio() const
{
	AK::SoundEngine::PostEvent("Resume_All", AK_INVALID_GAME_OBJECT);
}

void ModuleAudio::SetListener(AudioListener* new_listener)
{
	listener = new_listener;
}

void ModuleAudio::AddListenerToList(AudioListener* listener)
{
	listeners.push_back(listener);
}

void ModuleAudio::AddSourceToList(AudioSource* source)
{
	sources.push_back(source);
}
