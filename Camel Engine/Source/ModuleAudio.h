#pragma once
#include "Globals.h"
#include "Module.h"
#include "WwiseLibrary.h"

#include <vector>

class AudioSource;
class AudioListener;

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

	void LoadAudioBank(char* name);
	void UnLoadAudioBank(char* name);

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
	std::vector<AudioSource*> sources;
	std::vector<AudioListener*> listeners;
};

