#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>

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

        inline void normalize() {
            this->x = std::clamp(this->x, -89.0f, 89.0f);
            this->y = std::remainder(this->y, 360.0f);
            this->z = std::remainder(this->z, 360.0f);
        }

        inline qangle normalized() const {
            qangle result{ *this };
            result.normalize();
            return result;
        }

        qangle operator+(const qangle& other) const {
            return qangle{
                this->x + other.x,
                this->y + other.y,
                this->z + other.z
            };
        }

        qangle operator-(const qangle& other) const {
            return qangle{
                this->x - other.x,
                this->y - other.y,
                this->z - other.z
            };
        }

        qangle& operator+=(const qangle& other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        qangle& operator-=(const qangle& other) {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            return *this;
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
