#pragma once
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <array>
#include <intrin.h>
#include "page.hpp"

namespace aether {
    class vmt_method {
    public:
        inline vmt_method(void** table, std::size_t index)
            : m_table(table), m_index(index) {}

        static inline vmt_method from_class(void* instance, std::size_t index) {
            return vmt_method(*reinterpret_cast<void***>(instance), index);
        }

        template<typename function_type>
        inline function_type* get() {
            return (function_type*)m_table[m_index];
        }

        template<typename function_type>
        inline function_type* exchange(function_type* detour) {
            const auto result{ reinterpret_cast<function_type*>(m_table[m_index]) };
            if (override_page_protection prot{ &m_table[m_index], sizeof(void*), PAGE_EXECUTE_READWRITE }) {

                m_table[m_index] = detour;
                return result;
            }
            return nullptr;
        }

    protected:
        void** m_table;
        std::size_t m_index;
    };
}
