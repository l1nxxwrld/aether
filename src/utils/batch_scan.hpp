#pragma once
#include <Windows.h>
#include <functional>
#include <xscan.hpp>

namespace aether {
    class batch_scan {
    public:
        using handler_t = std::function<bool(xscan::cursor cursor)>;
        using found_handler_t = std::function<void(const std::string& module_name, const std::string& pattern_name)>;
        using error_handler_t = std::function<bool(const std::string& module_name, const std::string& pattern_name, const xscan::pattern& pattern)>;

        inline batch_scan(
            std::string module_name,
            found_handler_t found_handler = nullptr,
            error_handler_t error_handler = nullptr)
            : m_module_name(std::move(module_name)),
            m_found_handler(found_handler),
            m_error_handler(error_handler) {
        }

        inline batch_scan& add(
            std::string name,
            std::string_view pattern,
            handler_t handler) {

            m_patterns.emplace_back(
                std::move(name),
                xscan::pattern(pattern),
                std::move(handler));

            return *this;
        }

        inline bool scan() const {
            const auto image_base{ GetModuleHandleA(m_module_name.c_str()) };
            if (!image_base) {
                return false;
            }

            bool result{ true };
            xscan::pe_sections sections(image_base);
            for (const auto& [name, pattern, handler] : m_patterns) {

                if (const auto cursor{ pattern.scan(sections) }) {
                    if (!handler(cursor) && (!m_error_handler or !m_error_handler(m_module_name, name, pattern))) {
                        result = false;
                    }
                }

                if (m_found_handler) {
                    m_found_handler(m_module_name, name);
                }
            }

            return true;
        }

    protected:
        std::string m_module_name;
        std::vector<std::tuple<std::string, xscan::pattern, handler_t>> m_patterns;
        found_handler_t m_found_handler;
        error_handler_t m_error_handler;
    };
}
