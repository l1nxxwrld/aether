#include <imgui.h>
#include "../../context.hpp"
#include "../../ui/ui_manager.hpp"
#include "../../config/config.hpp"
#include "aimbot_view.hpp"

namespace aether {
	aimbot_view::aimbot_view() {}

	void aimbot_view::render() {
        auto& ui{ *context::get().ui() };
        auto& cfg{ *context::get().cfg() };

        if (!ui.is_open()) {
            return;
        }

        ImGui::SetNextWindowPos({ 700.0f, 100.0f }, ImGuiCond_Once);
        ImGui::SetNextWindowSize({ 250.0f, 250.0f }, ImGuiCond_Once);
        if (ImGui::Begin("Aimbot")) {
            ImGui::Checkbox("Enabled", &cfg.aimbot);
        }
        ImGui::End();
	}
}
