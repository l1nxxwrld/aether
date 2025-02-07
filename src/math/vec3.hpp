#pragma once
#include <cmath>
#include <numbers>
#include "qangle.hpp"

namespace aether {
    class vec3 {
    public:
        constexpr vec3()
            : x(0.0f), y(0.0f), z(0.0f) {}
        constexpr vec3(float x, float y, float z)
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
            return std::sqrt(
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

        inline qangle angle_to(const vec3& dst) const {
            const auto delta{ dst - *this };
            return qangle{
                rad_to_deg(-std::asin(delta.z / delta.magnitude())),
                rad_to_deg(std::atan2(delta.y, delta.x))
            };
        }

        float x, y, z;
    };
}
