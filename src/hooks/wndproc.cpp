#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include "../ui/ui_manager.hpp"
#include "../context.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace aether {
	LRESULT CALLBACK context::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
			return 1;
		}

		if (context::get().ui()->is_open()) {
			switch (msg) {
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_NCLBUTTONDOWN:
			case WM_NCLBUTTONUP:
			case WM_NCRBUTTONDOWN:
			case WM_NCRBUTTONUP:
			case WM_MOUSELEAVE:
			case WM_NCMOUSELEAVE:
				return 1;
			}
		}

		return CallWindowProcA(
			reinterpret_cast<WNDPROC>(context::get().m_wndproc),
			hwnd, msg, wparam, lparam
		);
	}
}
