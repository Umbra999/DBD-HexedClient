#include "GUI.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../kiero/kiero.h"

bool GUI::IsInitialized = false;
bool GUI::Enabled = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present GUI::oPresent = NULL;
WNDPROC GUI::oWndProc = NULL;
ID3D11Device* GUI::pDevice = NULL;
ID3D11DeviceContext* GUI::pContext = NULL;
ID3D11RenderTargetView* GUI::mainRenderTargetView = NULL;

void GUI::Create(HWND window)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	auto& style = ImGui::GetStyle();
	style.FrameRounding = 4.0f;
	style.WindowBorderSize = 2.0f;
	style.PopupBorderSize = 0.0f;
	style.GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.87f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.34f, 0, 0.6f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0, 0.6f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.54f, 0, 0.9f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0, 0.6f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.34f, 0, 0.6f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.34f, 0, 0.6f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.34f, 0, 0.6f, 0.67f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0, 0.6f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0, 0.9f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.34f, 0, 0.6f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.54f, 0, 0.9f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0, 0.6f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.34f, 0, 0.6f, 0.54f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.54f, 0, 0.9f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.34f, 0, 0.6f, 0.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.34f, 0, 0.6f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0, 0.9f, 0.54f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.34f, 0, 0.6f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.34f, 0, 0.6f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0, 0.9f, 0.66f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.34f, 0, 0.6f, 0.66f);
	colors[ImGuiCol_Tab] = ImVec4(0.34f, 0, 0.6f, 0.54f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0, 0.9f, 0.66f);
	colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0, 0.6f, 0.66f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.34f, 0, 0.6f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.54f, 0, 0.9f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.34f, 0, 0.6f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.54f, 0, 0.9f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    IsInitialized = true;
	Enabled = true;
}

void GUI::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("H E X E D");

	if (ImGui::CollapsingHeader("MOVEMENT"))
	{
		
	}

	if (ImGui::CollapsingHeader("ESP"))
	{
		
	}

	if (ImGui::CollapsingHeader("MISC"))
	{

	}

	ImGui::End();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

LRESULT __stdcall GUI::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (Enabled && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall GUI::OnSwapChain(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!IsInitialized && SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
	{
		pDevice->GetImmediateContext(&pContext);
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();
		oWndProc = (WNDPROC)SetWindowLongPtrW(sd.OutputWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
		Create((HWND)sd.OutputWindow);
	}
	else if (Enabled) Render();

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void GUI::KieroInit()
{
	while (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
	{
		Sleep(1);
	}

	kiero::bind(8, (void**)&oPresent, OnSwapChain);
}