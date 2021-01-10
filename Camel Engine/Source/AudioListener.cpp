#include "AudioListener.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "Transform.h"
#include "GameObject.h"

AudioListener::AudioListener(GameObject* game_object)
{
	enabled = true;
	_gameObject = game_object;
	type = ComponentType::AUDIO_LISTENER;
	id = LCG().Int();
	name = new char[128];
	name = game_object->GetName();
	App->audio->AddListenerToList(this);
	AKRESULT  eResult = AK::SoundEngine::RegisterGameObj(id, name);
	if (eResult != AK_Success)
	{
		LOG_ERROR("Could not create a listener.");
	}
	SetDefaultListener();

	Update();
}

AudioListener::~AudioListener()
{
	AK::SoundEngine::UnregisterGameObj(id);
}

void AudioListener::Update()
{
	Transform* transform = _gameObject->GetTransform();
	if (transform != nullptr)
	{
		// Get trnasform values //
		math::float4x4 global_matrix = transform->GetGlobalTransform();
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

		SetListenerPos(position.x, position.y, position.z, front_rotation.x, front_rotation.y, front_rotation.z, top_rotation.x, top_rotation.y, top_rotation.z);
		//LOG("GO Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);
		//LOG("AK Position: %.2f, %.2f, %.2f", listener_position.Position().X, listener_position.Position().Y, listener_position.Position().Z);


	}
}

void AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox(" Enabled", &enabled)) {}
	}
}

const char* AudioListener::GetName() const
{
	return name;
}

const uint& AudioListener::GetID() const
{
	return id;
}

void AudioListener::SetDefaultListener()
{
	AkGameObjectID tmp = id;
	AKRESULT eResult = AK::SoundEngine::SetDefaultListeners(&tmp, 1);
	if (eResult != AK_Success)
	{
		LOG_ERROR("Could not set GameObject as default listener.");
		return;
	}
	App->audio->SetListener(this);
}

void AudioListener::SetListenerPos(float pos_x, float pos_y, float pos_z, float front_rot_x, float front_rot_y, float front_rot_z, float top_rot_x, float top_rot_y, float top_rot_z)
{

	ak_position.X = pos_x;
	ak_position.Y = pos_y;
	ak_position.Z = pos_z;

	ak_front_rotation.X = front_rot_x;
	ak_front_rotation.Y = front_rot_y;
	ak_front_rotation.Z = front_rot_z;

	ak_top_rotation.X = top_rot_x;
	ak_top_rotation.Y = top_rot_y;
	ak_top_rotation.Z = top_rot_z;

	listener_position.Set(ak_position, ak_front_rotation, ak_top_rotation);
	AK::SoundEngine::SetPosition(id, listener_position);

}

void AudioListener::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_object.AddString("Name", name);

	save_array.AddObject(save_object);
}

void AudioListener::Load(GnJSONObj& load_object)
{
	name = (char*)load_object.GetString("Name", "");
}
