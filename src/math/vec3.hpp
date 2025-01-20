#pragma once
#include <cmath>
#include <xmmintrin.h>

namespace aether {
    class vec3 {
    public:
        constexpr vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : x(x), y(y), z(z) {}

        inline vec3 operator*(const float value) const {
            return vec3{
                this->x * value,
                this->y * value,
                this->z * value
            };
        }

        inline vec3 operator/(const float value) const {
            return vec3{
                this->x / value,
                this->y / value,
                this->z / value
            };
        }

        inline vec3 operator+(const vec3& other) const {
            return vec3{
                this->x + other.x,
                this->y + other.y,
                this->z + other.z
            };
        }

        inline vec3 operator-(const vec3& other) const {
            return vec3{
                this->x - other.x,
                this->y - other.y,
                this->z - other.z
            };
        }

        inline float dot_product(const vec3& other) const {
            return this->x * other.x
                + this->y * other.y
                + this->z * other.z;
        }

        inline float magnitude() const {
            return std::sqrtf(
                this->x * this->x +
                this->y * this->y +
                this->z * this->z
            );
        }

        inline vec3 normalize() const {
            const auto magnitude{ this->magnitude() };
            return vec3{
                this->x / magnitude,
                this->y / magnitude,
                this->z / magnitude
            };
        }

        inline float distance_to(const vec3& other) const {
            return vec3{
                this->x - other.x,
                this->y - other.y,
                this->z - other.z
            }.magnitude();
        }

        float x, y, z;
    };
}
