#pragma once

namespace aether {
	class esp_view {
	public:
		esp_view();

		void render();

	protected:
		void render_player_esp();

		void draw_snapline(cs2::C_CSPlayerPawn* player_pawn);
		void draw_hitboxes(cs2::C_CSPlayerPawn* player_pawn);
		void draw_bounding_box(cs2::C_CSPlayerPawn* player_pawn);
	};
}
