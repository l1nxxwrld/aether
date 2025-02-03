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
    static float normalize_angle(float angle);

    bool __fastcall context::create_move(cs2::CCSGOInput* input, std::int32_t a2, std::int64_t a3) {

        auto& cfg{ context::get().cfg()->aimbot };
        if (!cfg.enabled) {
            return context::get().m_create_move(input, a2, a3);
        }

        const auto local_player{ cs2::CCSPlayerController::get_local_player() };
        const auto local_pawn{ local_player->get_pawn() };

        qangle closest_angles;
        float closest_fov{ std::numeric_limits<float>::max() };

        for (std::int32_t i{ 1 }; i <= 64; i++) {

            const auto player{ cs2::CGameEntitySystem::get()->get_entity<cs2::CCSPlayerController>(i) };
            if (!player || !player->is_player_controller() || !player->is_alive() || player == local_player) {
                continue;
            }

            if (player->team_number() == local_player->team_number()) {
                continue;
            }

            const auto player_pawn{ player->get_pawn() };
            if (!player_pawn || player_pawn->is_dormant()) {
                continue;
            }

            const auto& target_coords{ player_pawn->anim_graph()->skeleton()->get_bone(6).position.xyz() };
            const auto target_angles{ (local_pawn->eye_origin().angle_to(target_coords) - input->view_angles()).normalized() };

            if (const auto fov{ std::hypot(target_angles.x, target_angles.y) }; fov < cfg.fov && fov < closest_fov) {
                closest_fov = fov;
                closest_angles = target_angles;
            }
        }

        input->set_view_angles((input->view_angles() + qangle{
            closest_angles.x * cfg.factor,
            closest_angles.y * cfg.factor,
            closest_angles.z
        }).normalized());

        return get().m_create_move(input, a2, a3);
    }
}
