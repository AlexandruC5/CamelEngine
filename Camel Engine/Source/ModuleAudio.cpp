#include "ModuleAudio.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Application.h"
#include "FileSystem.h"
#include "GnJSON.h"
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
	LoadBankInfo();
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
	std::vector<AudioSource*>::const_iterator it;
	for (it = sources.begin(); it != sources.end(); ++it)
	{
		AKRESULT eResult = AK::SoundEngine::UnregisterGameObj((*it)->GetID());
		if (eResult != AK_Success)
		{
			assert(!"Could not unregister GameObject. See eResult variable to more info");
			LOG("Could not unregister GameObject. See eResult variable to more info");
		}
	}
	std::vector<AudioListener*>::const_iterator it_l;
	for (it_l = listeners.begin(); it_l != listeners.end(); ++it_l)
	{
		AKRESULT eResult = AK::SoundEngine::UnregisterGameObj((*it_l)->GetID());
		if (eResult != AK_Success)
		{
			assert(!"Could not unregister GameObject. See eResult variable to more info");
			LOG("Could not unregister GameObject. See eResult variable to more info");
		}
	}
	sources.clear();
	listeners.clear();
	UnLoadAudioBank("Engine_Banks.bnk");

	TermSoundEngine();
	return true;
}

const uint ModuleAudio::GetListenerID() const
{
	return listener->GetID();
}

void ModuleAudio::LoadBankInfo()
{
	char* buffer;
	uint size = FileSystem::Load("Assets/AudioFiles/SoundbanksInfo.json", &buffer);

	GnJSONObj banks_info(buffer);
	GnJSONArray banks_array(banks_info.GetArray("SoundBanks"), banks_info.GetJSONObject());

	for (uint cursor = 0u; cursor < banks_array.Size() && banks_array.GetObjectAt(cursor).GetString("ShortName", "") != "Init.bnk"; ++cursor)
	{
		Bank* tmp_bank = new Bank();
		GnJSONObj tmp_obj = banks_array.GetObjectAt(cursor);
		GnJSONArray tmp_events;
		GnJSONArray tmp_audios;

		tmp_bank->bank_name = tmp_obj.GetString("ShortName", "");
		tmp_bank->id = tmp_obj.GetInt("Id");

		// Load bank events data
		tmp_events = tmp_obj.GetArray("IncludedEvents");
		for (uint event_cursor = 0u; event_cursor < tmp_events.Size(); ++event_cursor)
		{
			tmp_bank->events[tmp_events.GetObjectAt(event_cursor).GetInt("Id")] = tmp_events.GetObjectAt(event_cursor).GetString("Name", "");
		}

		// Load bank files data
		tmp_audios = tmp_obj.GetArray("IncludedMemoryFiles");
		for (uint audio_cursor = 0u; audio_cursor < tmp_audios.Size(); ++audio_cursor)
		{
			tmp_bank->audios[tmp_audios.GetObjectAt(audio_cursor).GetInt("Id")] = tmp_audios.GetObjectAt(audio_cursor).GetString("Name", "");
		}

		banks.push_back(tmp_bank);
	}
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
