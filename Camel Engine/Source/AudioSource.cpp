#include "Application.h"
#include "AudioSource.h"
#include "ImGui/imgui.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModuleAudio.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

AudioSource::AudioSource(GameObject* parent)
{
	type = ComponentType::AUDIO_SOURCE;
	
	id = LCG().Int();
	audio_to_play = new char[256];
	name = parent->GetName();
	audio_to_play = (char*)App->audio->banks[0]->audios[811455978].c_str();
	music_swap_time = 5.0f;
	priority = 128;
	volume = 0.5, pitch = 1, stereo_pan = 0, spatial_min_distance = 1, spatial_max_distance = 500;
	is_muted = false, play_on_awake = false, to_loop = false, is_stereo = false, is_mono = true, is_spatial = false;
	_gameObject = parent;

	AKRESULT eResult = AK::SoundEngine::RegisterGameObj(id, name);
	if (eResult != AK_Success)
	{
		assert(!"Could not register GameObject. See eResult variable to more info");
		LOG("Could not register GameObject. See eResult variable to more info");
	}

	App->audio->AddSourceToList(this);

	SetAudioToPlay(audio_to_play);
}

AudioSource::~AudioSource()
{
	RELEASE_ARRAY(name);
}

void AudioSource::Update()
{
	Transform* trans = (Transform*)_gameObject->GetComponent(ComponentType::TRANSFORM);

	if (trans != nullptr)
	{
		// Get trnasform values //
		math::float4x4 global_matrix = trans->GetGlobalTransform();
		math::float3 position;
		math::float3 front_rotation;
		math::float3 top_rotation;
		math::float3 scale;
		math::Quat full_rotation;

		global_matrix.Decompose(position, full_rotation, scale);

		front_rotation = full_rotation * math::float3{ 0,0,1 };
		top_rotation = full_rotation * math::float3{ 0,1,0 };

		front_rotation.Normalize();
		top_rotation.Normalize();

		// Apply values to listener //
		AkVector ak_position, ak_front_rotation, ak_top_rotation;

		ak_position.X = position.x;
		ak_position.Y = position.y;
		ak_position.Z = position.z;

		ak_front_rotation.X = front_rotation.x;
		ak_front_rotation.Y = front_rotation.y;
		ak_front_rotation.Z = front_rotation.z;

		ak_top_rotation.X = top_rotation.x;
		ak_top_rotation.Y = top_rotation.y;
		ak_top_rotation.Z = top_rotation.z;

		source_pos.Set(ak_position, ak_front_rotation, ak_top_rotation);

		AK::SoundEngine::SetPosition(id, source_pos);
	}


}

void AudioSource::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Enabled", &enabled);

		GetAudioToPlay();
		ImGui::Text("Audio File: %s", audio_to_play);

		GetMuted();
		if (ImGui::Checkbox("Muted", &is_muted))
		{
			SetMuted(is_muted);
		}

		GetPlayOnAwake();
		if (ImGui::Checkbox("Play On Awake", &play_on_awake))
		{
			SetPlayOnAwake(play_on_awake);
		}
		
		GetLoopActive();
		if (ImGui::Checkbox("Loop", &to_loop))
		{
			//SetLoopActive(to_loop);
		}
		
		GetPriority();
		if (ImGui::SliderInt("Priority", &priority, 0, 256))
		{
			SetPriority(priority);
		}
		
		GetVolume();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
		{
			SetVolume(volume);
		}
		
		GetPitch();
		if (ImGui::SliderFloat("Pitch", &pitch, -1.0f, 1.0f))
		{
			SetPitch(pitch);
		}
		
		GetStereo();
		if (ImGui::Checkbox("Stereo", &is_stereo))
		{
			ImGui::SliderFloat("Stereo Pan", &stereo_pan, -1.0f, 1.0f);

			is_mono = false;
			is_stereo = true;
			SetStereo(is_stereo);
		}

		ImGui::SameLine();

		GetStereo();
		if (ImGui::Checkbox("Mono", &is_mono))
		{
			is_mono = true;
			is_stereo = false;
			SetStereo(is_stereo);
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

void AudioSource::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;

	save_object.AddInt("Type", type);
	save_object.AddBool("Muted", is_muted);
	save_object.AddBool("Play On Awake", play_on_awake);
	save_object.AddBool("Loop", to_loop);

	//is_muted, play_on_awake, to_loop, is_stereo, is_mono, is_spatial

	save_array.AddObject(save_object);
}

void AudioSource::Load(GnJSONObj& load_object)
{
	is_muted = load_object.GetBool("Muted");
	play_on_awake = load_object.GetBool("Play On Awake");
	to_loop = load_object.GetBool("Loop");
}

const char* AudioSource::GetName()
{
	return name;
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

const bool& AudioSource::GetMuted()
{
	return is_muted;
}

void AudioSource::SetMuted(bool& muted)
{
	if (muted) {
		AK::SoundEngine::SetGameObjectOutputBusVolume(id, AK_INVALID_GAME_OBJECT, 0);
	}
	else {
		AK::SoundEngine::SetGameObjectOutputBusVolume(id, AK_INVALID_GAME_OBJECT, volume);
	}
	is_muted = muted;
}

const bool& AudioSource::GetPlayOnAwake()
{
	return play_on_awake;
}

void AudioSource::SetPlayOnAwake(bool& _play_on_awake)
{
	play_on_awake = _play_on_awake;
}

const bool& AudioSource::GetLoopActive()
{
	return to_loop;
}

void AudioSource::SetLoopActive(bool& on_loop)
{
	to_loop = on_loop;
}

const int& AudioSource::GetPriority()
{
	return priority;
}

void AudioSource::SetPriority(int& _priority)
{
	AK::SoundEngine::SetRTPCValue("Priority", _priority, AK_INVALID_GAME_OBJECT);
	priority = _priority;
}

const float& AudioSource::GetVolume()
{
	return volume;
}

void AudioSource::SetVolume(float& _volume)
{
	AK::SoundEngine::SetGameObjectOutputBusVolume(id, AK_INVALID_GAME_OBJECT, _volume);
	volume = _volume;
}

const float& AudioSource::GetPitch()
{
	return pitch;
}

void AudioSource::SetPitch(float& _pitch)
{
	AK::SoundEngine::SetRTPCValue("Pitch", _pitch, id);
	pitch = _pitch;
}

const bool& AudioSource::GetStereo()
{
	return is_stereo;
}

void AudioSource::SetStereo(bool& stereo)
{
	AkChannelConfig config;

	if (stereo) {
		config.SetStandard(AK_SPEAKER_SETUP_STEREO);
	}
	else {
		config.SetStandard(AK_SPEAKER_SETUP_MONO);
	}

	AK::SoundEngine::SetBusConfig(id, config);
	config.Clear();

	is_stereo = stereo;
}

const float& AudioSource::GetStereoPan()
{
	return stereo_pan;
}

void AudioSource::SetStereoPan(float& pan)
{
	AK::SoundEngine::SetRTPCValue("StereoPan", pan, id);
	stereo_pan = pan;
}

const bool& AudioSource::GetIsSpatial()
{
	return is_spatial;
}

void AudioSource::SetIsSpatial(bool& spatial)
{
	is_spatial = spatial;
}

const float& AudioSource::GetSpatialMaxDist()
{
	return spatial_max_distance;
}

void AudioSource::SetSpatialMaxDist(float& max_dist)
{
	spatial_max_distance = max_dist;
}

const float& AudioSource::GetSpatialMinDist()
{
	return spatial_min_distance;
}

void AudioSource::SetSpatialMinDistance(float& min_dist)
{
	spatial_min_distance = min_dist;
}

const uint& AudioSource::GetMusicSwapTime()
{
	return music_swap_time;
}

void AudioSource::SetMusicSwapTime(uint& swap_time)
{
	music_swap_time = swap_time;
}

const char* AudioSource::GetAudioToPlay()
{
	return audio_to_play;
}

void AudioSource::SetAudioToPlay(char* audio)
{
	audio_to_play = audio;
}

void AudioSource::PlayAudioByEvent(const char* name)
{
	AK::SoundEngine::PostEvent(name, id);
}

void AudioSource::PauseAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Pause, id);
}

void AudioSource::ResumeAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Resume);
}

void AudioSource::StopAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Stop);
}

void AudioSource::ChangeState(const char* general_state, const char* sub_state)
{
	//AKRESULT res = AK::SoundEngine::SetSwitch(general_state, sub_state, id);
	AKRESULT res = AK::SoundEngine::SetState(general_state, sub_state);
	if (res != AK_Success)
	{
		LOG_WARNING("Couldn't change between music states")
	}
}
