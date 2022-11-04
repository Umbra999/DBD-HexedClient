#include "Handler.hpp"
#include "../SDK/Engine.hpp"
#include "../Wrapper/Logger.hpp"
#include "InternalSettings.hpp"
#include "Modules/CosmeticUnlocker.hpp"

void Handler::SDKInit()
{
	if (!EngineInit())
	{
		Logger::LogError("Failed to Initialize Engine");
		return;
	}

	Logger::Log("Engine Initialized");

	InitModules();
}

void Handler::InitModules()
{
	if (InternalSettings::CosmeticUnlocker) CosmeticUnlocker::EnableCosmetics();
}

void Handler::OnUpdate()
{
	// update code
}