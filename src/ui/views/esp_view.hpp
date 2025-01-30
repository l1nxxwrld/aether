#pragma once

namespace aether {
	namespace cs2 {
		class C_CSPlayerPawn;
		class CCSPlayerController;
	}

	class esp_view {
	public:
		esp_view(ImFont* esp_font);

		void render();

	protected:
		void render_player_esp();

		void draw_snapline(cs2::C_CSPlayerPawn* player_pawn);
		void draw_hitboxes(cs2::C_CSPlayerPawn* player_pawn);
		void draw_bounding_box(cs2::CCSPlayerController* player, cs2::C_CSPlayerPawn* player_pawn);

	protected:
		ImFont* m_esp_font;
	};
}
