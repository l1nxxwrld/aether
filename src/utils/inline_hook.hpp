#pragma once

namespace aether {
    class inline_hook {
    public:
        inline_hook(void* target, const void* detour);

        bool enable();
        bool disable();
        bool is_enabled() const;

    protected:

    };
}
