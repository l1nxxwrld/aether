#pragma once
#include <cstdint>
#include <cstddef>
#include <functional>
#include <Windows.h>

namespace aether {
    class override_page_protection {
    public:
        template<bool immediately = true>
        inline override_page_protection(const void* address, std::size_t size, std::uint32_t protect)
            : m_address(address), m_size(size), m_protect(protect),
            m_old_protect(0), m_is_applied(false) {

            if constexpr (immediately) {
                m_is_applied = apply();
            }
        }

        inline ~override_page_protection() {
            restore();
        }

        inline bool apply() {
            return !m_is_applied ?
                (m_is_applied = VirtualProtect(
                    const_cast<void*>(m_address),
                    m_size, m_protect,
                    reinterpret_cast<PDWORD>(&m_old_protect))
                ) : false;
        }

        bool restore() {
            return m_is_applied ?
                !(m_is_applied = !VirtualProtect(
                    const_cast<void*>(m_address),
                    m_size, m_old_protect,
                    reinterpret_cast<PDWORD>(&m_old_protect))
                ) : false;
        }

        inline bool is_applied() const {
            return m_is_applied;
        }

        inline operator bool() const {
            return is_applied();
        }

        template<typename return_type, typename... arguments>
        inline return_type then(auto&& func, arguments&&... args) const {
            return func(std::forward<arguments>(args)...);
        }

    protected:
        const void* m_address;
        std::size_t m_size;
        std::uint32_t m_protect;
        std::uint32_t m_old_protect;
        bool m_is_applied;
    };
}
