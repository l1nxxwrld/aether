#include "script_system.hpp"

namespace aether::cs2 {
    const char* IScriptVM::get_vm_type() const {
        return invoke<const char*>(this, 5);
    }

    IScriptVM* CVScriptGameSystem::get_vm() const {
        return invoke<IScriptVM*>(this, 4);
    }
}
