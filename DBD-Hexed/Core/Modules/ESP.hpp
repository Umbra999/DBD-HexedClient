#pragma once
#include "../../SDK/Engine.hpp"

class ESP
{
private:
	static void RenderSurvivorESP(Canvas*, APlayerState*, APawn*, APlayerController*);
	static void RenderKillerESP(Canvas*, APlayerState*, APawn*, APlayerController*);
	static void RenderGeneratorESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderGateESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderHatchESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderTotemESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderChestESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderPalletESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderWindowESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderBreakableESP(Canvas*, AActor*, APawn*, APlayerController*);
	static void RenderCollectableESP(Canvas*, AActor*, APawn*, APlayerController*);

public:
	static void OnRender(UGameViewportClient*, Canvas*);
};
