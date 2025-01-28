#pragma once
#include <numbers>

namespace aether {
    class qangle {
    public:
        constexpr qangle()
            : x(0.0f), y(0.0f), z(0.0f) {}
        constexpr qangle(float x, float y, float z = 0.0f)
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

    constexpr float rad_to_deg(const float rad) {
        return rad * (180.0f / std::numbers::pi_v<float>);
    }

    constexpr float deg_to_rad(const float rad) {
        return rad * (std::numbers::pi_v<float> / 180.0f);
    }
}
