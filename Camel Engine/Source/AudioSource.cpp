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
	reference = App->audio->banks[0];
	audio_to_play = new char[256];
	name = parent->GetName();
	audio_to_play = "";
	music_swap_time = 100.0f;
	volume = 0.5, pitch = 0, stereo_pan = 0, spatial_min_distance = 1, spatial_max_distance = 500;
	is_muted = false, play_on_awake = true, to_loop = false, is_stereo = false, is_mono = true, is_spatial = false;
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
	if (audio_to_play != nullptr)
		StopAudioByEvent(audio_to_play);
	AK::SoundEngine::UnregisterGameObj(id);
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

		SetSourcePos(position.x, position.y, position.z, front_rotation.x, front_rotation.y, front_rotation.z, top_rotation.x, top_rotation.y, top_rotation.z);
	}


}

void AudioSource::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(" Enabled", &enabled);

		if (ImGui::BeginCombo("Bank", reference->bank_name.c_str()))
		{
			std::vector<Bank*>::const_iterator it;
			for (it = App->audio->banks.begin(); it != App->audio->banks.end(); ++it)
			{
				bool is_selected = (reference == (*it));
				if (ImGui::Selectable((*it)->bank_name.c_str()))
				{
					reference = (*it);
					App->audio->LoadAudioBank(reference->bank_name.c_str());
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if(ImGui::BeginCombo("Audio Event", audio_to_play))
		{
			std::map<uint64, std::string>::const_iterator ev_it;
			for (ev_it = reference->events.begin(); ev_it != reference->events.end(); ++ev_it)
			{
				if (!(*ev_it).second.find("Play"))
				{
					bool is_selected = (audio_to_play == (*ev_it).second.c_str());
					if (ImGui::Selectable((*ev_it).second.c_str()))
					{
						audio_to_play = (char*)(*ev_it).second.c_str();
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

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

		GetIsSpatial();
		if (ImGui::SliderInt("Spatial Audio", &position, 0, 1)) 
		{
			SetIsSpatial(position);
		}

		if (position == 0) {

			if (ImGui::Checkbox("Stereo", &is_stereo))
			{
				is_stereo = true;
				is_mono = false;

				SetStereo(is_stereo);
			}

			if (is_stereo) {


				if (ImGui::SliderFloat("Stereo Pan", &stereo_pan, -1.0f, 1.0f)) {

					SetStereoPan(stereo_pan);
				}
			}

			GetStereo();
			if (ImGui::Checkbox("Mono", &is_mono))
			{
				is_mono = true;
				is_stereo = false;
				SetStereo(is_stereo);
			}
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
	save_object.AddString("Name", name);
	save_object.AddString("Audio Source", audio_to_play);
	save_object.AddInt("Music Swap Time", music_swap_time);
	save_object.AddFloat("Volume", volume);
	save_object.AddFloat("Pitch", pitch);
	save_object.AddFloat("Stereo Pan", stereo_pan);
	save_object.AddFloat("Spatial Min Distance", spatial_min_distance);
	save_object.AddFloat("Spatial Max Distance", spatial_max_distance);
	save_object.AddBool("Muted", is_muted);
	save_object.AddBool("Play On Awake", play_on_awake);
	save_object.AddBool("Loop", to_loop);
	save_object.AddBool("Stereo", is_stereo);
	save_object.AddBool("Mono", is_mono);
	save_object.AddBool("Spatial", is_spatial);

	save_array.AddObject(save_object);
}

void AudioSource::Load(GnJSONObj& load_object)
{
	name = (char*)load_object.GetString("Name", "");
	audio_to_play = (char*)load_object.GetString("Audio Source", "");
	music_swap_time = load_object.GetInt("Music Swap Time");
	volume = load_object.GetFloat("Volume");
	pitch = load_object.GetFloat("Pitch");
	stereo_pan = load_object.GetFloat("Stereo Pan");
	spatial_min_distance = load_object.GetFloat("Spatial Min Distance");
	spatial_max_distance = load_object.GetFloat("Spatial Max Distance");
	is_muted = load_object.GetBool("Muted");
	play_on_awake = load_object.GetBool("Play On Awake");
	to_loop = load_object.GetBool("Loop");
	is_stereo = load_object.GetBool("Stereo");
	is_mono = load_object.GetBool("Mono");
	is_spatial = load_object.GetBool("Spatial");
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
	pitch = _pitch;
	AK::SoundEngine::SetRTPCValue("Pitch", pitch, id);
}

const bool& AudioSource::GetStereo()
{
	return is_stereo;
}

void AudioSource::SetStereo(bool& stereo)
{
	is_stereo = stereo;
	AkChannelConfig config;

	if (stereo) {
		config.SetStandard(AK_SPEAKER_SETUP_STEREO);
	}
	else {
		config.SetStandard(AK_SPEAKER_SETUP_MONO);
	}

	AK::SoundEngine::SetBusConfig(id, config);
	config.Clear();
}

const float& AudioSource::GetStereoPan()
{
	return stereo_pan;
}

void AudioSource::SetStereoPan(float& pan)
{
	stereo_pan = pan;
	AK::SoundEngine::SetRTPCValue("Pan", stereo_pan, id);
}

const bool& AudioSource::GetIsSpatial()
{
	return position;
}

void AudioSource::SetIsSpatial(int& pos)
{
	AK::SoundEngine::SetRTPCValue("Position", position, id);
	position = pos;
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
	if (this->enabled)
		AK::SoundEngine::PostEvent(name, id);
}

void AudioSource::PauseAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Pause);
}

void AudioSource::ResumeAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Resume);
}

void AudioSource::StopAudioByEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Stop);
}

void AudioSource::SetSourcePos(float x, float y,float z, float x_front, float y_front, float z_front, float x_top, float y_top, float z_top)
{

	//Reciving values
	ak_position.X = x;
	ak_position.Y = y;
	ak_position.Z = Z;

	ak_front_rotation.X = x_front;
	ak_front_rotation.Y = y_front;
	ak_front_rotation.Z = z_front;

	ak_top_rotation.X = x_top;
	ak_top_rotation.Y = y_top;
	ak_top_rotation.Z = z_top;

	//Creating normalization values
	float length_front = sqrt(pow(ak_front_rotation.X, 2) + pow(ak_front_rotation.Y, 2) + pow(ak_front_rotation.Z, 2));
	float lenght_top = sqrt(pow(ak_top_rotation.X, 2) + pow(ak_top_rotation.Y, 2) + pow(ak_top_rotation.Z, 2));


	//Vector normalization
	ak_front_rotation.X = ak_front_rotation.X / length_front;
	ak_front_rotation.Y = ak_front_rotation.Y / length_front;
	ak_front_rotation.Z = ak_front_rotation.Z / length_front;

	ak_top_rotation.X = ak_top_rotation.X / lenght_top;
	ak_top_rotation.Y = ak_top_rotation.Y / lenght_top;
	ak_top_rotation.Z = ak_top_rotation.Z / lenght_top;

	//Checking if there are orthogonals

	float dot_prod = ak_top_rotation.X * ak_front_rotation.X + ak_top_rotation.Y * ak_front_rotation.Y + ak_top_rotation.Z * ak_front_rotation.Z;

	if (dot_prod >= 0.0001)
		assert(!"Vectors are not orthogonal!");


	//Updating the position of the GameObject with the audioSource
	AkSoundPosition sound_pos;
	sound_pos.Set(ak_position, ak_front_rotation, ak_top_rotation);

	AKRESULT res = AK::SoundEngine::SetPosition((AkGameObjectID)id, sound_pos);
	if (res != AK_Success)
		assert(!"Something went wrong, check the res variables for more info.");

}

void AudioSource::ChangeEvent(const char* event_name)
{
	AK::SoundEngine::PostEvent(event_name, id);
}
