#pragma once
#include "Globals.h"
#include "Module.h"
#include "WwiseLibrary.h"

#include <list>



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

	void PlayOnAwake()const;
	void StopAudio()const;
	void PauseAudio()const;
	void ResumeAudio()const;
	void SetListener(AudioListener* new_listener);

	void AddListenerToList(AudioListener* listener);
	void AddSourceToList(AudioSource* source);

private:
	AudioListener* listener;
	bool isAudioPlayed = false;

public:
	std::list<AudioSource*> sources;
	std::list<AudioListener*> listeners;
};

