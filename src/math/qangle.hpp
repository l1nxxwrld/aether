#pragma once
#include <xmmintrin.h>

namespace aether {
    class qangle {
    public:
        constexpr qangle(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : x(x), y(y), z(z) {
        }

        constexpr float pitch() const {
            return this->x;
        }
        constexpr float yaw() const {
            return this->y;
        }
        constexpr float roll() const {
            return this->z;
        }

        float x, y, z;
    };
}
