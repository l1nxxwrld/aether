#include <d3d11.h>
#include "../memory_mgr.hpp"
#include "render_device.hpp"

namespace aether::cs2 {
	CRenderDeviceDx11* CRenderDeviceDx11::get() {
		return *g_memory->p_render_device;
	}

	CSwapChainDx11* CRenderDeviceDx11::get_swap_chain() const {
		return *reinterpret_cast<CSwapChainDx11**>(
			*reinterpret_cast<std::uintptr_t*>(
				reinterpret_cast<std::uintptr_t>(this) +
				g_memory->o_render_device_dx11_swap_chain_list
			) + 0x10
		);
	}

	ID3D11Device* CRenderDeviceDx11::get_d3d11_device() const {
		return *reinterpret_cast<ID3D11Device**>(
			reinterpret_cast<std::uintptr_t>(this) +
			g_memory->o_render_device_dx11_device
		);
	}

	ID3D11DeviceContext* CRenderDeviceDx11::get_d3d11_device_context() const {
		return *reinterpret_cast<ID3D11DeviceContext**>(
			reinterpret_cast<std::uintptr_t>(this) +
			g_memory->o_render_device_dx11_device_context
		);
	}
}
