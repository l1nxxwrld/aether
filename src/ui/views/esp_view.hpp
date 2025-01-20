#pragma once

namespace aether {
	class esp_view {
	public:
		esp_view();

		void render();

	protected:
		void render_player_esp();
	};
}
