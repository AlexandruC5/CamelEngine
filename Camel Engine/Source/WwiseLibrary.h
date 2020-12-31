#pragma once
#include "Globals.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/IBytes.h"
#include "AK/SoundEngine/Common/AkTypes.h"

#ifndef _DEBUG

#define AK_OPTIMIZED
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkSpatialAudio.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkRoomVerbFX.lib")

#else
#include "AK/Comm/AkCommunication.h"
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/CommunicationCentral.lib")
#pragma comment( lib, "AK/ws2_32.lib") 
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkSpatialAudio.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkStreamMgr.lib")
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkRoomVerbFX.lib")

#endif // !_DEBUG

#pragma comment( lib, "AK/dinput8.lib" )
#pragma comment( lib, "AK/dsound.lib" ) 
#pragma comment( lib, "AK/dxguid.lib" )

namespace AK
{
	void* AllocHook(size_t in_size);
	void FreeHook(void* in_ptr);

	void* VirtualAllocHook(void* in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void* in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
}

bool InitSoundEngine();
void ProcessAudio();
bool TermSoundEngine();