#pragma once
#include <cstdint>

namespace aether {
	class engine_cvar abstract {
	public:
		static engine_cvar* get() { return nullptr; }

		bool register_variable(const char* variable_name, std::uint64_t a2, std::uint64_t a3, std::uint64_t a4);

		bool register_command(std::uint64_t a1, std::uint64_t a2, std::uint64_t a3);
	};
}
