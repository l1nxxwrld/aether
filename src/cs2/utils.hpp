#pragma once
#include <cstdint>
#include <cstddef>
#include <functional>

namespace aether {
    template<typename return_type, typename... arguments>
    static inline return_type invoke(const void* class_base, std::size_t index, arguments&&...args) {
        const auto vmt{ *reinterpret_cast<void** const*>(class_base) };
        return reinterpret_cast<return_type(__stdcall*)(const void*, arguments...)>(vmt[index])(class_base, std::forward<arguments>(args)...);
    }
}
