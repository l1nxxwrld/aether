#pragma once
#include <Windows.h>
#include <utility>
#include <vector>
#include <type_traits>

namespace aether {
    template<std::size_t N>
    struct tstring {
        char data[N] {};

        consteval tstring(const char(&str)[N]) {
            std::copy_n(str, N, data);
        }
    };

    template<tstring module_name,
        tstring function_name,
        typename function_type>
    class dynamic_import {
    public:
        template<typename...arguments>
        inline std::invoke_result_t<function_type, arguments...> invoke(arguments&&... args) {
            static const auto instance{ reinterpret_cast<function_type*>(GetProcAddress(GetModuleHandleA(module_name.data), function_name.data)) };
            return instance(std::forward<arguments>(args)...);
        }

        template<typename...arguments>
        inline std::invoke_result_t<function_type, arguments...> operator()(arguments&&... args) {
            return invoke(std::forward<arguments>(args)...);
        }
    };
}
