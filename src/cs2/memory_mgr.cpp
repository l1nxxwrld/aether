#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include "utils.hpp"
#include "../utils/batch_scan.hpp"
#include "../context.hpp"
#include "memory_mgr.hpp"

namespace aether {
    bool memory_mgr::init() {

        const auto found_handler = [this](const std::string& module_name, const std::string& pattern_name) {
#ifdef _DEBUG
            std::printf("found %s:%s\n", module_name.c_str(), pattern_name.c_str()); // todo: print to cs2 console
#endif
        };

        const auto error_handler = [](const std::string& module_name, const std::string& pattern_name, const xscan::pattern& pattern) -> bool {
#ifdef _DEBUG
            std::printf("failed to find %s:%s\n", module_name.c_str(), pattern_name.c_str()); // todo: print to cs2 console
#endif
            return false;
        };

        batch_scan client("client.dll", found_handler, error_handler);
        client.add("game_entity_system", "48 89 35 ? ? ? ? 48 85 F6", [this](xscan::cursor p) -> bool {
            return (p_game_entity_system = p.add(3).rip());
        });
        client.add("view_render", "48 89 05 ? ? ? ? 48 8B C8 48 85 C0", [this](xscan::cursor p) -> bool {
            return (p_view_render = p.add(3).rip());
        });
        client.add("csgo_input", "75 1F 48 8B 05 ? ? ? ? C6 80 ? ? ? ? ?", [this](xscan::cursor p) -> bool {
            return (p_csgo_input = p.add(5).rip());
        });
        client.add("local_player_controller", "E8 ? ? ? ? 48 89 45 77", [this](xscan::cursor p) -> bool {
            return (p_local_player_controller = p.add(1).rip().add(0x23 + 3).rip());
        });
        client.add("player_controller_entity_index", "E8 ? ? ? ? 48 89 45 77", [this](xscan::cursor p) -> bool {
            return (o_player_controller_entity_index = *p.add(1).rip().add(0x33 + 2).as<std::int32_t*>());
        });

        if (!client.scan()) {
            return false;
        }

        batch_scan schemasystem("schemasystem.dll", found_handler, error_handler);
        schemasystem.add("schemasystem", "48 8B CB 48 89 1D ? ? ? ?", [this](xscan::cursor p) -> bool {
            return (p_schema_system = p.add(6).rip());
        });

        if (!schemasystem.scan()) {
            return false;
        }

        batch_scan inputsystem("inputsystem.dll", found_handler, error_handler);
        inputsystem.add("inputsystem", "48 8B 0D ? ? ? ? 49 8B D8", [this](xscan::cursor p) -> bool {
            return (p_input_system = p.add(3).rip());
        });
        inputsystem.add("apply_input_stack", "4C 89 34 E8 E8 ? ? ? ?", [this](xscan::cursor p) -> bool {
            return (f_apply_input_stack = p.add(5).rip());
        });

        if (!inputsystem.scan()) {
            return false;
        }

        batch_scan engine2("engine2.dll", found_handler, error_handler);
        engine2.add("current_rendering_world_session", "E8 ? ? ? ? 44 89 BD ? ? ? ?", [this](xscan::cursor p) -> bool {
            return (p_current_rendering_world_session = p.add(1).rip().add(0x24 + 3).rip());
        });
        engine2.add("rendering_world_session_get_world_name", "E8 ? ? ? ? 44 89 BD ? ? ? ?", [this](xscan::cursor p) -> bool {
            return (o_rendering_world_session_get_world_name = *p.add(1).rip().add(0x24 + 0x15 + 2).as<std::int32_t*>());
        });
        engine2.add("input_service", "74 30 48 8B 0D ? ? ? ? 48 8B 01", [this](xscan::cursor p) -> bool {
            return (p_input_service = p.add(5).rip());
        });

        if (!engine2.scan()) {
            return false;
        }

        batch_scan rendersystemdx11("rendersystemdx11.dll", found_handler, error_handler);
        rendersystemdx11.add("render_device_mgr", "48 8D 86 ? ? ? ? 48 89 44 24 ?", [this](xscan::cursor p) -> bool {
            o_render_device_dx11_device = *p.add(15).as<std::int32_t*>();
            o_render_device_dx11_device_context = *p.add(3).as<std::int32_t*>();
            return (p_render_device_mgr = p.add(88).rip());
        });
        rendersystemdx11.add("render_device", "48 8B 0D ? ? ? ? 44 8B CB", [this](xscan::cursor p) -> bool {
            return (p_render_device = p.add(3).rip());
        });
        rendersystemdx11.add("render_device_dx11_swap_chain_list", "33 C0 EB 24 48 8D 8E", [this](xscan::cursor p) -> bool {
            return (o_render_device_dx11_swap_chain_list = *p.add(7).as<std::int32_t*>());
        });
        rendersystemdx11.add("swap_chain_dx11_swap_chain", "48 8D BB ? ? ? ? 03 43 38", [this](xscan::cursor p) -> bool {
            return (o_swap_chain_dx11_swap_chain = *p.add(3).as<std::int32_t*>());
        });

        if (!rendersystemdx11.scan()) {
            return false;
        }

        return true;
    }
}
