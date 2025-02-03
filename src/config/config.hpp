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
	};

	class config {
	public:
		config() = default;

		aimbot_config aimbot;
		esp_config esp;
	};
}
