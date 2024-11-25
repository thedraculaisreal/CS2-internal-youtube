#pragma once
#include <d3d11.h>
#include <dxgi.h>


namespace Hook
{
	typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain*, UINT, UINT);
	extern PresentFn o_present;
	extern IDXGISwapChain* swap_chain;
	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern HWND window;
	extern ID3D11RenderTargetView* render_target_view;

	HRESULT __stdcall hkPresent(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);
	void init_hook();
	void unhook_present();
	void init_imgui();
};

