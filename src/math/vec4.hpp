#pragma once
#include <xmmintrin.h>
#include "vec3.hpp"

namespace aether {
    class vec4 {
    public:
        constexpr vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
            : x(x), y(y), z(z), w(w) {}

        inline const vec3& xyz() const {
            return *reinterpret_cast<const vec3*>(this);
        }

        float x, y, z, w;
    };
}
