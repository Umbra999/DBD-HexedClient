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

public:
	static void OnRender(UGameViewportClient*, Canvas*);
};
