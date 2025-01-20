#pragma once
#include <imgui.h>
#include <vector>
#include <memory>

namespace aether {
    class context;
    class lua_script;

    class ui_manager;
    class ui_code_editor;
    class aimbot_view;
    class esp_view;

    class ui_config {
    public:
        bool show_editor{ true };
        bool show_aimbot_view{ true };
        bool show_esp_view{ true };
    };

    class ui_manager {
    public:
        ui_manager(context& ctx);
        ~ui_manager();

        bool init();

        void render();

        const ui_config& config() const;
        ui_config& config();
        bool is_open() const;

        const std::unique_ptr<ui_code_editor>& editor();
        const std::shared_ptr<lua_script>& editor_script();

    protected:
        context& m_ctx;
        ui_config m_config;
        struct {
            ImFont* roboto_regular{ nullptr };
            ImFont* roboto_bold{ nullptr };
            ImFont* jetbrainsmono_nerdfont_mono_regular{ nullptr };
        } m_fonts;

        bool m_is_open{ true };

        std::unique_ptr<ui_code_editor> m_editor;
        std::unique_ptr<aimbot_view> m_aimbot_view;
        std::unique_ptr<esp_view> m_esp_view;

        std::shared_ptr<lua_script> m_editor_script;
    };
}
