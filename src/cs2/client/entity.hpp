#pragma once
#include <cstdint>
#include "../../math/vec3.hpp"
#include "../../math/qangle.hpp"

namespace aether::cs2 {
    class C_BaseEntity {
    public:
        const char* get_entity_type_name() const;
        bool is_dormant() const;
        std::int32_t health() const;
        std::uint32_t flags() const;
        std::uint8_t team_number() const;
        std::int32_t entity_index() const;
    };
}
