#pragma once
#include <cstdint>

namespace aether::cs2 {

    class input_stack_entry {
    public:
        const char* name;
        std::uint8_t gap8[8]; // 8 * 1
        void* pcapture;
        void* pcursor_clip;
        std::uint8_t gap20[128]; // 8 * 16
        std::uint32_t state;
        bool enabled;
        bool is_cursor_visible;
        bool is_relative_mouse_mode;
        bool allow_ime;
    }; // alloc size: 0xA8

    class CInputStackSystem {
    public:
        static CInputStackSystem* get();

        std::int32_t num_entries() const;

        input_stack_entry* get_entry(std::int32_t index) const;
        input_stack_entry* get_entry(const char* name) const;

        input_stack_entry* push(const char* name, std::uint32_t state);

        void pop(); // not sure

        void apply(std::uint32_t state);
    };
}
