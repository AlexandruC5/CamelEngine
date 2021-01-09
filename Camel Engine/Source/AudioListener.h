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


private:

	AkListenerPosition listener_position;
	const char* name;
	uint id;
};

#endif // !__AUDIOLISTENER_H__
