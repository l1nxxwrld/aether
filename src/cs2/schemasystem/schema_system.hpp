#pragma once
#include <cstdint>
#include <cstddef>
#include "../tier0/utlstring.hpp"

namespace aether::cs2 {
	class CSchemaSystemTypeScope {
	public:
		std::uintptr_t vftable;

		char scope_name[256];
		CSchemaSystemTypeScope* global_scope;
	};

	class CSchemaSystem {
	public:
		static CSchemaSystem* get();

		CSchemaSystemTypeScope& get_type_scope(std::size_t i);
		std::size_t type_scope_count();
	};

	class CSchemaType {
	public:
		inline const CUtlString& name() const {
			return m_name;
		}

		inline CSchemaSystemTypeScope* scope() const {
			return m_scope;
		}

		inline std::uint32_t flags() const {
			return m_flags;
		}

		inline bool is_polymorphic() const {
			return this->flags() & 1;
		}

		inline bool has_trivial_constructor() const {
			return this->flags() & 4;
		}

		inline bool has_trivial_destructor() const {
			return this->flags() & 8;
		}

	protected:
		std::uint64_t m_vftable;
		CUtlString m_name;
		CSchemaSystemTypeScope* m_scope;
		std::uint32_t m_flags;
	};

	class CSchemaType_Atomic
		: public CSchemaType {
	public:
		inline std::uint32_t type_id() const {
			return m_type_id;
		}

	protected:
		std::uint64_t m_unk_class_related_20;
		std::uint32_t m_type_id;
		std::uint16_t m_unk_2C;
		std::uint8_t m_unk_2E;
	};
}
