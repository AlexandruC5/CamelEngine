#include "AudioSource.h"
#include "ImGui/imgui.h"
#include "GameObject.h"

AudioSource::AudioSource(GameObject* parent)
{
	type = ComponentType::AUDIO_SOURCE;
	audio_name = "None";
	music_swap_time = 50.0f;
	priority = 128;
	volume = 50, pitch = 1, stereo_pan = 0, spatial_min_distance = 1, spatial_max_distance = 500;
	is_muted = false, play_on_awake = false, to_loop = false, is_stereo = false, is_mono = true, is_spatial = false;
}

AudioSource::~AudioSource()
{
}

void AudioSource::Update()
{
}

void AudioSource::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Enabled", &enabled);

		ImGui::Text("Audio File: %s", audio_name);

		if (ImGui::Checkbox("Play On Awake", &play_on_awake))
		{

		}
		if (ImGui::Checkbox("Loop", &to_loop))
		{

		}
		if (ImGui::SliderInt("Priority", &priority, 0, 256))
		{

		}
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
		{

		}
		if (ImGui::SliderFloat("Pitch", &pitch, -1.0f, 1.0f))
		{

		}
		if (ImGui::Checkbox("Stereo", &is_stereo))
		{
			ImGui::SliderFloat("Stereo Pan", &stereo_pan, -1.0f, 1.0f);

			is_mono = false;
			is_stereo = true;
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Mono", &is_mono))
		{
			is_mono = true;
			is_stereo = false;
		}

		if (ImGui::Checkbox("Spatial Audio", &is_spatial))
		{

		}
		if (ImGui::SliderFloat("Min distance", &spatial_min_distance, 1.0f, 10000.0f))
		{

		}
		if (ImGui::SliderFloat("Max distance", &spatial_max_distance, 1.0f, 10000.0f))
		{

		}
	}
}

const char* AudioSource::GetName()
{
	return audio_name;
}

const float3& AudioSource::GetSourcePosition()
{
	float3 ret;
	return ret;
}

const uint& AudioSource::GetID()
{
	return id;
}

const char* AudioSource::GetAudioBank()
{
	return nullptr;
}

void AudioSource::SetAudioBank(char* name)
{
}

const bool& AudioSource::GetMuted()
{
	return is_muted;
}

void AudioSource::SetMuted(bool& muted)
{
}

const bool& AudioSource::GetPlayOnAwake()
{
	return play_on_awake;
}

void AudioSource::SetPlayOnAwake(bool& _play_on_awake)
{
}

const bool& AudioSource::GetLoopActive()
{
	return to_loop;
}

void AudioSource::SetLoopActive(bool& on_loop)
{
}

const uint& AudioSource::GetPriority()
{
	return priority;
}

void AudioSource::SetPriority(uint& _priority)
{
}

const float& AudioSource::GetVolume()
{
	return volume;
}

void AudioSource::SetVolume(float& _volume)
{
}

const float& AudioSource::GetPitch()
{
	return pitch;
}

void AudioSource::SetPitch(float& pitch)
{
}

const bool& AudioSource::GetStereo()
{
	return is_stereo;
}

void AudioSource::SetStereo(bool& stereo)
{
}

const float& AudioSource::GetStereoPan()
{
	return stereo_pan;
}

void AudioSource::SetStereoPan(float& pan)
{
}

const bool& AudioSource::GetIsSpatial()
{
	return is_spatial;
}

void AudioSource::SetIsSpatial(bool& spatial)
{
}

const float& AudioSource::GetSpatialMaxDist()
{
	return spatial_max_distance;
}

void AudioSource::SetSpatialMaxDist(float& max_dist)
{
}

const float& AudioSource::GetSpatialMinDist()
{
	return spatial_min_distance;
}

void AudioSource::SetSpatialMinDistance(float& min_dist)
{
}

const uint& AudioSource::GetMusicSwapTime()
{
	return music_swap_time;
}

void AudioSource::SetMusicSwapTime(uint& swap_time)
{
}

const char* AudioSource::GetAudioToPlay()
{
	return audio_to_play;
}

void AudioSource::SetAudioToPlay(char* audio)
{
}

void AudioSource::PlayAudioByEvent(const char* name)
{
}

void AudioSource::PauseAudioByEvent(const char* name)
{
}

void AudioSource::ResumeAudioByEvent(const char* name)
{
}

void AudioSource::StopAudioByEvent(const char* name)
{
}
