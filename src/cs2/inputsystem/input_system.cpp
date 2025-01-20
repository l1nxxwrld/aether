#include "../memory_mgr.hpp"
#include "../../utils/vmt.hpp"
#include "input_system.hpp"

namespace aether::cs2 {
    CInputSystem* CInputSystem::get() {
        return *g_memory->p_input_system;
    }

    bool CInputSystem::has_mouse_focus() const {
        return (*reinterpret_cast<std::add_pointer_t<bool __stdcall(const CInputSystem*)>*const*>(this))[0x220 / 8](this);
    }

    void CInputSystem::set_mouse_focus(bool value) {
        *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + 0x4C) = value;
    }

    // 0x208: Mouse capture enabled
    // 0x210: Mouse capture disabled -> Mouse capture enabled (false)

    void CInputSystem::set_cursor_clip_mode(std::int32_t mode) {
        return (*reinterpret_cast<std::add_pointer_t<void __stdcall(const CInputSystem*, std::int32_t)>*const*>(this))[0x248 / 8](this, mode);
    }

    void CInputSystem::set_relative_mouse_mode(std::int32_t mode) {
        return (*reinterpret_cast<std::add_pointer_t<void __stdcall(const CInputSystem*, std::int32_t)>*const*>(this))[0x260 / 8](this, mode);
    }
}
