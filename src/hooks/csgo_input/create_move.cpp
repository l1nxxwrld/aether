#include <cmath>
#include <numeric>
#include <algorithm>
#include <imgui.h>
#include "../../cs2/client/entity_system.hpp"
#include "../../cs2/client/player_controller.hpp"
#include "../../cs2/client/player_pawn.hpp"
#include "../../cs2/client/csgo_input.hpp"
#include "../../config/config.hpp"
#include "../../context.hpp"

namespace aether {
    static qangle calc_angle(const vec3& src, const vec3& dst);

    bool __fastcall context::create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3) {
        auto& cfg{ *context::get().cfg()->aimbot };
        if (!cfg.enabled) {
            return context::get().m_create_move(input, a2, a3);
        }

        const auto local_player{ cs2::CCSPlayerController::get_local_player() };
        const auto local_pawn{ local_player->get_pawn() };

        auto& src_angles{ input->view_angles() };
        qangle new_angles{ src_angles };

        float closest_fov{ cfg.fov };
        qangle best_angle{};

        for (std::int32_t i{ 1 }; i <= 64; i++) {
            const auto player{ cs2::CGameEntitySystem::get()->get_entity<cs2::CCSPlayerController>(i) };
            if (!player || !player->is_alive() || player == local_player)
                continue;

            if (player->team_number() == local_player->team_number())
                continue;

            const auto player_pawn{ player->get_pawn() };
            if (!player_pawn || player_pawn->is_dormant())
                continue;

            const qangle dst_angle = calc_angle(local_pawn->eye_origin(), player_pawn->eye_origin());
            const qangle delta{ dst_angle.x - src_angles.x, dst_angle.y - src_angles.y };
            const float current_fov = std::hypot(delta.x, delta.y);

            if (current_fov < closest_fov) {
                closest_fov = current_fov;
                best_angle = dst_angle;
            }
        }

        if (closest_fov < cfg.fov) {
            const float factor = 1.0f / cfg.smoothing;
            new_angles.x = src_angles.x + (best_angle.x - src_angles.x) * factor;
            new_angles.y = src_angles.y + (best_angle.y - src_angles.y) * factor;
        }

        new_angles.x = std::clamp(new_angles.x, -89.0f, 89.0f);
        new_angles.y = std::clamp(new_angles.y, -189.0f, 189.0f);

        input->set_view_angles(new_angles);
        return get().m_create_move(input, a2, a3);
    }

    qangle calc_angle(const vec3& src, const vec3& dst) {
        const auto delta{ dst - src };
        return qangle{
            rad_to_deg(-std::asin(delta.z / delta.magnitude())),
            rad_to_deg(std::atan2(delta.y, delta.x))
        };
    }
}
