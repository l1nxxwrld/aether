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

    // I think this is what we call the create move function?
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
            const auto& player_team{ player->team_number() };
            if (player->team_number() == local_player->team_number()) {
                continue;
            }

            const auto player_pawn{ player->get_pawn() };
            if (!player_pawn or player_pawn->is_dormant()) {
                continue;
            }

            const auto distance{ std::sqrtf(
                local_player->get_pawn()->eye_origin().distance_to(player_pawn->eye_origin())
            ) };

            if (distance < closest_distance) {
                closest_distance = distance;
                closest_pawn = player_pawn;
            }
        }

        if (closest_pawn) {

            qangle angle{ input->view_angles() };

            const auto& src{ local_pawn->eye_origin() };
            const auto& dst{ closest_pawn->eye_origin() };
            const auto delta{ src - dst };

            angle.x = rad_to_deg(std::asinf(delta.z / delta.magnitude()));
            angle.y = rad_to_deg(std::atan2f(-delta.y, -delta.x));

            angle.x = std::clamp(angle.x, -89.0f, 89.0f);
            angle.y = std::clamp(angle.y, -189.0f, 189.0f);

            input->set_view_angles(angle);
        }

        return get().m_create_move(input, a2, a3);
    }
}
