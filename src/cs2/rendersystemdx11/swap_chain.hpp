#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <cstdint>

namespace aether::cs2 {
	class CSwapChainDx11 {
	public:
		HWND get_window_handle() const;
		IDXGISwapChain* get_d3d11_swap_chain() const;
	};
}
