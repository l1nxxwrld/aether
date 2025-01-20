#include "../interface_mgr.hpp"
#include "input_stack_system.hpp"

namespace aether::cs2 {
	CInputStackSystem* CInputStackSystem::get() {
		static const auto instance{ g_interfaces->get<CInputStackSystem>("inputsystem.dll", "InputStackSystemVersion001") };
		return instance;
	}
}
