#include "../memory_mgr.hpp"
#include "schema_system.hpp"

namespace aether::cs2 {
	CSchemaSystem* CSchemaSystem::get() {
		return *g_memory->p_schema_system;
	}

	CSchemaSystemTypeScope& CSchemaSystem::get_type_scope(std::size_t i) {
		return *reinterpret_cast<CSchemaSystemTypeScope**>(
			reinterpret_cast<std::uintptr_t>(this) +
			0x190
		)[i];
	}

	std::size_t CSchemaSystem::type_scope_count() {
		return *reinterpret_cast<std::size_t*>(
			reinterpret_cast<std::uintptr_t>(this) +
			0x188
		);
	}
}
