#pragma once
#include <cstdint>
#include "entity.hpp"
#include "entity_system.hpp"
#include "../../math/vec3.hpp"
#include "../../math/qangle.hpp"

namespace aether::cs2 {

    class C_CSWeaponBaseGun;
    class EntitySpottedState_t;

    class C_CSPlayerPawn
        : public C_BaseEntity {
    public:
        static C_CSPlayerPawn* get_local_player();
        
        const qangle& view_angle() const;
        const vec3& abs_origin() const;
        const vec3& eye_origin() const;
        const vec3& view_direction() const;
        C_CSWeaponBaseGun* current_weapon() const;

        EntitySpottedState_t& spotted_state();
        const EntitySpottedState_t& spotted_state() const;
    };

    class EntitySpottedState_t {
    public:
        virtual void f0() = 0;

        bool spotted;
    };
}
