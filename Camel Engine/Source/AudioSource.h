#pragma once
#include "Globals.h"
#include "Component.h"
#include "WwiseLibrary.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "GnJSON.h"
#include "ModuleAudio.h"
#include <string>

class GameObject;
class AudioEvent;

class AudioSource : public Component {
public:
	AudioSource(GameObject* parent);
	~AudioSource();

	void Update() override;
	void OnEditor() override;

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	const char* GetName();
	const float3& GetSourcePosition();
	const uint& GetID();

	const char* GetAudioBank();

	const bool& GetMuted();
	void SetMuted(bool& muted);
	
	const bool& GetPlayOnAwake();
	void SetPlayOnAwake(bool& _play_on_awake);


	const float& GetVolume();
	void SetVolume(float& _volume);

	const float& GetPitch();
	void SetPitch(float& _pitch);

	const bool& GetStereo();
	void SetStereo(bool& stereo);

	const float& GetStereoPan();
	void SetStereoPan(float& pan);

	const bool& GetIsSpatial();
	void SetIsSpatial(int& pos);

	const float& GetSpatialMaxDist();
	void SetSpatialMaxDist(float& max_dist);

	const float& GetSpatialMinDist();
	void SetSpatialMinDistance(float& min_dist);

	const uint& GetMusicSwapTime();
	void SetMusicSwapTime(uint& swap_time);

	const char* GetAudioToPlay();
	void SetAudioToPlay(char* audio);

	void PlayAudioByEvent(const char* name);
	void PauseAudioByEvent(const char* name);
	void ResumeAudioByEvent(const char* name);
	void StopAudioByEvent(const char* name);
	void SetSourcePos(float x, float y,float z, float x_front, float y_front, float z_front, float x_top, float y_top, float z_top);
	void ChangeEvent(const char* name);

private:

	const char* name;
	char* audio_to_play;
	AkSoundPosition source_pos;
	uint id, music_swap_time;
	Bank* reference;

	int position;
	float volume, pitch, stereo_pan, spatial_min_distance, spatial_max_distance;
	bool is_muted, play_on_awake, is_stereo, is_mono, is_spatial;

public:
	AkVector ak_position, ak_front_rotation, ak_top_rotation;

};