#pragma once
#include <mutex>
#include <memory>
#include <d3d11.h>

namespace aether {
    class ui_manager;
    class script_mgr;
    class config;

    namespace cs2 {
        class CCSGOInput;
        class CInputStackSystem;
    }

    class context {
    public:
        static context& get();

        bool init();
        bool uninit();
        bool awaiting_shutdown() const;
        void queue_shutdown();

        const std::unique_ptr<ui_manager>& ui() const;
        const std::unique_ptr<script_mgr>& scripts() const;
        const std::unique_ptr<config>& cfg() const;

    protected:
        bool init_ui(
            HWND window_handle,
            IDXGISwapChain* swap_chain,
            ID3D11Device* device,
            ID3D11DeviceContext* device_context);

    protected:
        static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        std::intptr_t m_wndproc{ 0 };

        static HRESULT STDMETHODCALLTYPE present(
            IDXGISwapChain* swap_chain,
            UINT sync_interval,
            UINT flags);

        decltype(&present) m_present{ nullptr };

        static HRESULT STDMETHODCALLTYPE resize_buffers(
            IDXGISwapChain* swap_chain,
            UINT buffer_count,
            UINT width,
            UINT height,
            DXGI_FORMAT new_format,
            UINT swap_chain_flags);

        decltype(&resize_buffers) m_resize_buffers{ nullptr };

        static bool __fastcall create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3);

        decltype(&create_move) m_create_move{ nullptr };

        static void __fastcall apply_input_stack(cs2::CInputStackSystem* iss, std::uint32_t state);

        decltype(&apply_input_stack) m_apply_input_stack{ nullptr };

    protected:
        bool m_shutdown{ false };

        std::unique_ptr<ui_manager> m_ui;
        std::unique_ptr<script_mgr> m_scripts;
        std::unique_ptr<config> m_cfg;

        ID3D11RenderTargetView* m_render_target_view{ nullptr };

    protected:
        context();
        ~context();
    };
}

std::uint32_t __stdcall DllMain(
    std::uintptr_t module_base,
    std::uint32_t reason,
    std::uintmax_t
);
