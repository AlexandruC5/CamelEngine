#include "AudioSource.h"
#include "ImGui/imgui.h"
#include "GameObject.h"

AudioSource::AudioSource(GameObject* gameObject)
{
	type = ComponentType::AUDIO_SOURCE;
	volume = 50;
	to_loop = false;
	is_spatial = false;
	audio_path = "Hello";
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
		ImGui::Text("Audio File: %s", audio_path.c_str());

		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 100.0f))
		{

		}
		if (ImGui::Checkbox("Loop", &to_loop))
		{

		}
		if (ImGui::Checkbox("Spatial", &is_spatial))
		{

		}

	}
}
