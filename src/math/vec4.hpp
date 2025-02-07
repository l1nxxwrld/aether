#pragma once
#include "vec3.hpp"

namespace aether {
    class vec4 {
    public:
        constexpr vec4()
            : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        constexpr vec4(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w) {}

        inline const vec3& xyz() const {
            return *reinterpret_cast<const vec3*>(this);
        }

        float x, y, z, w;
    };
}
