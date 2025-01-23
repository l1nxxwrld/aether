#include "../cs2/memory_mgr.hpp"
#include "../cs2/client/weapon.hpp"
#include "../cs2/client/player_pawn.hpp"
#include "../cs2/client/player_controller.hpp"
#include "../cs2/client/entity_system.hpp"
#include "../cs2/schemasystem/schema_system.hpp"
#include "../context.hpp"
#include "../config/config.hpp"
#include "../ui/ui_manager.hpp"
#include "../ui/components/code_editor.hpp"
#include "script.hpp"

namespace aether {
	const char* lua_script::script_language() const {
		return "lua";
	}

	lua_script::lua_script(const std::string& code)
		: m_state(std::make_unique<sol::state>()), m_code(code) {}

	bool lua_script::init() {

		m_state->open_libraries(sol::lib::base,
			sol::lib::package, sol::lib::string,
			sol::lib::os, sol::lib::math,
			sol::lib::table, sol::lib::debug,
			sol::lib::bit32, sol::lib::io,
			sol::lib::utf8);

		if (!m_state->safe_script(m_code).valid()) {
			return false;
		}

		sol::table table{ m_state->globals() };

		table.set_function("print", [](const std::string& text) {
			context::get().ui()->editor()->output_buffer() += text + "\n";
		});
		
		table.set_function("clear", []() {
			context::get().ui()->editor()->output_buffer().clear();
		});

		auto callbacks{ table["callbacks"].get_or_create<sol::table>() };

		callbacks.set_function("add", [this](
			const std::string& callback_type,
			const sol::variadic_args& callback) -> bool {
		
			if (callback_type == "pre_ui"
				&& callback.size() == 1
				&& callback.get_type(0) == sol::type::function) {

				m_on_pre_ui.push_back(callback.get<on_pre_ui_t>(0));
				return true;
			}
			else if (callback_type == "post_ui"
				&& callback.size() == 1
				&& callback.get_type(0) == sol::type::function) {

				m_on_post_ui.push_back(callback.get<on_post_ui_t>(0));
				return true;
			}

			return false;
		});

		auto mem{ table["mem"].get_or_create<sol::table>() };

		mem.set_function("read_u8", [](std::uint64_t address) -> std::uint8_t { return *reinterpret_cast<std::uint8_t*>(address);  });
		mem.set_function("read_u16", [](std::uint64_t address) -> std::uint16_t { return *reinterpret_cast<std::uint16_t*>(address); });
		mem.set_function("read_u32", [](std::uint64_t address) -> std::uint32_t { return *reinterpret_cast<std::uint32_t*>(address); });
		mem.set_function("read_u64", [](std::uint64_t address) -> std::uint64_t { return *reinterpret_cast<std::uint64_t*>(address); });
		mem.set_function("read_i8", [](std::uint64_t address) -> std::int8_t { return *reinterpret_cast<std::int8_t*>(address);   });
		mem.set_function("read_i16", [](std::uint64_t address) -> std::int16_t { return *reinterpret_cast<std::int16_t*>(address);  });
		mem.set_function("read_i32", [](std::uint64_t address) -> std::int32_t { return *reinterpret_cast<std::int32_t*>(address);  });
		mem.set_function("read_i64", [](std::uint64_t address) -> std::int64_t { return *reinterpret_cast<std::int64_t*>(address);  });
		mem.set_function("read_str", [](std::uint64_t address, std::size_t max = std::string::npos) -> std::string {
			std::string result;
			for (std::size_t i{ 0 }; i < max; i++) {
				if (const auto c{ reinterpret_cast<char*>(address)[i] }) {
					result.push_back(c);
				}
				else {
					break;
				}
			}
			return result;
			});

		table.new_usertype<qangle>("qangle",
			sol::constructors<qangle(), qangle(float x, float y)>(),
			"x", &qangle::x,
			"y", &qangle::y
		);

		table.new_usertype<vec3>("vec3",
			sol::constructors<vec3(), vec3(float x, float y)>(),
			"x", &vec3::x,
			"y", &vec3::y,
			"z", &vec3::z
		);

		table.new_usertype<cs2::C_BaseEntity>("BaseEntity",
			"address", [](const cs2::C_BaseEntity* entity) { return reinterpret_cast<std::uintptr_t>(entity); },
			"from_address", [](std::uintptr_t address) { return reinterpret_cast<cs2::C_BaseEntity*>(address); },
			"as_player_pawn", [](cs2::C_BaseEntity* entity) { return reinterpret_cast<cs2::C_CSPlayerPawn*>(entity); },
			"as_player_controller", [](cs2::C_BaseEntity* entity) { return reinterpret_cast<cs2::CCSPlayerController*>(entity); },
			"entity_index", &cs2::C_CSPlayerPawn::entity_index,
			"get_entity_type_name", &cs2::C_BaseEntity::get_entity_type_name
		);

		table.new_usertype<cs2::CCSPlayerController>("PlayerController",
			"get_local", cs2::CCSPlayerController::get_local_player,
			"base_player", [](cs2::CCSPlayerController* player) { return reinterpret_cast<cs2::C_BaseEntity*>(player); },
			"address", [](const cs2::CCSPlayerController* player) { return reinterpret_cast<std::uintptr_t>(player); },
			"get_pawn", [](const cs2::CCSPlayerController* player) { return reinterpret_cast<std::uintptr_t>(player->get_pawn()); },
			"pawn_index", &cs2::CCSPlayerController::pawn_index
		);

		table.new_usertype<cs2::C_CSPlayerPawn>("PlayerPawn",
			"get_local", cs2::C_CSPlayerPawn::get_local_player,
			"base_player", [](cs2::CCSPlayerController* player) { return reinterpret_cast<cs2::C_BaseEntity*>(player); },
			"address", [](const cs2::C_CSPlayerPawn* pawn) { return reinterpret_cast<std::uintptr_t>(pawn); },
			"get_name", &cs2::C_CSPlayerPawn::get_entity_type_name,
			"view_angle", &cs2::C_CSPlayerPawn::view_angle,
			"abs_origin", &cs2::C_CSPlayerPawn::abs_origin,
			"eye_origin", &cs2::C_CSPlayerPawn::eye_origin,
			"view_direction", &cs2::C_CSPlayerPawn::view_direction,
			"current_weapon", &cs2::C_CSPlayerPawn::current_weapon
		);

		table.new_usertype<cs2::C_CSWeaponBaseGun>("Weapon",
			"address", [](const cs2::C_CSWeaponBaseGun* wpn) { return reinterpret_cast<std::uintptr_t>(wpn); }
		);

		table.new_usertype<cs2::CGameEntitySystem>("GameEntitySystem",
			"get", cs2::CGameEntitySystem::get,
			"address", [](const cs2::CGameEntitySystem* sys) { return reinterpret_cast<std::uintptr_t>(sys); },
			"get_entity", &cs2::CGameEntitySystem::get_entity<cs2::C_BaseEntity>
		);

		table.new_usertype<cs2::CSchemaSystem>("SchemaSystem",
			"get", cs2::CSchemaSystem::get,
			"address", [](const cs2::CSchemaSystem* sys) { return reinterpret_cast<std::uintptr_t>(sys); }
		);

		return true;
	}

	bool lua_script::start() {
		m_running = true;
		return true;
	}

	bool lua_script::stop() {
		m_running = false;
		return true;
	}

	bool lua_script::is_running() const {
		return m_running;
	}

	void lua_script::on_pre_ui() {
		for (const auto& callback : m_on_pre_ui) {
			callback();
		}
	}

	void lua_script::on_post_ui() {
		for (const auto& callback : m_on_post_ui) {
			callback();
		}
	}

	std::string lua_script::submit(const std::string_view& code) {
		try {
			const auto result{ m_state->safe_script(code) };
			return result.get_type() == sol::type::string ?
				result : std::string("");
		}
		catch (const sol::error& err) {
			return err.what() + std::string("\n");
		}
	}
}
