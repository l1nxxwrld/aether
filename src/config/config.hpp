#pragma once

namespace aether {
	class context;

	class config {
	public:
		config(context& cfg);

		bool aimbot{ false };
		bool show_aimbot_fov{ false };
		float aimbot_fov{ 10.0f };

		bool esp{ false };
		bool snaplines{ false };

	protected:
		context& m_cfg;
	};
}
