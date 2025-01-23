#include <cmath>
#include <numeric>
#include <numbers>
#include <algorithm>
#include "../../cs2/client/entity_system.hpp"
#include "../../cs2/client/player_controller.hpp"
#include "../../cs2/client/player_pawn.hpp"
#include "../../cs2/client/csgo_input.hpp"
#include "../../config/config.hpp"
#include "../../context.hpp"

namespace aether {
    static constexpr float rad_to_deg(const float rad) {
        return rad * (180.0f / std::numbers::pi_v<float>);
    }

    static constexpr float deg_to_rad(const float rad) {
        return rad * (std::numbers::pi_v<float> / 180.0f);
    }

    static qangle calc_angle(const vec3& src, const vec3& dst) {
        const auto delta{ dst - src };
        return qangle{
            rad_to_deg(-std::asin(delta.z / delta.magnitude())),
            rad_to_deg(std::atan2(delta.y, delta.x))
        };
    }

    // create move?
    bool __fastcall context::create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3) {

        if (!get().cfg()->aimbot) {
            return get().m_create_move(input, a2, a3);
        }

        const auto local_player{ cs2::CCSPlayerController::get_local_player() };
        const auto local_pawn{ local_player->get_pawn() };

        float closest_distance{ std::numeric_limits<float>::max() };
        cs2::C_CSPlayerPawn* closest_pawn{ nullptr };
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

            const auto distance{ local_player->get_pawn()->eye_origin().distance_to(player_pawn->eye_origin()) };
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_pawn = player_pawn;
            }

            const auto& src{ local_pawn->eye_origin() };
            const auto& dst{ player_pawn->eye_origin() };

            const auto& src_angles{ input->view_angles() };

            auto dst_angles{ calc_angle(src, dst) };

            dst_angles.x = std::clamp(dst_angles.x, -89.0f, 89.0f);
            dst_angles.y = std::clamp(dst_angles.y, -189.0f, 189.0f);

            if (std::abs(dst_angles.x - src_angles.x) <= get().cfg()->aimbot_fov
                && std::abs(dst_angles.y - src_angles.y) <= get().cfg()->aimbot_fov) {

                input->set_view_angles(dst_angles);
                return get().m_create_move(input, a2, a3);
            }
        }

        return get().m_create_move(input, a2, a3);
    }
}
