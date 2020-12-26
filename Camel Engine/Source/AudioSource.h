#pragma once
#include "Globals.h"
#include "Component.h"

class GameObject;

class AudioSource : public Component {
public:
	AudioSource();
	AudioSource(GameObject* gameObject);
	~AudioSource();

	void Update() override;
	void OnEditor() override;

	/*void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	void SetPosition(float3 position);*/
	//float3 GetPosition();
	
	/*float* GetViewMatrix();
	float* GetProjectionMatrix();*/

public:

private:
	int volume;
};