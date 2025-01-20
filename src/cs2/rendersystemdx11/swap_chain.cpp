#include <d3d11.h>
#include "../memory_mgr.hpp"
#include "swap_chain.hpp"

namespace aether::cs2 {
	HWND CSwapChainDx11::get_window_handle() const {
		return *reinterpret_cast<HWND*>(
			reinterpret_cast<std::uintptr_t>(this) +
			0x10
		);
	}

	IDXGISwapChain* CSwapChainDx11::get_d3d11_swap_chain() const {
		return *reinterpret_cast<IDXGISwapChain**>(
			reinterpret_cast<std::uintptr_t>(this) +
			g_memory->o_swap_chain_dx11_swap_chain
		);
	}
}
