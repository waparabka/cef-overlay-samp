#include "plugin.hpp"


void c_plugin::update_hooked(const decltype(update_hook)& hook) {
	static bool inited = false;

	if (!inited && c_chat().reference() && rakhook::initialize()) {
		inited = true;
		
		overlay::initialize();
		render::initialize();
		
		render::on_present += []() {
			static bool is_open = false;
			
			if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_End, false)) { is_open ^= true; c_game().reference()->set_cursor_mode(is_open ? 3 : 0, is_open); };
			
			if (is_open) {
				ImGui::SetNextWindowSize({ 1920, 1080 }, ImGuiCond_::ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_::ImGuiCond_FirstUseEver);

				ImGui::Begin("imgui cef sample", &is_open, ImGuiWindowFlags_::ImGuiWindowFlags_None | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration); {
					if (overlay::texture) {
						ImGui::Image(reinterpret_cast<ImTextureID>(overlay::texture), { 840, 660 });
					}
					else {
						ImGui::Text("texture is nullptr");
					}
				}
				ImGui::End();
			}
		};
	}
	return hook.call_trampoline();
}


HRESULT c_plugin::wnd_proc_hooked(const decltype(wnd_proc_hook)& hook, HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp) {
	ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wp, lp);
	overlay::wnd_proc(hwnd, umsg, wp, lp);

	return hook.call_trampoline(hwnd, umsg, wp, lp);
}
