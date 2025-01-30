#pragma once
#include <memory>
#include "../math/qangle.hpp"

namespace aether {
	class context;

	class aimbot_state {
	public:
		bool enabled{ false };
		bool show_fov{ false };
		float fov{ 3.0f };
		float dps{ 10.0f };
	};

	class esp_state {
	public:
		bool enabled{ false };
		bool show_snaplines{ false };
		bool show_name{ false };
		bool show_box_2d{ false };
		bool show_hitboxes{ false };
	};

	class config {
	public:
		config(context& cfg);

		std::unique_ptr<aimbot_state> aimbot;
		std::unique_ptr<esp_state> esp;

	protected:
		context& m_cfg;
	};
}
