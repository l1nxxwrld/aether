#include "csgo_input.hpp"

namespace aether::cs2 {
    const qangle& CCSGOInput::view_angles() const {
        return *reinterpret_cast<qangle*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x3D0
        );
    }

    void CCSGOInput::set_view_angles(const qangle& angle) {
        *reinterpret_cast<qangle*>(
            reinterpret_cast<std::uintptr_t>(this) +
            0x3D0
        ) = angle;
    }
}
