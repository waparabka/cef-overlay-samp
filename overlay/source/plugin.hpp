#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <windows.h>

#include <kthook/kthook.hpp>
#include <RakHook/rakhook.hpp>

#include "overlay.hpp"

using namespace std::placeholders;


class c_plugin {

public:
	c_plugin() = default;
	~c_plugin() { rakhook::destroy(); render::destroy(); overlay::destroy(); };

	kthook::kthook_simple<void(__cdecl*)()> update_hook { reinterpret_cast<void*>(0x561B10), std::bind(&c_plugin::update_hooked, this, _1) };
	void update_hooked(const decltype(update_hook)& hook);

	kthook::kthook_simple<HRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM)> wnd_proc_hook{ reinterpret_cast<void*>(0x747EB0), std::bind(&c_plugin::wnd_proc_hooked, this, _1, _2, _3, _4, _5) };
	HRESULT wnd_proc_hooked(const decltype(wnd_proc_hook)& hook, HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp);
};


#endif // PLUGIN_HPP