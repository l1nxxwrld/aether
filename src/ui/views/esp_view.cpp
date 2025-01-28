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
            ImGui::Checkbox("enabled", &cfg.enabled);
            ImGui::Checkbox("show snaplines", &cfg.show_snaplines);
            ImGui::Checkbox("show box ESP", &cfg.show_box);
            if (cfg.show_box) {
                ImGui::Checkbox("fill box", &cfg.box_fill);
                ImGui::ColorEdit4("box color", &cfg.box_color.x);
                ImGui::SliderFloat("box thickness", &cfg.box_thickness, 0.5f, 5.0f, "%.1f");

                ImGui::Checkbox("box outline", &cfg.box_outline);
                if (cfg.box_outline) {
                    ImGui::SliderFloat("outline thickness", &cfg.outline_thickness, 0.5f, 5.0f, "%.1f");
                }
            }


            ImGui::Checkbox("show health bar", &cfg.show_health_bar);
            if (cfg.show_health_bar) {
                ImGui::SliderFloat("health bar width", &cfg.healthbar_thickness, 1.0f, 10.0f, "%.1f");
                ImGui::Checkbox("healthbar outline", &cfg.healthbar_outline);
                if (cfg.healthbar_outline) {
                    ImGui::SliderFloat("healthbar outline thickness", &cfg.healthbar_outline_thickness, 0.5f, 5.0f, "%.1f");
                }
            }
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

        const auto& vm{ cs2::CViewRender::get()->view_matrix() };

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

            const float health = player_pawn->health();
            const float health_percent = health / 100.0f;

            const vec3 origin = player_pawn->abs_origin();

            const float height = 72.0f;
            const float width = 32.0f;

            vec3 feet = origin;
            vec3 head = origin + vec3{ 0, 0, height };

            ImVec2 screen_feet, screen_head;
            {
                const auto [x1, y1, z1, w1] = vm * feet;
                if (w1 < 0.001f) continue;
                screen_feet = ImVec2{
                    (io.DisplaySize.x / 2) * (1 + x1 / w1),
                    (io.DisplaySize.y / 2) * (1 - y1 / w1)
                };
            }

            {
                const auto [x2, y2, z2, w2] = vm * head;
                if (w2 < 0.001f) continue;
                screen_head = ImVec2{
                    (io.DisplaySize.x / 2) * (1 + x2 / w2),
                    (io.DisplaySize.y / 2) * (1 - y2 / w2)
                };
            }
            const float box_height = screen_feet.y - screen_head.y;
            const float box_width = box_height * 0.5f;

            const ImVec2 box_min = ImVec2(screen_head.x - box_width / 2, screen_head.y);
            const ImVec2 box_max = ImVec2(screen_head.x + box_width / 2, screen_feet.y);
            if (cfg.show_box) {
                // box fill
                if (cfg.box_fill) {
                    ImU32 fill_col = ImGui::ColorConvertFloat4ToU32(cfg.box_color);
                    draw_list.AddRectFilled(box_min, box_max, fill_col);
                }

				// box outline
                if (cfg.box_outline) {
                    const ImVec2 outline_min = ImVec2(box_min.x - 1, box_min.y - 1);
                    const ImVec2 outline_max = ImVec2(box_max.x + 1, box_max.y + 1);
                    draw_list.AddRect(
                        outline_min,
                        outline_max,
                        IM_COL32(0, 0, 0, 255),
                        0.0f,
                        0,
                        cfg.outline_thickness
                    );
                }
                ImU32 box_col = ImGui::ColorConvertFloat4ToU32(cfg.box_color);
                draw_list.AddRect(
                    box_min,
                    box_max,
                    box_col,
                    0.0f,
                    0,
                    cfg.box_thickness
                );
            }

			// health bar
            if (cfg.show_health_bar && health > 0) {
                const float healthbar_width = cfg.healthbar_thickness;
                const ImVec2 health_min = ImVec2(
                    box_min.x - (3.0f + healthbar_width),
                    box_max.y - (box_height * health_percent)
                );
                const ImVec2 health_max = ImVec2(
                    box_min.x - 3.0f,
                    box_max.y
                );

                if (cfg.healthbar_outline) {
                    draw_list.AddRect(
                        ImVec2(health_min.x - 1.0f, box_min.y - 1.0f),
                        ImVec2(health_max.x + 1.0f, box_max.y + 1.0f),
                        IM_COL32(0, 0, 0, 255),
                        0.0f,
                        0,
                        cfg.healthbar_outline_thickness
                    );
                }

                // fill healt
                draw_list.AddRectFilled(
                    health_min,
                    health_max,
                    IM_COL32(0, static_cast<int>(255 * health_percent), 0, 200)
                );
            }
        }
    }
}
