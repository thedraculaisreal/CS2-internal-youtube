#include "menu.h"
#include "../imgui/imgui.h"
#include "../settings/settings.h"
namespace Menu
{
	void menu()
	{
		if (Settings::menu_toggle)
		{
			ImGui::Begin("cheat", &Settings::menu_toggle);
			ImGui::SetWindowSize(ImVec2(600, 700));
			ImGui::BeginTabBar("hi pal");
			ImGui::EndTabBar();
			ImGui::End();
		}
	}
	void toggle_menu()
	{
		Settings::menu_toggle = !Settings::menu_toggle;

		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureMouse = Settings::menu_toggle;
		io.MouseDrawCursor = Settings::menu_toggle;
	}
};