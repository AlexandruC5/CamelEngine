#include "WwiseLibrary.h"

#include "AK/SoundEngine/Common/AkMemoryMgr.h"                  // Memory Manager interface
#include "AK/SoundEngine/Common/AkModule.h"                     // Default memory manager
#include "AK/SoundEngine/Common/IAkStreamMgr.h"                 // Streaming Manager
#include "AK/Tools/Common/AkPlatformFuncs.h"
#include "AK/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "AK/MusicEngine/Common/AkMusicEngine.h"                // Music Engine
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"				// Spatial audio

#include "AK/Plugin/AkRoomVerbFXFactory.h"

#include <assert.h>

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

namespace AK
{
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}

	void* VirtualAllocHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
}

bool InitSoundEngine()
{
    AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        assert(!"Could not create the memory manager.");
        return false;
    }

	// Init streaming manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.
	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		LOG("Could not create the Streaming Manager");
		return false;
	}
	
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		LOG("Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	// Init sound engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		LOG("Could not initialize the Sound Engine.");
		return false;
	}

	//Init music engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		LOG("Could not initialize the Music Engine.");
		return false;
	}

	////Init spatial audio engine
	//AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	//if (AK::SpatialAudio::Init(&settings) != AK_Success)
	//{
	//	assert(!"Could not initialize the Spatial Audio.");
	//	return false;
	//}

#ifndef AK_OPTIMIZED
//
// Initialize communications (not in release build!)
//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}

#endif // AK_OPTIMIZED

	return true;
}

void ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}

bool TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif 

	// Terminate the spatial audio
	//AK::SpatialAudio::Term();

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();

	// Terminate IO device
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	return true;
}
