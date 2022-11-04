#pragma once

class Patching
{
private:
	static void PatchPostRenderer();

public:
	static void ApplyPatches();
	static void UnapplyPatches();
};
