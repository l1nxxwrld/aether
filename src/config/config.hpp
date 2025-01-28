#pragma once
#include <memory>
#include "../math/qangle.hpp"
#include <imgui.h> // for ImVec4


namespace aether {
	class context;

	class aimbot_state {
	public:
		bool enabled{ false };
		bool show_fov{ false };
		float fov{ 3.0f };
		float smoothing{ 5.0f }; // less = more snappy | more = more smooth
		bool show_box = false;
	};

	class esp_state {
	public:
		bool enabled{ false };
		bool show_snaplines{ false };

		// box esp
		bool show_box{ false };
		bool box_outline{ true };           
		bool box_fill{ false };            
		ImVec4 box_color{ 1.0f, 0.0f, 0.0f, 1.0f };
		float box_thickness{ 1.5f };
		float outline_thickness{ 1.5f };

		// health bar
		bool show_health_bar{ false };
		bool healthbar_outline{ true };     
		float healthbar_thickness{ 3.0f };
		float healthbar_outline_thickness{ 1.0f };
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
