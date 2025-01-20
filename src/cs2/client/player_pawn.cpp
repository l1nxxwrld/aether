#include "../memory_mgr.hpp"
#include "entity_system.hpp"
#include "player_controller.hpp"
#include "player_pawn.hpp"

namespace aether::cs2 {
	C_CSPlayerPawn* C_CSPlayerPawn::get_local_player() {
		if (const auto controller{ CCSPlayerController::get_local_player() }) {
			return reinterpret_cast<C_CSPlayerPawn*>(
				CGameEntitySystem::get()->get_entity(controller->pawn_index())
			);
		}
		return nullptr;
	}

	const qangle& C_CSPlayerPawn::view_angle() const {
		return *reinterpret_cast<qangle*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x124C
		);
	}
	
	const vec3& C_CSPlayerPawn::abs_origin() const {
		return *reinterpret_cast<vec3*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x1324
		);
	}

	const vec3& C_CSPlayerPawn::eye_origin() const {
		return *reinterpret_cast<vec3*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x1384
		);
	}

	const vec3& C_CSPlayerPawn::view_direction() const {
		return *reinterpret_cast<vec3*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x1390
		);
	}

	C_CSWeaponBaseGun* C_CSPlayerPawn::current_weapon() const {
		return *reinterpret_cast<C_CSWeaponBaseGun**>(
			reinterpret_cast<std::uintptr_t>(this) + 0x1C38
		);
	}

	EntitySpottedState_t& C_CSPlayerPawn::spotted_state() {
		return *reinterpret_cast<EntitySpottedState_t*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x23D0
		);
	}

	const EntitySpottedState_t& C_CSPlayerPawn::spotted_state() const {
		return *reinterpret_cast<EntitySpottedState_t*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x23D0
		);
	}
}
