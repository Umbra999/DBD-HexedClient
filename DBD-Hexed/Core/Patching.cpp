#include "Patching.hpp"
#include "../minhook/MinHook.h"
#include "../Wrapper/Logger.hpp"
#include "../Core/Handler.hpp"

void PatchedPostRender(UGameViewportClient* UGameViewportClient, Canvas* canvas)
{
	Handler::OnRender(UGameViewportClient, canvas);
	OPostRender(UGameViewportClient, canvas);
}

void Patching::PatchPostRenderer()
{
	UWorld* World = *(UWorld**)(WRLD);
	if (!World) return;

	UGameInstance* OwningGameInstance = World->OwningGameInstance;
	if (!OwningGameInstance) return;

	TArray<UPlayer*>LocalPlayers = OwningGameInstance->LocalPlayers;

	UPlayer* LocalPlayer = LocalPlayers[0];
	if (!LocalPlayer) return;

	UGameViewportClient* ViewPortClient = LocalPlayer->ViewportClient;
	if (!ViewPortClient) return;

	void** ViewPortClientVTable = ViewPortClient->VFTable;
	if (!ViewPortClientVTable) return;

	DWORD protecc;
	VirtualProtect(&ViewPortClientVTable[102], 8, PAGE_EXECUTE_READWRITE, &protecc);
	OPostRender = reinterpret_cast<decltype(OPostRender)>(ViewPortClientVTable[102]);
	ViewPortClientVTable[102] = &PatchedPostRender;
	VirtualProtect(&ViewPortClientVTable[102], 8, protecc, 0);
}

void Patching::ApplyPatches()
{
	MH_Initialize();

	if (!EngineInit())
	{
		Logger::LogError("Failed to Initialize Engine");
		return;
	}

	PatchPostRenderer();

	MH_EnableHook(MH_ALL_HOOKS);
}

void Patching::UnapplyPatches()
{
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}