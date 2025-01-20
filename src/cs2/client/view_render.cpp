#include "../memory_mgr.hpp"
#include "view_render.hpp"

namespace aether::cs2 {
    CViewRender* CViewRender::get() {
        return *g_memory->p_view_render;
    }

    const matrix4x4& CViewRender::view_matrix() const {
        return *reinterpret_cast<matrix4x4*>(
            reinterpret_cast<std::uintptr_t>(this) +
            g_memory->o_view_render_view_matrix
        );
    }
}
