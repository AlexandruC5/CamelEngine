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
	InitSoundEngine();
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Start()
{
	LoadBankInfo();
	LoadAudioBank(banks[0]->bank_name.c_str());
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
	std::vector<Bank*>::const_iterator it_b;
	for (it_b = banks.begin(); it_b != banks.end(); ++it_b)
	{
		UnLoadAudioBank((*it_b)->bank_name.c_str());
		delete (*it_b);
	}
	sources.clear();
	listeners.clear();
	banks.clear();

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
	GnJSONObj sound_banks(banks_info.GetJSONObjectByName("SoundBanksInfo"));
	GnJSONArray banks_array(sound_banks.GetArray("SoundBanks"), sound_banks.GetJSONObject());

	for (uint cursor = 0u; cursor < banks_array.Size(); ++cursor)
	{
		if (strcmp(banks_array.GetObjectAt(cursor).GetString("ShortName", ""), "Init") != 0)
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
				tmp_bank->events[std::stoull(tmp_events.GetObjectAt(event_cursor).GetString("Id", ""))] = tmp_events.GetObjectAt(event_cursor).GetString("Name", "");
			}

			// Load bank files data
			tmp_audios = tmp_obj.GetArray("IncludedMemoryFiles");
			for (uint audio_cursor = 0u; audio_cursor < tmp_audios.Size(); ++audio_cursor)
			{
				tmp_bank->audios[std::stoull(tmp_audios.GetObjectAt(audio_cursor).GetString("Id", ""))] = tmp_audios.GetObjectAt(audio_cursor).GetString("ShortName", "");
			}

			banks.push_back(tmp_bank);
		}
	}
}


void ModuleAudio::LoadAudioBank(const char* name)
{
	AkBankID bankID;
	AKRESULT eResult = AK::SoundEngine::LoadBank(name, AK_DEFAULT_POOL_ID, bankID);

	if (eResult == AK_Success)
	{
		LOG("Bank created");
	}
	
}

void ModuleAudio::UnLoadAudioBank(const char* name)
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
	std::vector<AudioSource*>::const_iterator it;
	for (it = sources.begin(); it != sources.end(); ++it)
	{
		AK::SoundEngine::PostEvent("Pause_All", (*it)->GetID());
	}
}

void ModuleAudio::ResumeAudio() const
{
	std::vector<AudioSource*>::const_iterator it;
	for (it = sources.begin(); it != sources.end(); ++it)
	{
		AK::SoundEngine::PostEvent("Resume_All", (*it)->GetID());
	}
}

void ModuleAudio::SetListener(AudioListener* new_listener)
{
	listener = new_listener;
}

void ModuleAudio::SetIsAudioPlayed(bool boolean)
{
	isAudioPlayed = boolean;
}

void ModuleAudio::AddListenerToList(AudioListener* listener)
{
	listeners.push_back(listener);
}

void ModuleAudio::AddSourceToList(AudioSource* source)
{
	sources.push_back(source);
}



void ModuleAudio::ApplyEnvReverb(AkReal32 desired_level, const char* target)
{
	AkAuxSendValue environment;
	environment.listenerID = GetListenerID();
	environment.fControlValue = desired_level;
	environment.auxBusID = AK::SoundEngine::GetIDFromString(target);

	AKRESULT res = AK::SoundEngine::SetGameObjectAuxSendValues(GetListenerID(), &environment, 2);
}