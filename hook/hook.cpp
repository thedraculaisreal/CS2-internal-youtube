#include "hook.h"
#include <Windows.h>
#include <iostream>
#include <minhook.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../menu/menu.h"
#include "../settings/settings.h"

namespace Hook
{
	extern PresentFn o_present = nullptr;
	extern IDXGISwapChain* swap_chain = nullptr;
	extern ID3D11Device* device = nullptr;
	extern ID3D11DeviceContext* context = nullptr;
	extern ID3D11RenderTargetView* render_target_view = nullptr;
	extern HWND window = NULL;

	// hook function for IDXGISwapChain::Present
	HRESULT __stdcall hkPresent(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags)
	{
		// init imgui;
		static bool imgui_initialized = false;
		if (!imgui_initialized)
		{
			// retrieve device and context;
			if (!device || !context)
			{
				p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
				device->GetImmediateContext(&context);
			}

			// create render target view;
			ID3D11Texture2D* back_buffer = nullptr;
			p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
			device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
			back_buffer->Release();

			init_imgui();

			imgui_initialized = true;
		}
		// toggle for menu; change VK_INSERT to whatever you want.
		if (GetAsyncKeyState(VK_INSERT) & 0x1)
			Menu::toggle_menu();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Menu::menu();
		// esp goes here.

		ImGui::Render();
		context->OMSetRenderTargets(1, &render_target_view, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return o_present(p_swap_chain, sync_interval, flags); 	// call og func.
	}
	// initilization of present hook;
	void init_hook()
	{
		// create temp device and swapchain for VTABLE;
		DXGI_SWAP_CHAIN_DESC sc_desc = {};
		sc_desc.BufferCount = 1;
		sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sc_desc.BufferDesc.Width = 800; // temp, doesnt matter;
		sc_desc.BufferDesc.Height = 600; // same for that ^;
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc_desc.OutputWindow = GetForegroundWindow();
		sc_desc.SampleDesc.Count = 1;
		sc_desc.Windowed = TRUE;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
			&sc_desc, &swap_chain, &device, nullptr, &context)))
		{
			// getting swapchain vtable;
			void** p_vtable = *(void***)swap_chain;

			// printing out addresses for debugging;
			std::cout << "VTable Address: " << p_vtable << '\n';
			std::cout << "Presnt function address: " << p_vtable[8] << '\n';

			// initialize minhook;
			if (MH_Initialize() == MH_OK)
			{
				// hooking present function.
				if (MH_CreateHook((void*)p_vtable[8], hkPresent, (void**)&o_present) == MH_OK)
				{
					MH_EnableHook((void*)p_vtable[8]);
					std::cout << "Presnt hooked successfully\n";
				}
				else
				{	
					std::cout << "Failed to hook present\n";
				}
			}
			else
			{
				std::cout << "Failed to initialize minhook\n";
			}

			// cleanup;
			swap_chain->Release();
			device->Release();
			context->Release();
		}
		else
		{
			std::cout << "Failed to create device and swap chain\n";
		}
	}
	void unhook_present()
	{
		if (MH_DisableHook((void*)o_present) == MH_OK)
		{
			std::cout << "presnt unhooked successfully\n";
		}
		else
		{
			std::cout << "failed to unhook present\n";
		}

		/*ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();*/
		MH_Uninitialize();
	}
	void init_imgui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		window = FindWindow(nullptr, "Counter-Strike 2");
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(device, context);
	}
};
