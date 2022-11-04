#include "CosmeticUnlocker.hpp"
#include "../../SDK/Engine.hpp"

void CosmeticUnlocker::EnableCosmetics()
{
	*reinterpret_cast<short*>(GetDebugCosmetics) = 1;
}

void CosmeticUnlocker::DisableCosmetics()
{
	*reinterpret_cast<short*>(GetDebugCosmetics) = 0;
}