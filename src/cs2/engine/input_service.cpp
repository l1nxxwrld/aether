#include "../memory_mgr.hpp"
#include "../../utils/vmt.hpp"
#include "input_service.hpp"

namespace aether::cs2 {
    CInputService* CInputService::get() {
        return *g_memory->p_input_service;
    }

    /*
    bool CInputService::has_mouse_focus() const {

        // calls InputSystemVersion001 + 0x220
        return (*reinterpret_cast<std::add_pointer_t<bool __stdcall(const CInputService*)>*const*>(this))[0x140 / 8](this);
    }
    */
}
