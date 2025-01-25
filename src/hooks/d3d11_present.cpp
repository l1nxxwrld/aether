#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "../ui/ui_manager.hpp"
#include "../cs2/rendersystemdx11/render_device.hpp"
#include "../cs2/inputsystem/input_system.hpp"
#include "../cs2/inputsystem/input_stack_system.hpp"
#include "../context.hpp"

namespace aether {
	HRESULT STDMETHODCALLTYPE context::present(
		IDXGISwapChain* swap_chain,
		UINT sync_interval,
		UINT flags) {

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		context::get().ui()->render();

		ImGui::Render();

		const auto render_device{ cs2::CRenderDeviceDx11::get() };
		const auto device_context{ render_device->get_d3d11_device_context() };

		ID3D11RenderTargetView* o_rtv;
		ID3D11DepthStencilView* o_dsv;
		device_context->OMGetRenderTargets(1, &o_rtv, &o_dsv);

		device_context->OMSetRenderTargets(1, &get().m_render_target_view, nullptr);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		device_context->OMSetRenderTargets(1, &o_rtv, o_dsv);

		return context::get().m_present(swap_chain, sync_interval, flags);
	}
}
