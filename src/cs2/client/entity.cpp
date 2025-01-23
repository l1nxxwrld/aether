#include "entity.hpp"

namespace aether::cs2 {
    const char* C_BaseEntity::get_entity_type_name() const {
        return *reinterpret_cast<const char**>(
            *reinterpret_cast<std::uintptr_t*>(
                reinterpret_cast<std::uintptr_t>(this) +
                0x10
            ) + 0x20
        );
    }

    bool C_BaseEntity::is_dormant() const {
        return *reinterpret_cast<bool*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x0EF
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
}
