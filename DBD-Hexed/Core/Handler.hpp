#pragma once
#include "../SDK/Engine.hpp"

class Handler
{
private:

public:
	static void InitModules();
	static void OnRender(UGameViewportClient*, Canvas*);
};
