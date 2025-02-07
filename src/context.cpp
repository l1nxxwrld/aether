#include <Windows.h>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "utils/vmt.hpp"
#include "cs2/memory_mgr.hpp"
#include "cs2/interface_mgr.hpp"
#include "cs2/inputsystem/input_stack_system.hpp"
#include "cs2/rendersystemdx11/render_device.hpp"
#include "cs2/rendersystemdx11/swap_chain.hpp"
#include "cs2/tier0/platform.hpp"
#include "ui/ui_manager.hpp"
#include "scripts/script_mgr.hpp"
#include "scripts/script.hpp"
#include "config/config.hpp"
#include "context.hpp"


namespace aether {
	context& context::get() {
		static context instance;
		return instance;
	}

	bool context::init() {

		g_memory = std::make_unique<memory_mgr>();

		if (!g_memory->init()) {
			return false;
		}

		g_interfaces = std::make_unique<interface_mgr>();
		m_cfg = std::make_unique<config>();
		m_ui = std::make_unique<ui_manager>();
		m_scripts = std::make_unique<script_mgr>();

		if (MH_Initialize() != MH_OK) {
			return false;
		}

		if (MH_CreateHook(g_memory->f_apply_input_stack, &apply_input_stack, reinterpret_cast<void**>(&m_apply_input_stack)) != MH_OK) {
			return false;
		}

		m_create_move = vmt_method::from_class(*g_memory->p_csgo_input, 21).exchange(&create_move);

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			return false;
		}

		const auto render_device{ cs2::CRenderDeviceDx11::get() };
		const auto swap_chain{ render_device->get_swap_chain() };
		const auto d3d11_swap_chain{ swap_chain->get_d3d11_swap_chain() };
		const auto window_handle{ swap_chain->get_window_handle() };

		if (!init_ui(
			window_handle, d3d11_swap_chain,
			render_device->get_d3d11_device(),
			render_device->get_d3d11_device_context())) {

			return false;
		}

		m_wndproc = SetWindowLongPtrA(window_handle, GWLP_WNDPROC, reinterpret_cast<std::intptr_t>(&wndproc));
		m_present = vmt_method::from_class(d3d11_swap_chain, 8).exchange(&present);
		m_resize_buffers = vmt_method::from_class(d3d11_swap_chain, 13).exchange(&resize_buffers);

		if (!m_scripts->init()) {
			return false;
		}

		return true;
	}

	bool context::uninit() {

		const auto render_device{ cs2::CRenderDeviceDx11::get() };
		const auto swap_chain{ render_device->get_swap_chain() };
		const auto d3d11_swap_chain{ swap_chain->get_d3d11_swap_chain() };
		const auto window_handle{ swap_chain->get_window_handle() };

		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();

		m_ui->uninit();

		SetWindowLongPtrA(window_handle, GWLP_WNDPROC, m_wndproc);
		vmt_method::from_class(*g_memory->p_csgo_input, 21).exchange(m_create_move);
		vmt_method::from_class(d3d11_swap_chain, 8).exchange(m_present);
		vmt_method::from_class(d3d11_swap_chain, 13).exchange(m_resize_buffers);

		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		return true;
	}

	bool context::awaiting_shutdown() const {
		return m_shutdown;
	}

	void context::queue_shutdown() {
		m_shutdown = true;
	}

	const std::unique_ptr<ui_manager>& context::ui() const {
		return m_ui;
	}

	const std::unique_ptr<script_mgr>& context::scripts() const {
		return m_scripts;
	}

	const std::unique_ptr<config>& context::cfg() const {
		return m_cfg;
	}

	bool context::init_ui(
		HWND window_handle,
		IDXGISwapChain* swap_chain,
		ID3D11Device* device,
		ID3D11DeviceContext* device_context) {

		ImGui::CreateContext();

		if (!ImGui_ImplWin32_Init(window_handle)) {
			return false;
		}

		if (!ImGui_ImplDX11_Init(device, device_context)) {
			ImGui_ImplWin32_Shutdown();
			return false;
		}

		if (!m_ui->init()) {
			ImGui_ImplWin32_Shutdown();
			ImGui_ImplDX11_Shutdown();
			return false;
		}

		if (!ImGui_ImplDX11_CreateDeviceObjects()) {
			ImGui_ImplWin32_Shutdown();
			ImGui_ImplDX11_Shutdown();
			return false;
		}

		ID3D11Texture2D* back_buffer{ nullptr };
		if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)))) {
			ImGui_ImplWin32_Shutdown();
			ImGui_ImplDX11_Shutdown();
			return false;
		}

		if (FAILED(device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target_view))) {
			ImGui_ImplWin32_Shutdown();
			ImGui_ImplDX11_Shutdown();
			back_buffer->Release();
			return false;
		}

		back_buffer->Release();

		return true;
	}

	context::context() = default;
	context::~context() = default;

	static std::uint32_t init_thread(HMODULE module_base) {

		if (!context::get().init()) {
			FreeLibraryAndExitThread(module_base, EXIT_FAILURE);
		}

		while (!context::get().awaiting_shutdown()) {
			Sleep(1000);
		}

		context::get().uninit();

		FreeLibraryAndExitThread(module_base, EXIT_SUCCESS);
	}
}

std::uint32_t __stdcall DllMain(
	std::uintptr_t module_base,
	std::uint32_t reason,
	std::uintmax_t
) {
	if (reason == DLL_PROCESS_ATTACH) {

		const auto thread_handle{ CreateThread(
			nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(aether::init_thread),
			reinterpret_cast<LPVOID>(module_base), 0, nullptr
		) };

		if (thread_handle == NULL) {

			return FALSE;
		}

		CloseHandle(thread_handle);
	}

	return TRUE;
}
