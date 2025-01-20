#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <xscan.hpp>
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

namespace aether {
    class ui_manager;
    
    namespace cs2 {
        class CRenderDeviceBase;
        class CGameEntitySystem;
        class CCSPlayerController;
        class CViewRender;
        class CCSGOInput;
        class CRenderingWorldSession;
        class CRenderDeviceMgrDx11;
        class CRenderDeviceDx11;
        class C_CSPlayerPawn;
        class C_CSPlayerPawnBase;
        class CSchemaSystem;
        class CSchemaSystemTypeScope;
        class CInputSystem;
        class CInputService;
    }

    class memory_mgr {
    public:
        memory_mgr() = default;
        ~memory_mgr() = default;

        bool init();

        using pattern = xscan::pattern;
        using cursor = xscan::cursor;

        cs2::CCSPlayerController** p_local_player_controller{};
        cs2::CGameEntitySystem** p_game_entity_system{};
        cs2::CViewRender** p_view_render{};
        cs2::CCSGOInput** p_csgo_input{};
        cs2::CRenderingWorldSession** p_current_rendering_world_session{}; // CRenderingWorldSession::s_pCurrentSession
        cs2::CRenderDeviceMgrDx11** p_render_device_mgr{};
        cs2::CRenderDeviceDx11** p_render_device{};
        cs2::CSchemaSystem** p_schema_system{};
        cs2::CInputSystem** p_input_system{};
        cs2::CInputService** p_input_service{};

        std::int32_t o_game_entity_system_entity_list{ 0x10 };
        std::int32_t o_player_controller_entity_index{};
        std::int32_t o_render_device_dx11_swap_chain_list{};
        std::int32_t o_render_device_dx11_device{};
        std::int32_t o_render_device_dx11_device_context{};
        std::int32_t o_swap_chain_dx11_swap_chain{};
        std::int32_t o_view_render_view_matrix{ 0x298 };
        std::int32_t o_rendering_world_session_get_world_name{};

        __int64(__stdcall* f_find_binding)(cs2::CSchemaSystemTypeScope* pthis, const char* name) {};

    protected:
        nlohmann::json m_json;
    };

    inline std::unique_ptr<memory_mgr> g_memory;
}
