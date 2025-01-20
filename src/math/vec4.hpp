#pragma once
#include <xmmintrin.h>

namespace aether {
    class vec4 {
    public:
        constexpr vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
            : x(x), y(y), z(z), w(w) {}

        float x, y, z, w;
    };
}
