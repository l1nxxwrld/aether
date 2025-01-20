#pragma once
#include <cstdint>

namespace aether::cs2 {
    class C_BaseEntity;

    class CGameEntitySystem {
    public:
        static CGameEntitySystem* get();

        C_BaseEntity* get_entity(std::int32_t index) const;

        template<typename entity_type>
        inline entity_type* get_entity(std::int32_t index) const {
            return reinterpret_cast<entity_type*>(this->get_entity(index));
        }
    };
}
