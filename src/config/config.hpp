#pragma once

namespace aether {
	class context;

	class config {
	public:
		config(context& cfg);

		bool aimbot{ false };

		bool esp{ false };
		bool snaplines{ false };

	protected:
		context& m_cfg;
	};
}
