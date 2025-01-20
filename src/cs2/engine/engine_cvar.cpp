#include "../interface_mgr.hpp"
#include "../utils.hpp"
#include "engine_cvar.hpp"

namespace aether {
	bool engine_cvar::register_variable(const char* variable_name, std::uint64_t a2, std::uint64_t a3, std::uint64_t a4) {
		return invoke<bool>(this, 0x118 / 8, variable_name, a2, a3, a4);
	}

	bool engine_cvar::register_command(std::uint64_t a1, std::uint64_t a2, std::uint64_t a3) {
		return invoke<bool>(this, 0x130 / 8, a1, a2, a3);
	}
}
