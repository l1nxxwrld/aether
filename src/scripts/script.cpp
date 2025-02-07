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
#include "../math/vec2.hpp"
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
		
		table.set_function("clear_output", []() {
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

		callbacks.set_function("clear", [this](const sol::variadic_args& args) {
			if (args.size() == 0) {
				m_on_pre_ui.clear();
				m_on_post_ui.clear();
			}
			else if (args.size() == 1 && args.get_type() == sol::type::string) {
				if (args.get<std::string>(0) == "pre_ui") {
					m_on_pre_ui.clear();
				}
				else if (args.get<std::string>(0) == "post_ui") {
					m_on_post_ui.clear();
				}
			}
		});

		auto mem{ table["memory"].get_or_create<sol::table>() };

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
			sol::constructors<qangle(), qangle(float x, float y, float z)>(),
			"x", &qangle::x,
			"y", &qangle::y,
			"z", &qangle::z
		);

		table.new_usertype<vec2>("vec2",
			sol::constructors<vec2(), vec2(float x, float y)>(),
			"x", &vec2::x,
			"y", &vec2::y
		);

		table.new_usertype<vec3>("vec3",
			sol::constructors<vec3(), vec3(float x, float y, float z)>(),
			"x", &vec3::x,
			"y", &vec3::y,
			"z", &vec3::z
		);

		table.new_usertype<vec4>("vec4",
			sol::constructors<vec4(), vec4(float x, float y, float z, float w)>(),
			"x", &vec4::x,
			"y", &vec4::y,
			"z", &vec4::z,
			"w", &vec4::w
		);

		table.new_usertype<cs2::CSkeletonInstance>("SkeletonInstance",
			"bone_count", &cs2::CSkeletonInstance::bone_count,
			"get_bone_position", [](cs2::CSkeletonInstance* skeleton, std::int32_t index) {
				return skeleton->get_bone(index).position.xyz();
			}
		);

		table.new_usertype<cs2::CBodyComponentBaseAnimGraph>("BodyComponentBaseAnimGraph",
			"skeleton", &cs2::CBodyComponentBaseAnimGraph::skeleton
		);

		table.new_usertype<cs2::C_BaseEntity>("BaseEntity",
			"address", [](const cs2::C_BaseEntity* entity) { return reinterpret_cast<std::uintptr_t>(entity); },
			"from_address", [](std::uintptr_t address) { return reinterpret_cast<cs2::C_BaseEntity*>(address); },
			"as_player_pawn", [](cs2::C_BaseEntity* entity) { return reinterpret_cast<cs2::C_CSPlayerPawn*>(entity); },
			"as_player_controller", [](cs2::C_BaseEntity* entity) { return reinterpret_cast<cs2::CCSPlayerController*>(entity); },
			"anim_graph", &cs2::C_CSPlayerPawn::anim_graph,
			"entity_type_name", &cs2::C_BaseEntity::get_entity_type_name,
			"is_dormant", &cs2::C_BaseEntity::is_dormant,
			"collision_property", &cs2::C_BaseEntity::collision_property,
			"health", &cs2::C_BaseEntity::health,
			"flags", &cs2::C_BaseEntity::flags,
			"team_number", &cs2::C_BaseEntity::team_number,
			"entity_index", &cs2::C_BaseEntity::entity_index
		);

		table.new_usertype<cs2::CCSPlayerController>("PlayerController",
			"get_local_player", cs2::CCSPlayerController::get_local_player,
			"base_entity", [](cs2::CCSPlayerController* player) { return reinterpret_cast<cs2::C_BaseEntity*>(player); },
			"address", [](const cs2::CCSPlayerController* player) { return reinterpret_cast<std::uintptr_t>(player); },
			"pawn", [](const cs2::CCSPlayerController* player) { return reinterpret_cast<std::uintptr_t>(player->get_pawn()); },
			"csgoid", &cs2::CCSPlayerController::get_csgoid,
			"name", &cs2::CCSPlayerController::get_name,
			"pawn_index", &cs2::CCSPlayerController::pawn_index,
			"is_alive", &cs2::CCSPlayerController::is_alive
		);

		table.set_function("get_local_player", cs2::CCSPlayerController::get_local_player);

		table.new_usertype<cs2::C_CSPlayerPawn>("PlayerPawn",
			"base_entity", [](cs2::CCSPlayerController* player) { return reinterpret_cast<cs2::C_BaseEntity*>(player); },
			"address", [](const cs2::C_CSPlayerPawn* pawn) { return reinterpret_cast<std::uintptr_t>(pawn); },
			"view_angle", &cs2::C_CSPlayerPawn::view_angle,
			"abs_origin", &cs2::C_CSPlayerPawn::abs_origin,
			"eye_origin", &cs2::C_CSPlayerPawn::eye_origin,
			"view_direction", &cs2::C_CSPlayerPawn::view_direction,
			"current_weapon", &cs2::C_CSPlayerPawn::current_weapon,
			"is_spotted", [](const cs2::C_CSPlayerPawn* pawn) {
				return pawn->spotted_state().spotted;
			}
		);

		table.set_function("get_local_pawn", cs2::C_CSPlayerPawn::get_local_player);

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

		auto ui_table{ table["ui"].get_or_create<sol::table>() };
		
		ui_table.set_function("is_open", [] {
			return context::get().ui()->is_open();
		});

		ui_table.set_function("set_open", [](bool open) {
			return context::get().ui()->set_open(open);
		});

		ui_table.set_function("show_input_stack_system", [] {
			return context::get().ui()->config().show_input_stack_system = true;
		});

		ui_table.set_function("hide_input_stack_system", [] {
			return context::get().ui()->config().show_input_stack_system = false;
		});

		ui_table.set_function("get_delta_time", [] {
			return ImGui::GetIO().DeltaTime;
		});

		ui_table.set_function("get_delta_time", [] {
			const auto& display_size{ ImGui::GetIO().DisplaySize };
			return vec2{ display_size.x, display_size.y };
		});

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
