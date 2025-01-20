#include "../memory_mgr.hpp"
#include "entity_system.hpp"
#include "player_controller.hpp"

namespace aether::cs2 {
    CCSPlayerController* CCSPlayerController::get_local_player() {
        return *g_memory->p_local_player_controller;
    }

    C_CSPlayerPawn* CCSPlayerController::get_pawn() const {
        return CGameEntitySystem::get()
            ->get_entity<C_CSPlayerPawn>(this->pawn_index());
    }

    std::int32_t CCSPlayerController::pawn_index() const {
        return *reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uintptr_t>(this) +
            g_memory->o_player_controller_entity_index
        );
    }

    bool CCSPlayerController::is_alive() const {
        return *reinterpret_cast<bool*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x814
        );
    }
}
