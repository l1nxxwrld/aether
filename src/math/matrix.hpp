#pragma once
#include <xmmintrin.h>
#include "vec3.hpp"
#include "vec4.hpp"

namespace aether {
    class matrix3x4 {
    public:
        inline matrix3x4() = default;

        float m[3][4]{ 0.0f };
    };

    class matrix4x4 {
    public:
        inline matrix4x4() = default;

        inline vec4 operator*(const vec3& other) const {
            return {
                m[0][0] * other.x + m[0][1] * other.y + m[0][2] * other.z + m[0][3],
                m[1][0] * other.x + m[1][1] * other.y + m[1][2] * other.z + m[1][3],
                m[2][0] * other.x + m[2][1] * other.y + m[2][2] * other.z + m[2][3],
                m[3][0] * other.x + m[3][1] * other.y + m[3][2] * other.z + m[3][3],
            };
        }

        inline vec4 operator*(const vec4& other) const {
            return {
                m[0][0] * other.x + m[0][1] * other.y + m[0][2] * other.z + m[0][3],
                m[1][0] * other.x + m[1][1] * other.y + m[1][2] * other.z + m[1][3],
                m[2][0] * other.x + m[2][1] * other.y + m[2][2] * other.z + m[2][3],
                m[3][0] * other.x + m[3][1] * other.y + m[3][2] * other.z + m[3][3],
            };
        }

        float m[4][4]{ 0.0f };
    };
}
