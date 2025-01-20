#include <imgui_impl_dx11.h>
#include "../context.hpp"

namespace aether {
	HRESULT STDMETHODCALLTYPE context::resize_buffers(
		IDXGISwapChain* swap_chain,
		UINT buffer_count,
		UINT width,
		UINT height,
		DXGI_FORMAT new_format,
		UINT swap_chain_Flags) {

		ImGui_ImplDX11_InvalidateDeviceObjects();

		const auto result{ get().m_resize_buffers(swap_chain, buffer_count, width, height, new_format, swap_chain_Flags) };

		ImGui_ImplDX11_CreateDeviceObjects();

		return result;
	}
}
