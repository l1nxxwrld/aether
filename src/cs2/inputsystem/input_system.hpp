#pragma once
#include <cstdint>

namespace aether::cs2 {
    class CInputSystem {
    public:
        static CInputSystem* get();

        bool has_mouse_focus() const;

        void set_mouse_focus(bool value);

        void set_cursor_clip_mode(std::int32_t mode);

        void set_relative_mouse_mode(bool mode);
    };
}
