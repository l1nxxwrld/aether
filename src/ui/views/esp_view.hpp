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
		void draw_player_esp(cs2::CCSPlayerController* player, cs2::C_CSPlayerPawn* player_pawn);

	protected:
		ImFont* m_esp_font;
	};
}
