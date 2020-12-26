#include "AudioSource.h"/*
#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/IBytes.h"*/

AudioSource::AudioSource()
{
	type = ComponentType::AUDIO_SOURCE;
	volume = 0;
}

AudioSource::AudioSource(GameObject* gameObject)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::Update()
{
}

void AudioSource::OnEditor()
{
}
