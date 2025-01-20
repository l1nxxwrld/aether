#include "../memory_mgr.hpp"
#include "../utils.hpp"
#include "rendering_world_session.hpp"

namespace aether::cs2 {
    CRenderingWorldSession* CRenderingWorldSession::get_current() {
        return *g_memory->p_current_rendering_world_session;
    }
}
