#pragma once
#ifndef __AUDIOLISTENER_H__
#define __AUDIOLISTENER_H__

#include "Component.h"
#include "WwiseLibrary.h"
#include "Globals.h"

class AudioListener : public Component
{
public:
	AudioListener(GameObject* game_object);
	~AudioListener();

	void Update() override;
	void OnEditor();

	const char* GetName() const;
	const uint& GetID() const;

	void SetDefaultListener();

	void SetListenerPos(float pos_x, float pos_y, float pos_z, float front_rot_x, float front_rot_y, float front_rot_z, float top_rot_x, float top_rot_y, float top_rot_z);


private:

	AkListenerPosition listener_position;
	const char* name;
	uint id;


public:
	AkVector ak_position, ak_front_rotation, ak_top_rotation;

};

#endif // !__AUDIOLISTENER_H__
