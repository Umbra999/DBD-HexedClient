#include "Patching.hpp"
#include "../minhook/MinHook.h"
#include "../Wrapper/Logger.hpp"

void Patching::ApplyPatches()
{
	MH_Initialize();

	// Patches here

	MH_EnableHook(MH_ALL_HOOKS);
}

void Patching::UnapplyPatches()
{
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}