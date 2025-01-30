#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include "../cs2/memory_mgr.hpp"
#include "../cs2/client/entity_system.hpp"
#include "../cs2/client/player_controller.hpp"
#include "../cs2/client/player_pawn.hpp"
#include "../cs2/engine/rendering_world_session.hpp"
#include "../cs2/inputsystem/input_stack_system.hpp"
#include "../cs2/inputsystem/input_system.hpp"
#include "../context.hpp"
#include "../scripts/script_mgr.hpp"
#include "../scripts/script.hpp"
#include "fonts/roboto.hpp"
#include "fonts/jetbrainsmono.hpp"
#include "components/code_editor.hpp"
#include "views/aimbot_view.hpp"
#include "views/esp_view.hpp"
#include "ui_manager.hpp"

namespace aether {
    ui_manager::ui_manager(context& ctx)
        : m_ctx(ctx) {}

    ui_manager::~ui_manager() {}

    bool ui_manager::init() {

        ImFontConfig font_config;
        font_config.FontDataOwnedByAtlas = false;

        auto& io{ ImGui::GetIO() };
        io.IniFilename = nullptr;

        m_fonts.roboto_bold = io.Fonts->AddFontFromMemoryTTF(
            fonts::roboto_bold, sizeof(fonts::roboto_bold), 14.0f, &font_config);
        m_fonts.roboto_regular = io.Fonts->AddFontFromMemoryTTF(
            fonts::roboto_regular, sizeof(fonts::roboto_regular), 14.0f, &font_config);
        m_fonts.jetbrainsmono_nerdfont_mono_regular = io.Fonts->AddFontFromMemoryTTF(
            fonts::jetbrainsmono_nerdfont_mono_regular, sizeof(fonts::jetbrainsmono_nerdfont_mono_regular), 14.0f, &font_config);

        m_editor = std::make_unique<ui_code_editor>(
            m_fonts.jetbrainsmono_nerdfont_mono_regular,
            m_fonts.jetbrainsmono_nerdfont_mono_regular
        );

        m_aimbot_view = std::make_unique<aimbot_view>();
        m_esp_view = std::make_unique<esp_view>(m_fonts.roboto_regular);

        m_ctx.scripts()->add_script((m_editor_script = std::make_shared<lua_script>()));

        this->set_open(true);
        return true;
    }

    void ui_manager::uninit() {
        this->set_open(false);
    }

    void ui_manager::render() {

        m_ctx.scripts()->on_pre_ui();

        if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) {
            this->set_open(!this->is_open());
        }

        ImGui::GetIO().MouseDrawCursor = false;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui::PushFont(m_fonts.roboto_regular);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 225.0f / 255.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200.0, 0.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 3.0f, 3.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 5.0f);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(9, 9, 11).Value);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(24, 24, 27).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(24, 24, 27).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImLerp(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg), ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive), 0.5f));

        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImLerp(ImColor(0, 0, 0).Value, ImColor(220, 38, 38).Value, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(ImGui::GetStyleColorVec4(ImGuiCol_Button), ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive), 0.5f));

        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImColor(220, 38, 38).Value);

        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImColor(175, 175, 175, 255).Value);

        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImColor(220, 38, 38).Value);

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImColor(220, 38, 38).Value);

        ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor(220, 38, 38).Value);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(220, 38, 38).Value);

        m_aimbot_view->render();
        
        m_esp_view->render();

        if (m_config.show_input_stack_system) {
            ImGui::PushFont(m_fonts.jetbrainsmono_nerdfont_mono_regular);
            if (ImGui::Begin("Input System")) {
                const auto& iss{ *cs2::CInputStackSystem::get() };
                for (std::int32_t i{ 0 }; i < iss.num_entries(); i++) {
                    const auto& entry{ *iss.get_entry(i) };

                    ImGui::Text("%s (%d)", entry.name, i);
                    ImGui::Text(" - %s", entry.enabled ? "enabled" : "disabled");
                    ImGui::Text(" - cursor %s", entry.is_cursor_visible ? "visible" : "invisible");
                    ImGui::Text(" - cursor %s", entry.is_relative_mouse_mode ? "relative" : "not relative");
                    ImGui::Text(" - IME %s", entry.allow_ime ? "allowed" : "disallowed");
                    ImGui::Text(" - state: %08X", entry.state);
                    ImGui::Text(" - capture: %016X", entry.pcapture);
                    ImGui::Text(" - cursor clip: %016X", entry.pcursor_clip);
                    ImGui::Text("");
                }
            }
            ImGui::End();
            ImGui::PopFont();
        }

        if (m_is_open && m_config.show_editor) {
            ImGui::PushFont(m_fonts.roboto_bold);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

            ImGui::SetNextWindowPos({ 100.0f, 150.0f }, ImGuiCond_Once);
            ImGui::SetNextWindowSize({ 500.0f, 300.0f }, ImGuiCond_Once);

            if (ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_MenuBar)) {

                ImGui::PushFont(m_fonts.roboto_regular);
                if (ImGui::BeginMenuBar()) {
                    ImGui::PushFont(m_fonts.roboto_regular);
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(220, 38, 38).Value);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(220, 38, 38).Value);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(220, 38, 38).Value);
                    ImGui::PushStyleColor(ImGuiCol_Border, ImColor(220, 38, 38).Value);
                    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImColor(220, 38, 38).Value);

                    if (ImGui::BeginMenu("File")) {

                        if (ImGui::Button("Exit")) {
                            context::get().queue_shutdown();
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Edit")) {

                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
                        if (ImGui::Button("Clear Output")) {

                            m_editor->output_buffer().clear();
                        }

                        if (ImGui::Button("Run Script", { ImGui::GetContentRegionAvail().x, 0.0f })) {

                            const auto& input{ m_editor->buffer() };
                            const auto output{ m_editor_script->submit(input) };
                            m_editor->output_buffer() += output;
                        }
                        ImGui::PopStyleVar(1);

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Views")) {

                        ImGui::EndMenu();
                    }

                    ImGui::PopStyleColor(5);
                    ImGui::PopStyleVar(1);
                    ImGui::PopFont();
                    ImGui::EndMenuBar();
                }
                ImGui::PopFont();

                m_editor->render("##code-editor");
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
            ImGui::PopFont();
        }

        ImGui::PopStyleColor(19);
        ImGui::PopStyleVar(8);
        ImGui::PopFont();

        m_ctx.scripts()->on_post_ui();
    }

    const ui_config& ui_manager::config() const {
        return m_config;
    }

    ui_config& ui_manager::config() {
        return m_config;
    }

    bool ui_manager::is_open() const {
        return m_is_open;
    }

    void ui_manager::set_open(bool open) {
        m_is_open = open;

        cs2::CInputStackSystem::get()->apply(8);
    }

    const std::unique_ptr<ui_code_editor>& ui_manager::editor() {
        return m_editor;
    }

    const std::shared_ptr<lua_script>& ui_manager::editor_script() {
        return m_editor_script;
    }
}
