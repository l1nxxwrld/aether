#pragma once
#include <map>
#include <mutex>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <Windows.h>

namespace aether {
    template<typename interface_t>
    using create_interface_t = interface_t*(__stdcall*)(const char* interface_name, BOOL failed);

    template<typename interface_t>
    using get_interface_t = interface_t*(__stdcall*)();

    class interface_entry {
    public:
        template<typename interface_t>
        inline interface_t* get() const {
            return reinterpret_cast<get_interface_t<interface_t>>(this->get_instance)();
        }

        get_interface_t<void> get_instance;
        const char* name;
        interface_entry* next;
    };

    class interface_list
        : public interface_entry {
    public:
        interface_entry* find(
            std::string_view interface_name
        );
    };

    class interface_mgr {
    public:
        interface_list* find_list(
            const HMODULE module_base
        ) const;

        interface_list* find_list(
            std::string_view module_name
        ) const;

        interface_entry* find(
            std::string_view module_name,
            std::string_view interface_name
        ) const;

        template<typename interface_t>
        inline interface_t* get(
            std::string_view module_name,
            std::string_view interface_name
        ) const {
            if (const auto entry{ this->find(module_name, interface_name) }) {
                return entry->get<interface_t>();
            }
            return nullptr;
        }

    protected:
        mutable std::mutex m_mutex;
        mutable std::unordered_map<std::string_view, interface_list*> m_lookup_cache;
    };

    inline std::unique_ptr<interface_mgr> g_interfaces;
}
