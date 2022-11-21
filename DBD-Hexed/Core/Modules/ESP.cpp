#include "ESP.hpp"
#include "../InternalSettings.hpp"

void ESP::OnRender(UGameViewportClient* UGameViewportClient, Canvas* canvas)
{
	UWorld* World = *(UWorld**)(WRLD);
	if (!World) return;

	UGameInstance* OwningGameInstance = World->OwningGameInstance;
	if (!OwningGameInstance) return;

	ULevel* PersistentLevel = World->PersistentLevel;
	if (!PersistentLevel) return;

	AGameStateBase* GameState = World->GameState;
	if (!GameState) return;


	TArray<AActor*> Actors = PersistentLevel->Actors;

	TArray<UPlayer*> LocalPlayers = OwningGameInstance->LocalPlayers;

	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;

	UPlayer* LocalPlayer = LocalPlayers[0];
	if (!LocalPlayer) return;

	APlayerController* PlayerController = LocalPlayer->PlayerController;
	if (!PlayerController) return;

	int width = 0;
	int height = 0;

	PlayerController->GetViewportSize(width, height);

	APawn* MyPlayer = PlayerController->AcknowledgedPawn;
	if (!MyPlayer) return;

	APlayerState* PlayerState = MyPlayer->PlayerState;
	if (!PlayerState) return;

	for (auto i = 0; i < PlayerArray.Num(); i++)
	{
		if (!Actors.IsValidIndex(i)) continue;
		APlayerState* Player = PlayerArray[i];
		if (!Player || Player == MyPlayer->PlayerState) continue;

		if (InternalSettings::SurvivorESP) RenderSurvivorESP(canvas, Player, MyPlayer, PlayerController);
		if (InternalSettings::KillerESP) RenderKillerESP(canvas, Player, MyPlayer, PlayerController);
	}

	for (auto i = 0; i < Actors.Num(); i++)
	{
		if (!Actors.IsValidIndex(i)) continue;
		AActor* Actor = Actors[i];
		if (!Actor || Actor == MyPlayer) continue;

		if (InternalSettings::GeneratorESP) RenderGeneratorESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::GatesESP) RenderGateESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::HatchESP) RenderHatchESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::TotemESP) RenderTotemESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::ChestESP) RenderChestESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::PalletESP) RenderPalletESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::WindowESP) RenderWindowESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::BreakableESP) RenderBreakableESP(canvas, Actor, MyPlayer, PlayerController);
		if (InternalSettings::ItemESP) RenderCollectableESP(canvas, Actor, MyPlayer, PlayerController);
	}
}

void ESP::RenderSurvivorESP(Canvas* canvas, APlayerState* Player, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (Player->PawnPrivate->GetPlayerTeam() != 2) return;

	FVector2D ScreenLocation;
	FVector WorldLocation = Player->PawnPrivate->Instigator->RootComponent->RelativeLocation;

	if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Player->PawnPrivate->GetDistanceTo(MyPlayer) / 100));
	std::string name = Player->PlayerNamePrivate.ToString();

	std::string text = name + "[" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.18f, 0.91f, 0.23f, 1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderKillerESP(Canvas* canvas, APlayerState* Player, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (Player->PawnPrivate->GetPlayerTeam() != 1) return;

	FVector2D ScreenLocation;
	FVector WorldLocation = Player->PawnPrivate->Instigator->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Player->PawnPrivate->GetDistanceTo(MyPlayer) / 100));
	std::string name = Player->PlayerNamePrivate.ToString();

	std::string text = name + "[" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.64f,0.f,0.01f, 1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderGeneratorESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(AGeneratorClass)) return;

	AGenerator* Gen = (AGenerator*)Actor;

	if (Gen->IsRepaired) return;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string percentFin = std::to_string((int)(Gen->percent_complete * 100));
	std::string dist = std::to_string((int)(Gen->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Generator [" + percentFin + "%] [" + dist + "m]";

	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.0570f, 0.578f, 0.950f , 1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderGateESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(AEscapeDoorClass)) return;

	AEscapeDoor* Gate = (AEscapeDoor*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Gate->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Gate [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.73f,0.3f,1.f ,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderHatchESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(AHatchClass)) return;

	AHatch* Hatch = (AHatch*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Hatch->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Hatch [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.83f,0.82f,0.f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderTotemESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(ATotemClass)) return;

	ATotem* Totem = (ATotem*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Totem->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Totem [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.98f,0.98f,0.98f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderChestESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(ASearchableClass)) return;

	ASearchable* Chest = (ASearchable*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Chest->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Chest [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.44f,0.57f,0.62f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderPalletESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(APalletClass)) return;

	APallet* Pallet = (APallet*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Pallet->GetDistanceTo(MyPlayer) / 100));

	std::string status = "Up";
	if (Pallet->GetIsPulledDown()) status = "Down";
	else if (Pallet->IsPulldownBlockedByEntity()) status = "Blocked";
	else if (Pallet->GetIsDreamPallet()) status = "Dream";

	std::string text = "Pallet [" + status + "] [" + dist + "m]";

	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.83f,0.82f,0.f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderWindowESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(AWindowClass)) return;

	AWindow* Window = (AWindow*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Window->GetDistanceTo(MyPlayer) / 100));
	std::string status = "Open"; // needs check if blocked

	std::string text = "Window [" + status + "] [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.44f,0.57f,0.62f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });


}

void ESP::RenderBreakableESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(ABreakableClass)) return;

	ABreakableBase* Breakable = (ABreakableBase*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	if (!Breakable->IsUnbroken()) return;

	std::string dist = std::to_string((int)(Breakable->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Breakable [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.44f,0.57f,0.62f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}

void ESP::RenderCollectableESP(Canvas* canvas, AActor* Actor, APawn* MyPlayer, APlayerController* PlayerController)
{
	if (!Actor->IsA(ACollectableClass)) return;

	ACollectable* Collectable = (ACollectable*)Actor;

	FVector2D ScreenLocation;
	FVector WorldLocation = Actor->RootComponent->RelativeLocation;

	if (!PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return;
	if (!ScreenLocation.X && !ScreenLocation.Y) return;

	std::string dist = std::to_string((int)(Collectable->GetDistanceTo(MyPlayer) / 100));

	std::string text = "Collectable [" + dist + "m]";
	std::wstring rendertext(text.begin(), text.end());

	canvas->K2_DrawText(FString(rendertext.c_str()), ScreenLocation, FVector2D{ 1.0f,1.0f }, FLinearColor{ 0.44f,0.57f,0.62f,1.f }, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
}