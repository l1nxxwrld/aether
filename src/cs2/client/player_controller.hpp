#pragma once
#include <cstdint>
#include "entity.hpp"

namespace aether::cs2 {

    class C_CSPlayerPawn;

    class CCSPlayerController
        : public C_BaseEntity {
    public:
        static CCSPlayerController* get_local_player();

        C_CSPlayerPawn* get_pawn() const;

        std::int32_t pawn_index() const;

        bool is_alive() const;
    };
}
