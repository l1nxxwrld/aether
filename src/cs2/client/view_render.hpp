#pragma once
#include "../../math/matrix.hpp"

namespace aether::cs2 {
    class CViewRender {
    public:
        static CViewRender* get();

        const matrix4x4& view_matrix() const;
    };
}
