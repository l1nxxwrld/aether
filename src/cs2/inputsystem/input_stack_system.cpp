#include "../interface_mgr.hpp"
#include "../utils.hpp"
#include "input_stack_system.hpp"

namespace aether::cs2 {
	CInputStackSystem* CInputStackSystem::get() {
		static const auto instance{ g_interfaces->get<CInputStackSystem>("inputsystem.dll", "InputStackSystemVersion001") };
		return instance;
	}

	std::int32_t CInputStackSystem::num_entries() const {
		return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x28);
	}

	input_stack_entry* CInputStackSystem::get_entry(std::int32_t index) const {
		return (*reinterpret_cast<input_stack_entry***>(reinterpret_cast<std::uintptr_t>(this) + 0x30))[index];
	}

	input_stack_entry* CInputStackSystem::get_entry(const char* name) const {
		for (std::int32_t i{ 0 }; i < this->num_entries(); i++) {
			if (const auto entry{ this->get_entry(i) }; std::strcmp(entry->name, name) == 0) {
				return entry;
			}
		}
		return nullptr;
	}

	input_stack_entry* CInputStackSystem::push(const char* name, std::uint32_t state) {
		return invoke<input_stack_entry*>(this, 11, state);
	}

	void CInputStackSystem::pop() {
		return invoke<void>(this, 12);
	}

	void CInputStackSystem::apply(std::uint32_t state) {
		return invoke<void>(this, 28, state);
	}
}
