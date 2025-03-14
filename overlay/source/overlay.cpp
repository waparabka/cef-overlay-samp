#include "overlay.hpp"


void c_overlay::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirty_rects, const void* buffer, int width, int height) {
    if (!overlay::texture)
        D3DXCreateTexture(render::device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &overlay::texture);

    D3DLOCKED_RECT locked_rect;
    
    overlay::texture->LockRect(0, &locked_rect, nullptr, 0);
    memcpy(locked_rect.pBits, buffer, width * height * 4);

    overlay::texture->UnlockRect(0);
}


void overlay::create_browser(std::string url) {
    CefWindowInfo window_info;
    CefBrowserSettings browser_settings;

    window_info.SetAsWindowless(nullptr);
    
    CefBrowserHost::CreateBrowser(window_info, cef, url.c_str(), browser_settings, nullptr, nullptr);
}


void overlay::initialize() {
    cef = new c_overlay();
    
    std::string work_path = std::filesystem::current_path().string();
    std::filesystem::create_directories(work_path + "\\cache");

    CefSettings settings;
    
    CefString(&settings.browser_subprocess_path).FromASCII(std::string(work_path + "\\cef-sub-process.exe").c_str());
    CefString(&settings.log_file) = work_path + "\\cef-log.log";
    CefString(&settings.cache_path).FromASCII(std::string(work_path + "\\cache").c_str());
    CefString(&settings.locales_dir_path).FromASCII(std::string(work_path + "\\locales").c_str());
    CefString(&settings.resources_dir_path) = work_path;
    CefString(&settings.locale).FromASCII("ru");
    CefString(&settings.accept_language_list).FromASCII("en-US,en,ru-RU,ru");

    settings.no_sandbox = 1;
    settings.multi_threaded_message_loop = 1;
    settings.windowless_rendering_enabled = 1;

    settings.background_color = 0;
    settings.log_severity = LOGSEVERITY_DEBUG;
    settings.command_line_args_disabled = 1;

    CefMainArgs main_args;

    CefInitialize(main_args, settings, nullptr, nullptr);
    
    create_browser();
}


bool overlay::wnd_proc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp) {
    if (cef && cef->GetBrowser()) {
        CefRefPtr<CefBrowserHost> host = cef->GetBrowser()->GetHost();

        if (umsg >= WM_MOUSEMOVE && umsg <= WM_MBUTTONUP || umsg == WM_MOUSEWHEEL) {
            POINT point = { LOWORD(lp), HIWORD(lp) };
            ScreenToClient(hwnd, &point);

            CefMouseEvent mouse_event;
            
            mouse_event.x = point.x;
            mouse_event.y = point.y;
            mouse_event.modifiers = 0;

            switch (umsg) {
                case WM_LBUTTONDOWN: host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1); break;
                case WM_LBUTTONUP:   host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1); break;
                case WM_RBUTTONDOWN: host->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1); break;
                case WM_RBUTTONUP:   host->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1); break;
                case WM_MBUTTONDOWN: host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, false, 1); break;
                case WM_MBUTTONUP:   host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, true, 1); break;
                case WM_MOUSEWHEEL: {
                    int delta = GET_WHEEL_DELTA_WPARAM(wp);
                    host->SendMouseWheelEvent(mouse_event, 0, delta);
                    break;
                }
                default: {
                    host->SendMouseMoveEvent(mouse_event, false);
                    break;
                }
            }
            return true;
        }

        if (umsg >= WM_KEYDOWN && umsg <= WM_CHAR) {
            int keycode = static_cast<int>(wp);
            int modifiers = 0;

            if (GetKeyState(VK_SHIFT) & 0x8000) modifiers |= EVENTFLAG_SHIFT_DOWN;
            if (GetKeyState(VK_CONTROL) & 0x8000) modifiers |= EVENTFLAG_CONTROL_DOWN;
            if (GetKeyState(VK_MENU) & 0x8000) modifiers |= EVENTFLAG_ALT_DOWN;

            CefKeyEvent event;
            
            event.windows_key_code = keycode;
            event.native_key_code = static_cast<int>(lp);
            event.modifiers = modifiers;

            if (umsg == WM_KEYDOWN || umsg == WM_SYSKEYDOWN) { event.type = KEYEVENT_RAWKEYDOWN; }
            else if (umsg == WM_KEYUP || umsg == WM_SYSKEYUP) { event.type = KEYEVENT_KEYUP; }
            else if (umsg == WM_CHAR) { event.type = KEYEVENT_CHAR; }

            host->SendKeyEvent(event);

            return true;
        }
    }
    return false;
}


void overlay::destroy() {
    CefShutdown();
    
    if (texture)
        texture->Release();
    
    delete cef;
}