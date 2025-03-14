#include "render.hpp"
#include "overlay.hpp"

kthook::kthook_signal<HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> present_hook;
kthook::kthook_signal<HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> reset_hook;


std::optional<HRESULT> d3d9_present_hooked(const decltype(present_hook)& hook, IDirect3DDevice9* device, CONST RECT* src_rect, CONST RECT* dest_rect, HWND dest_window, CONST RGNDATA* dirty_region) {
    if (!render::initialized) {
        render::device = device;
        render::initialized = true;
        
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(GetForegroundWindow());
        ImGui_ImplDX9_Init(device);
        
        ImGui::GetIO().IniFilename = nullptr;
        ImGui::GetIO().MouseDrawCursor = false;

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui::GetStyle().WindowPadding = { 0, 0 };

#pragma warning(push)
#pragma warning(disable: 4996)
        std::string font{ getenv("WINDIR") }; font += "\\Fonts\\Arialbd.TTF";
#pragma warning(pop)
        ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), 14.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    }
    else if (ImGui::GetCurrentContext()) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        
        for (auto it = render::on_present.begin(); it != render::on_present.end();) {
            if (auto f = *it) {
                f(); it++;
            }
        }

        ImGui::EndFrame();
        ImGui::Render();

        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
    return std::nullopt;
}


std::optional<HRESULT> d3d9_lost_hooked(const decltype(reset_hook)& hook, LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* present_params) {
    ImGui_ImplDX9_InvalidateDeviceObjects();

    return std::nullopt;
}


uintptr_t find_device(uint32_t length) {
    static uintptr_t base = [](size_t length) {
        std::string path(MAX_PATH, '\0');

        if (auto size = GetSystemDirectoryA(path.data(), MAX_PATH)) {
            path.resize(size);
            path += "\\d3d9.dll";

            uintptr_t object_base = reinterpret_cast<uintptr_t>(LoadLibraryA(path.c_str()));

            while (object_base++ < object_base + length) {
                if (*reinterpret_cast<uint16_t*>(object_base + 0x00) == 0x06C7 && *reinterpret_cast<uint16_t*>(object_base + 0x06) == 0x8689 && *reinterpret_cast<uint16_t*>(object_base + 0x0C) == 0x8689) {
                    object_base += 2;

                    break;
                }
            }
            return object_base;
        }
        return uintptr_t(0);
    }(length);

    return base;
}


void* get_function_address(int virtual_table_index) {
    return (*reinterpret_cast<void***>(find_device(0x128000)))[virtual_table_index];
}


void render::initialize() {
    present_hook.set_dest(get_function_address(17));
    present_hook.install();
    
    reset_hook.set_dest(get_function_address(16));
    reset_hook.install();

    present_hook.before.connect(&d3d9_present_hooked);
    reset_hook.before.connect(&d3d9_lost_hooked);
}


void render::destroy() {
    if (ImGui::GetCurrentContext()) {

        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();

        ImGui::DestroyContext();
    }
}
