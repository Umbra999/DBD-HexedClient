#include "Handler.hpp"
#include "../Wrapper/Logger.hpp"
#include "InternalSettings.hpp"
#include "Modules/CosmeticUnlocker.hpp"
#include "Modules/ESP.hpp"

void Handler::InitModules()
{
	if (InternalSettings::CosmeticUnlocker) CosmeticUnlocker::EnableCosmetics();
}

void Handler::OnRender(UGameViewportClient* UGameViewportClient, Canvas* canvas)
{
	ESP::OnRender(UGameViewportClient, canvas);
}