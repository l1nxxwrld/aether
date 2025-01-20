#include <Zydis/Zydis.h>
#include <filesystem>
#include "tier0/platform.hpp"
#include "interface_mgr.hpp"

namespace aether {
    interface_entry* interface_list::find(
        std::string_view interface_name
    ) {
        for (interface_entry* it{ this }; it && it->name; it = it->next) {

            interface_entry* found{ it };
            for (std::size_t i{ 0 }; i < interface_name.size(); i++) {
                if (interface_name[i] != it->name[i]) {
                    found = nullptr;
                    break;
                }
            }

            if (found) {
                return found;
            }
        }

        return nullptr;
    }

    interface_list* interface_mgr::find_list(
        const HMODULE module_base
    ) const {
        const auto create_interface{ reinterpret_cast<std::uint8_t*>(
            GetProcAddress(module_base, "CreateInterface")
        ) };

        if (!create_interface) {
            return nullptr;
        }

        ZydisDecoder decoder;
        ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

        ZydisDecodedInstruction instruction;
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

        ZyanUSize offset{ 0x0 };
        ZyanUSize length{ 0x50 };
        while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(
            &decoder, create_interface + offset,
            length - offset, &instruction, operands
        ))) {
            if (instruction.mnemonic == ZYDIS_MNEMONIC_MOV
                and operands[0].type == ZYDIS_OPERAND_TYPE_REGISTER
                and operands[1].type == ZYDIS_OPERAND_TYPE_MEMORY) {

                ZyanU64 result;
                if (!ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(
                    &instruction, &operands[1],
                    reinterpret_cast<std::uintptr_t>(create_interface) + offset,
                    &result
                ))) {
                    return nullptr;
                }

                return *reinterpret_cast<interface_list**>(result);
            }

            if (instruction.meta.branch_type != ZYDIS_BRANCH_TYPE_NONE) {
                break;
            }

            length -= instruction.length;
        }

        return nullptr;
    }

    interface_list* interface_mgr::find_list(
        std::string_view module_name
    ) const {
        std::scoped_lock guard(m_mutex);
        if (auto entry{ m_lookup_cache.find(module_name) }; entry != m_lookup_cache.end()) {
            return entry->second;
        }

        const auto module_base{ GetModuleHandleA(module_name.data()) };
        if (!module_base) {
            return nullptr;
        }

        if (const auto list{ find_list(module_base) }) {

            return (m_lookup_cache[module_name] = list);
        }

        return nullptr;
    }

    interface_entry* interface_mgr::find(
        std::string_view module_name,
        std::string_view interface_name
    ) const {
        if (const auto list{ find_list(module_name) }) {
            return list->find(interface_name);
        }

        return nullptr;
    }
}
