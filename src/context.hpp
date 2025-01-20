#pragma once
#include <memory>
#include <d3d11.h>

namespace aether {
    class ui_manager;
    class script_mgr;
    class config;

    namespace cs2 {
        class CCSGOInput;
    }

    class context {
    public:
        static context& get();

        bool init();
        bool uninit();
        bool running() const;
        void queue_shutdown();

        const std::unique_ptr<ui_manager>& ui() const;
        const std::unique_ptr<script_mgr>& scripts() const;
        const std::unique_ptr<config>& cfg() const;

    protected:
        bool init_ui();

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
            UINT swap_chain_Flags);

        decltype(&resize_buffers) m_resize_buffers{ nullptr };

        static bool __fastcall create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3);

        decltype(&create_move) m_create_move{ nullptr };

    protected:
        bool m_initialized{ false };
        bool m_shutdown{ false };

        std::unique_ptr<ui_manager> m_ui;
        std::unique_ptr<script_mgr> m_scripts;
        std::unique_ptr<config> m_cfg;

        ID3D11RenderTargetView* m_render_target_view{ nullptr };

    protected:
        context() = default;
        ~context();
    };
}

std::uint32_t __stdcall DllMain(
    std::uintptr_t module_base,
    std::uint32_t reason,
    std::uintmax_t
);
