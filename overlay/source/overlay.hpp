#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <filesystem>

#include "include/cef_app.h"
#include "render.hpp"


class c_overlay : public CefClient, public CefRenderHandler, public CefLifeSpanHandler {

public:
    c_overlay() = default;
    ~c_overlay() = default;

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; };
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override { this->browser = browser; }
    
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirty_rects, const void* buffer, int width, int height) override;
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override { rect = CefRect(0, 0, 840, 660); };
    
    CefRefPtr<CefBrowser> GetBrowser() { return this->browser; };

private:
    CefRefPtr<CefBrowser> browser;

    IMPLEMENT_REFCOUNTING(c_overlay);
    DISALLOW_COPY_AND_ASSIGN(c_overlay);
};


namespace overlay {
    inline static c_overlay* cef;

    inline LPDIRECT3DTEXTURE9 texture;

    void initialize();
    
    void create_browser(std::string url = "https://google.com");
    bool wnd_proc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp);

    void destroy();
}


#endif // !OVERLAY_HPP