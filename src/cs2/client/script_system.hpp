#pragma once
#include "../utils.hpp"

namespace aether::cs2 {

    class IScriptVM abstract {
    public:
        const char* get_vm_type() const;
    };

    class CNopVM : public IScriptVM {};
    class CLuaVM : public IScriptVM {};

    class CVScriptGameSystem {
    public:
        IScriptVM* get_vm() const;
    };
}
