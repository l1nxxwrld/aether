#pragma once
#include <memory>
#include "../math/qangle.hpp"
#include "../math/vec2.hpp"

namespace aether {
	class context;

	class aimbot_config {
	public:
		bool enabled{ false };
		bool show_fov{ false };
		float fov{ 3.0f };
		float factor{ 0.1f };
	};

	class esp_config {
	public:
		bool enabled{ false };
		bool show_snaplines{ false };
		bool show_name{ false };
		bool show_box_2d{ false };
		bool show_health_bar{ false };
		bool show_hitboxes{ false };

		ImVec4 box_color{ 0.0f, 1.0f, 0.0f, 1.0f };
		ImVec4 box_outline_color{ 0.0f, 0.0f, 0.0f, 0.2f };
		float box_thickness{ 1.0f };
		float box_outline_thickness{ 1.0f };

		bool healthbar_outline{ false };
		ImVec4 healthbar_outline_color{ 0.0f, 0.0f, 0.0f, 0.2f };
		float healthbar_thickness{ 2.0f };
		float healthbar_outline_thickness{ 0.5f };

		float healthbar_offset_x{ 4.0f };
		float name_offset_y{ -4.0f };
	};

	class config {
	public:
		config() = default;

		aimbot_config aimbot;
		esp_config esp;
	};
}
