#ifndef RENDER_HPP
#define RENDER_HPP

#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "kthook/kthook.hpp"

#include "samp.hpp"


namespace render {
	template <typename T>
	struct on_event : public std::vector<std::function<T>> {
		on_event& operator+=(std::function<T> func) {
			this->push_back(func);
			return *this;
		}
	};
	inline on_event<void()> on_present;

	inline bool initialized = false;

	inline IDirect3DDevice9* device;

	void initialize();
	void destroy();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp);


#endif // !RENDER_HPP