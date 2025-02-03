#include <string>
#include "entity.hpp"

namespace aether::cs2 {
    CBodyComponentBaseAnimGraph* C_BaseEntity::anim_graph() const {
        return *reinterpret_cast<CBodyComponentBaseAnimGraph**>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x38
        );
    }

    const char* C_BaseEntity::get_entity_type_name() const {
        return *reinterpret_cast<const char**>(
            *reinterpret_cast<std::uintptr_t*>(
                reinterpret_cast<std::uintptr_t>(this) +
                0x10
            ) + 0x20
        );
    }

    bool C_BaseEntity::is_player_controller() const {
        return std::strcmp(this->get_entity_type_name(), "cs_player_controller") == 0;
    }

    bool C_BaseEntity::is_dormant() const {
        return *reinterpret_cast<bool*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x0EF
        );
    }

    CCollisionProperty* C_BaseEntity::collision_property() const {
        return *reinterpret_cast<CCollisionProperty**>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x338
        );
    }

    std::int32_t C_BaseEntity::health() const {
        return *reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x344
        );
    }

    std::uint32_t C_BaseEntity::flags() const {
        return *reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x3EC
        );
    }

    e_team_number C_BaseEntity::team_number() const {
        return *reinterpret_cast<e_team_number*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x3E3
        );
    }

	std::int32_t C_BaseEntity::entity_index() const {
		return *reinterpret_cast<std::int32_t*>(
			reinterpret_cast<std::uintptr_t>(this) + 0x440
		);
	}

    CSkeletonInstance* CBodyComponentBaseAnimGraph::skeleton() const {
        return *reinterpret_cast<CSkeletonInstance**>(
			reinterpret_cast<std::uintptr_t>(this) + 0x8
		);
    }

    std::int32_t CSkeletonInstance::bone_count() const {
        return *reinterpret_cast<std::int32_t*>(
            reinterpret_cast<std::uintptr_t>(this) + 0x1CC
        );
    }

    const CSkeletonInstance::bone& CSkeletonInstance::get_bone(std::int32_t index) const {
        return (*reinterpret_cast<bone**>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x1F0 // CModelState + 0x80
        ))[index];
    }

    const vec3& CCollisionProperty::mins() const {
        return *reinterpret_cast<vec3*>(
            reinterpret_cast<std::uintptr_t>(this) + 0x40
        );
    }

    const vec3& CCollisionProperty::maxs() const {
        return *reinterpret_cast<vec3*>(
            reinterpret_cast<std::uintptr_t>(this) + 0x4C
        );
    }
}
