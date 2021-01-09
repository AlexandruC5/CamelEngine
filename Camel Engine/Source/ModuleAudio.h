#pragma once
#include "Globals.h"
#include "Module.h"
#include "WwiseLibrary.h"

#include <vector>
#include <string>
#include <map>

class AudioSource;
class AudioListener;

typedef struct
{
	std::string						bank_name;
	bool							loaded_in_heap;
	uint64							id = 0u;
	std::map<uint64, std::string>	events;
	std::map<uint64, std::string>	audios;
} Bank;

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	const uint GetListenerID() const;

	void LoadBankInfo();
	void LoadAudioBank(const char* name);
	void UnLoadAudioBank(const char* name);

	void PlayOnAwake()const;
	void StopAudio()const;
	void PauseAudio()const;
	void ResumeAudio()const;
	void SetListener(AudioListener* new_listener);

	void AddListenerToList(AudioListener* listener);
	void AddSourceToList(AudioSource* source);

private:
	AudioListener* listener;
	AudioSource* aud_source;
	bool isAudioPlayed = false;

public:
	std::vector<Bank*> banks;

	std::vector<AudioSource*> sources;
	std::vector<AudioListener*> listeners;
};

