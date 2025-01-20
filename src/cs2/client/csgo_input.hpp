#pragma once
#include <cstdint>
#include "../../math/qangle.hpp"

namespace aether::cs2 {
    class CMsgQAngle;

    class CCSGOInput {
    public:
        const qangle& view_angles() const;

        void set_view_angles(const qangle& angle);
    };

    class CMsgQAngle {
    public:
        char pad0[0x18];
        float x, y, z;
    };
}
