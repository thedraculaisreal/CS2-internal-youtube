#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

namespace Hook
{
	typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain*, UINT, UINT);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	extern PresentFn o_present;
	extern IDXGISwapChain* swap_chain;
	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern HWND window;
	extern ID3D11RenderTargetView* render_target_view;
	extern WNDPROC oWndProc;

	HRESULT __stdcall hkPresent(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);
	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void init_hook();
	void unhook_present();
	void init_imgui();
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

