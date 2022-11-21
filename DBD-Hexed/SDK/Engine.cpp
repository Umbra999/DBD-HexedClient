#include "Engine.hpp"
#include "../Wrapper/Utils.hpp"
#include <Psapi.h>


std::string FNameEntry::String()
{
	if (bIsWide) { return std::string(); }
	return { AnsiName, Len };
}

FNameEntry* FNamePool::GetEntry(FNameEntryHandle handle) const
{
	return reinterpret_cast<FNameEntry*>(Blocks[handle.Block] + 4 * static_cast<uint64_t>(handle.Offset));
}

std::string FName::GetName()
{
	auto entry = NamePoolData->GetEntry(Index);
	auto name = entry->String();
	if (Number > 0)
	{
		name += '_' + std::to_string(Number);
	}
	auto pos = name.rfind('/');
	if (pos != std::string::npos)
	{
		name = name.substr(pos + 1);
	}
	return name;
}

std::string UObject::GetName()
{
	return NamePrivate.GetName();
}

std::string UObject::GetFullName()
{
	std::string name;
	for (auto outer = OuterPrivate; outer; outer = outer->OuterPrivate) { name = outer->GetName() + "." + name; }
	name = ClassPrivate->GetName() + " " + name + this->GetName();
	return name;
}

bool UObject::IsA(void* cmp)
{
	for (auto super = ClassPrivate; super; super = static_cast<UClass*>(super->SuperStruct)) { if (super == cmp) { return true; } }
	return false;
}

UObject* TUObjectArray::GetObjectPtr(uint32_t id) const
{
	if (id >= NumElements) return nullptr;
	uint64_t chunkIndex = id / 65536;
	if (chunkIndex >= NumChunks) return nullptr;
	auto chunk = Objects[chunkIndex];
	if (!chunk) return nullptr;
	uint32_t withinChunkIndex = id % 65536 * 24;
	auto item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
	return item;
}

UObject* TUObjectArray::FindObject(const char* name) const
{
	for (auto i = 0u; i < NumElements; i++)
	{
		auto object = GetObjectPtr(i);
		if (object && object->GetFullName() == name) { return object; }
	}
	return nullptr;
}

void UObject::ProcessEvent(void* UFunction, void* Params)
{
	auto vtable = *reinterpret_cast<void***>(this);
	reinterpret_cast<void(*)(void*, void*, void*)>(vtable[68])(this, UFunction, Params);
}

FNamePool* NamePoolData = nullptr;
TUObjectArray* ObjObjects = nullptr;
UWorld* WRLD = nullptr;

UObject* WorldToScreenUFunc;
UObject* GetViewportSizeUFunc;
UObject* GetPlayerNameUFunc;
UObject* GetBoneNameUFunc;
UObject* SetControlRotationUFunc;
UObject* K2_DrawLineUFunc;
UObject* K2_DrawTextUFunc;
UObject* LineOfSightToUFunc;
UObject* EnemyClass;
UObject* Font;
UObject* GetPlayerTeamUFunc;
UObject* AGeneratorClass;
UObject* GetDistanceToUFunc;
UObject* AEscapeDoorClass;
UObject* HookClass;
UObject* ASearchableClass;
UObject* ATotemClass;
UObject* GetTotemStateFunc;
UObject* AHatchClass;
UObject* IsPulledDownFunc;
UObject* IsBlockedPalletFunc;
UObject* IsDreamPalletFunc;
UObject* APalletClass;
UObject* AWindowClass;
UObject* ACollectableClass;
UObject* UnbrokenFunc;
UObject* ABreakableClass;



uintptr_t GetBoneMatrixF;
uintptr_t GetDebugCosmetics;

bool APlayerController::ProjectWorldLocationToScreen(FVector& WorldLocation, FVector2D& ScreenLocation)
{
	struct {
		FVector WorldLocation;
		FVector2D ScreenLocation;
		bool bPlayerViewportRelative;
		bool ReturnValue;
	} Parameters;

	Parameters.WorldLocation = WorldLocation;
	Parameters.ScreenLocation = ScreenLocation;
	Parameters.bPlayerViewportRelative = FALSE;

	ProcessEvent(WorldToScreenUFunc, &Parameters);

	ScreenLocation = Parameters.ScreenLocation;

	return Parameters.ReturnValue;
}

bool APallet::GetIsPulledDown()
{
	struct {

		bool ReturnValue;
	} Parameters;

	

	ProcessEvent(IsPulledDownFunc, &Parameters);

	

	return Parameters.ReturnValue;
}

bool APallet::GetIsDreamPallet()
{
	struct {

		bool ReturnValue;
	} Parameters;



	ProcessEvent(IsDreamPalletFunc, &Parameters);



	return Parameters.ReturnValue;
}

bool APallet::IsPulldownBlockedByEntity()
{

	struct {

		bool ReturnValue;
	} Parameters;



	ProcessEvent(IsBlockedPalletFunc, &Parameters);



	return Parameters.ReturnValue;
}

int8_t ATotem::GetTotemState()
{
	struct {
		int8_t ReturnValue;
	} Parameters;

	ProcessEvent(GetTotemStateFunc, &Parameters);

	return Parameters.ReturnValue;
}

bool ABreakableBase::IsUnbroken()
{
	struct {
		bool ReturnValue;
	} Parameters;

	ProcessEvent(UnbrokenFunc, &Parameters);

	return Parameters.ReturnValue;
}



bool AController::LineOfSightTo(AActor* Other)
{
	struct {
		AActor* Other;
		FVector ViewPoint;
		bool bAlternateChecks;
		bool ReturnValue;
	} Parameters;

	Parameters.Other = Other;
	Parameters.ViewPoint = FVector{ 0, 0, 0 };
	Parameters.bAlternateChecks = FALSE;

	ProcessEvent(LineOfSightToUFunc, &Parameters);

	return Parameters.ReturnValue;
}

void APlayerController::GetViewportSize(INT& X, INT& Y)
{
	struct {
		INT X;
		INT Y;
	} Parameters;

	Parameters.X = X;
	Parameters.Y = Y;

	ProcessEvent(GetViewportSizeUFunc, &Parameters);

	X = Parameters.X;
	Y = Parameters.Y;
}

void AController::SetControlRotation(FRotator NewRotation) {
	struct {
		FRotator NewRotation;
	} Parameters;

	Parameters.NewRotation = NewRotation;

	ProcessEvent(SetControlRotationUFunc, &Parameters);
}

void Canvas::K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLOAT Thickness, FLinearColor Color)
{
	struct {
		FVector2D ScreenPositionA;
		FVector2D ScreenPositionB;
		FLOAT Thickness;
		FLinearColor Color;
	} Parameters;

	Parameters.ScreenPositionA = ScreenPositionA;
	Parameters.ScreenPositionB = ScreenPositionB;
	Parameters.Thickness = Thickness;
	Parameters.Color = Color;

	ProcessEvent(K2_DrawLineUFunc, &Parameters);
}

void Canvas::K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor)
{
	struct {
		UObject* RenderFont; //UFont* 
		FString RenderText;
		FVector2D ScreenPosition;
		FVector2D Scale;
		FLinearColor RenderColor;
		float Kerning;
		FLinearColor ShadowColor;
		FVector2D ShadowOffset;
		bool bCentreX;
		bool bCentreY;
		bool bOutlined;
		FLinearColor OutlineColor;
	} Parameters;

	Parameters.RenderFont = Font;
	Parameters.RenderText = RenderText;
	Parameters.ScreenPosition = ScreenPosition;
	Parameters.Scale = Scale;
	Parameters.RenderColor = RenderColor;
	Parameters.Kerning = Kerning;
	Parameters.ShadowColor = ShadowColor;
	Parameters.ShadowOffset = ShadowOffset;
	Parameters.bCentreX = bCentreX;
	Parameters.bCentreY = bCentreY;
	Parameters.bOutlined = bOutlined;

	ProcessEvent(K2_DrawTextUFunc, &Parameters);
}

FVector USkeletalMeshComponent::GetBoneMatrix(INT index) {

	auto GetBoneMatrix = reinterpret_cast<FMatrix * (*)(USkeletalMeshComponent*, FMatrix*, INT)>(GetBoneMatrixF);

	FMatrix matrix;
	GetBoneMatrix(this, &matrix, index);

	return FVector({ matrix.M[3][0], matrix.M[3][1], matrix.M[3][2] });
}

FName USkeletalMeshComponent::GetBoneName(INT index) {
	struct {
		INT index;
		FName ReturnValue;
	} Parameters;

	Parameters.index = index;

	ProcessEvent(GetBoneNameUFunc, &Parameters);

	return Parameters.ReturnValue;
}

FVector2D GetBone(USkeletalMeshComponent* Mesh, INT index, APlayerController* PlayerController) {

	FVector WorldLocation = Mesh->GetBoneMatrix(index);

	FVector2D ScreenLocation;

	if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation)) return ScreenLocation;

	return { 0,0 };
}

float AActor::GetDistanceTo(AActor* OtherActor)
{
	struct {
		AActor* OtherActor;
		float ReturnValue;
	} Parameters;

	Parameters.OtherActor = OtherActor;

	ProcessEvent(GetDistanceToUFunc, &Parameters);

	return Parameters.ReturnValue;
}

int8_t APawn::GetPlayerTeam()
{
	struct {
		int8_t ReturnValue;
	} Parameters;

	ProcessEvent(GetPlayerTeamUFunc, &Parameters);

	return Parameters.ReturnValue;
}


bool EngineInit()
{
	auto main = GetModuleHandleA(nullptr);

	static byte objSig[] = { 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x0C, 0xC8, 0x48, 0x8D, 0x04, 0xD1, 0xEB };
	ObjObjects = reinterpret_cast<decltype(ObjObjects)>(Utils::FindPointer(main, objSig, sizeof(objSig), 0));
	if (!ObjObjects) return false;

	static byte poolSig[] = { 0x4C, 0x8D, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x1E, 0x48, 0x8D, 0x0D };
	NamePoolData = reinterpret_cast<decltype(NamePoolData)>(Utils::FindPointer(main, poolSig, sizeof(poolSig), 0));
	if (!NamePoolData) return false;

	static byte worldSig[] = { 0x48, 0x8B, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x85, 0xDB, 0x74, 0x3B, 0x41, 0xB0, 0x01, 0x33, 0xD2, 0x48, 0x8B, 0xCB, 0xE8 };
	WRLD = reinterpret_cast<decltype(WRLD)>(Utils::FindPointer(main, worldSig, sizeof(worldSig), 0));
	if (!WRLD) return false;

	static byte debugCosmeticsSig[] = { 0x48, 0x8B, 0x05, 0x00, 0x00 ,0x00 ,0x00, 0x83 ,0x38, 0x00, 0x75, 0x31 };
	GetDebugCosmetics = reinterpret_cast<decltype(GetDebugCosmetics)>(Utils::FindPointer(main, debugCosmeticsSig, sizeof(debugCosmeticsSig), 0));
	if (!GetDebugCosmetics) return false;


	// Functions & Class Finding

	WorldToScreenUFunc = ObjObjects->FindObject("Function Engine.PlayerController.ProjectWorldLocationToScreen");

	GetViewportSizeUFunc = ObjObjects->FindObject("Function Engine.PlayerController.GetViewportSize");

	K2_DrawLineUFunc = ObjObjects->FindObject("Function Engine.Canvas.K2_DrawLine");

	LineOfSightToUFunc = ObjObjects->FindObject("Function Engine.Controller.LineOfSightTo");

	K2_DrawTextUFunc = ObjObjects->FindObject("Function Engine.Canvas.K2_DrawText");

	GetBoneNameUFunc = ObjObjects->FindObject("Function Engine.SkinnedMeshComponent.GetBoneName");

	SetControlRotationUFunc = ObjObjects->FindObject("Function Engine.Controller.SetControlRotation");

	EnemyClass = ObjObjects->FindObject("Class DeadByDaylight.DBDBasePlayer");

	Font = ObjObjects->FindObject("Font Roboto.Roboto");

	GetPlayerTeamUFunc = ObjObjects->FindObject("Function DeadByDaylight.DBDPlayer.GetPlayerTeam");

	AGeneratorClass = ObjObjects->FindObject("Class DeadByDaylight.Generator");

	GetDistanceToUFunc = ObjObjects->FindObject("Function Engine.Actor.GetDistanceTo");

	AEscapeDoorClass = ObjObjects->FindObject("Class DeadByDaylight.EscapeDoor");

	HookClass = ObjObjects->FindObject("Class DeadByDaylight.MeatHook");

	ASearchableClass = ObjObjects->FindObject("Class DeadByDaylight.Searchable");

	
	//Totem
	ATotemClass = ObjObjects->FindObject("Class DeadByDaylight.Totem");

	AHatchClass = ObjObjects->FindObject("Class DeadByDaylight.Hatch");

	GetTotemStateFunc = ObjObjects->FindObject("Function DeadByDaylight.Totem.GetTotemState");


	
	//Pallet
	APalletClass = ObjObjects->FindObject("Class DeadByDaylight.Pallet");

	IsPulledDownFunc = ObjObjects->FindObject("Function DeadByDaylight.Pallet.GetIsPulledDown");

	IsDreamPalletFunc = ObjObjects->FindObject("Function DeadByDaylight.Pallet.GetIsDreamPallet");

	IsBlockedPalletFunc = ObjObjects->FindObject("Function DeadByDaylight.Pallet.IsPulldownBlockedByEntity");


	//Breakable
	ABreakableClass = ObjObjects->FindObject("Class DeadByDaylight.BreakableBase");

	UnbrokenFunc = ObjObjects->FindObject("Function DeadByDaylight.BreakableBase.IsUnbroken");

	AWindowClass = ObjObjects->FindObject("Class DeadByDaylight.Window");

	ACollectableClass = ObjObjects->FindObject("Class DeadByDaylight.Collectable");


	return true;
}