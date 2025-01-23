#include <imgui.h>
#include "../../context.hpp"
#include "../../ui/ui_manager.hpp"
#include "../../config/config.hpp"
#include "../../cs2/client/entity.hpp"
#include "../../cs2/client/player_pawn.hpp"
#include "../../cs2/client/player_controller.hpp"
#include "../../cs2/client/entity_system.hpp"
#include "../../cs2/client/view_render.hpp"
#include "esp_view.hpp"

namespace aether {
	esp_view::esp_view() {}

	void esp_view::render() {
        auto& ui{ *context::get().ui() };
        auto& cfg{ *context::get().cfg()->esp };

        if (cfg.enabled) {

            render_player_esp();
        }

        if (!ui.is_open()) {
            return;
        }

        ImGui::SetNextWindowPos({ 375.0f, 100.0f }, ImGuiCond_Once);
        ImGui::SetNextWindowSize({ 250.0f, 250.0f }, ImGuiCond_Once);

        if (ImGui::Begin("ESP")) {
            ImGui::Checkbox("Enabled", &cfg.enabled);
            ImGui::Checkbox("Show Snaplines", &cfg.show_snaplines);
        }
        ImGui::End();
	}

    void esp_view::render_player_esp() {
        auto& cfg{ *context::get().cfg()->esp };

        auto& draw_list{ *ImGui::GetBackgroundDrawList() };
        const auto& io{ ImGui::GetIO() };

        const auto local_player{ cs2::CCSPlayerController::get_local_player() };
        if (!local_player) {
            return;
        }

        for (std::size_t i{ 1 }; i <= 64; i++) {
            const auto player{ cs2::CGameEntitySystem::get()->get_entity<cs2::CCSPlayerController>(i) };
            if (!player or !player->is_alive() or player == local_player) {
                continue;
            }
            const auto& player_team{ player->team_number() };
            if (player->team_number() == local_player->team_number()) {
                continue;
            }

            const auto player_pawn{ player->get_pawn() };
            if (!player_pawn or player_pawn->is_dormant()) {
                continue;
            }

            const auto player_health{ player_pawn->health() };
            const auto& player_pos{ player_pawn->abs_origin() };

            const auto& vm{ cs2::CViewRender::get()->view_matrix() };

            if (cfg.show_snaplines) {

                const auto [x, y, z, w] = vm * player_pos;
                if (w < 0.01f) {
                    continue;
                }

                const ImVec2 screen_pos{
                    (io.DisplaySize.x / 2.0f) * (1.0f + x / w),
                    (io.DisplaySize.y / 2.0f) * (1.0f - y / w)
                };

                draw_list.AddLine(
                    { io.DisplaySize.x * 0.5f, io.DisplaySize.y },
                    screen_pos,
                    ImColor(120, 81, 169, 200),
                    1.0f
                );
            }
        }
    }
}
