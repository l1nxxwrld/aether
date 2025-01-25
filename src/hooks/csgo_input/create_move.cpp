#include <cmath>
#include <numeric>
#include <numbers>
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>
#include "../../cs2/client/entity_system.hpp"
#include "../../cs2/client/player_controller.hpp"
#include "../../cs2/client/player_pawn.hpp"
#include "../../cs2/client/csgo_input.hpp"
#include "../../config/config.hpp"
#include "../../context.hpp"

namespace aether {
    static constexpr float rad_to_deg(const float rad);
    static constexpr float deg_to_rad(const float rad);
    static qangle calc_angle(const vec3& src, const vec3& dst);

    bool __fastcall context::create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3) {

        auto& cfg{ *get().cfg()->aimbot };
        if (!cfg.enabled) {
            return get().m_create_move(input, a2, a3);
        }

        const auto local_player{ cs2::CCSPlayerController::get_local_player() };
        const auto local_pawn{ local_player->get_pawn() };

        const auto& src_angles{ input->view_angles() };
        qangle new_angles{ src_angles };

        float closest_fov{ std::numeric_limits<float>::max() };
        for (std::int32_t i{ 1 }; i <= 64; i++) {
            const auto player{ cs2::CGameEntitySystem::get()->get_entity<cs2::CCSPlayerController>(i) };
            if (!player or !player->is_alive() or player == local_player) {
                continue;
            }

            if (player->team_number() == local_player->team_number()) {
                continue;
            }

            const auto player_pawn{ player->get_pawn() };
            if (!player_pawn or player_pawn->is_dormant()) {
                continue;
            }

            const auto dst_angles{ calc_angle(local_pawn->eye_origin(), player_pawn->eye_origin()) };
            const qangle delta_angles{
                dst_angles.x - src_angles.x,
                dst_angles.y - src_angles.y
            };

            if (std::abs(delta_angles.x) <= cfg.fov
                && std::abs(delta_angles.y) <= cfg.fov
                && std::abs(delta_angles.y) < closest_fov) {

                closest_fov = std::abs(dst_angles.y - src_angles.y);

                const auto change{ ImGui::GetIO().DeltaTime * cfg.dps };

                new_angles.x = std::clamp(
                    src_angles.x + (delta_angles.x > 0.0f ? change : -change),
                    std::min(src_angles.x, dst_angles.x),
                    std::max(src_angles.x, dst_angles.x)
                );

                new_angles.y = std::clamp(
                    src_angles.y + (delta_angles.y > 0.0f ? change : -change),
                    std::min(src_angles.y, dst_angles.y),
                    std::max(src_angles.y, dst_angles.y)
                );
            }
        }

        new_angles.x = std::clamp(new_angles.x, -89.0f, 89.0f);
        new_angles.y = std::clamp(new_angles.y, -189.0f, 189.0f);
        input->set_view_angles(new_angles);

        return get().m_create_move(input, a2, a3);
    }

    constexpr float rad_to_deg(const float rad) {
        return rad * (180.0f / std::numbers::pi_v<float>);
    }

    constexpr float deg_to_rad(const float rad) {
        return rad * (std::numbers::pi_v<float> / 180.0f);
    }

    qangle calc_angle(const vec3& src, const vec3& dst) {
        const auto delta{ dst - src };
        return qangle{
            rad_to_deg(-std::asin(delta.z / delta.magnitude())),
            rad_to_deg(std::atan2(delta.y, delta.x))
        };
    }
}
