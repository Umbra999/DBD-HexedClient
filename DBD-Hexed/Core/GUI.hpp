#pragma once
#include "../kiero/includes.hpp"

class GUI
{
private:
	static bool IsInitialized;
	static void Create(HWND);
	static void Render();

	static HRESULT __stdcall OnSwapChain(IDXGISwapChain*, UINT, UINT);
	static ID3D11Device* pDevice;
	static ID3D11DeviceContext* pContext;
	static Present oPresent;
	static WNDPROC oWndProc;
	static ID3D11RenderTargetView* mainRenderTargetView;

	static LRESULT __stdcall WndProc(const HWND, UINT, WPARAM, LPARAM);

public:
	static bool Enabled;
	static void KieroInit();
};
