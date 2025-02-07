#pragma once
#include <cmath>

namespace aether {
    class vec2 {
    public:
        constexpr vec2()
            : x(0.0f), y(0.0f) {}
        constexpr vec2(float x, float y)
            : x(x), y(y) {}

        inline vec2 operator*(const float value) const {
            return vec2{
                this->x * value,
                this->y * value
            };
        }

        inline vec2 operator/(const float value) const {
            return vec2{
                this->x / value,
                this->y / value
            };
        }

        inline vec2 operator+(const vec2& other) const {
            return vec2{
                this->x + other.x,
                this->y + other.y
            };
        }

        inline vec2 operator-(const vec2& other) const {
            return vec2{
                this->x - other.x,
                this->y - other.y
            };
        }
        float x, y;
    };
}
