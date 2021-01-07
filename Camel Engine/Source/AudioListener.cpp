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
	type = ComponentType::AUDIO_SOURCE;
	id = LCG().Int();
	name = new char[128];
	name = "listener";
	App->audio->AddListenerToList(this);
	AKRESULT  eResult = AK::SoundEngine::RegisterGameObj(id, name);
	if (eResult != AK_Success)
	{
		LOG_ERROR("Could not create a listener.");
	}

	Update();
}

AudioListener::~AudioListener()
{
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

		listener_position.Set(ak_position, ak_front_rotation, ak_top_rotation);

		AK::SoundEngine::SetPosition(id, listener_position);
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
