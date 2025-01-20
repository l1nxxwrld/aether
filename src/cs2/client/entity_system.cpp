#include "../memory_mgr.hpp"
#include "entity_system.hpp"

namespace aether::cs2 {
    CGameEntitySystem* CGameEntitySystem::get() {
        return *g_memory->p_game_entity_system;
    }

    struct entity_entry {
        void* entity;
        char pad8[0x8];
        std::int32_t index;
        char pad14[0x64];
    };

    C_BaseEntity* CGameEntitySystem::get_entity(std::int32_t index) const {
        const auto list{ *reinterpret_cast<entity_entry**>(
            reinterpret_cast<std::uintptr_t>(this) +
            g_memory->o_game_entity_system_entity_list +
            8llu * ((index & 0x7FFF) >> 9)
        ) };

        if (!list) {
            return nullptr;
        }

        const auto& entry{ list[index & 0x1FF] };
        if (entry.entity == nullptr or (entry.index & 0x1FF) != (index & 0x1FF)) {
            return nullptr;
        }

        return reinterpret_cast<C_BaseEntity*>(entry.entity);
    }
}
