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

        if (cfg.aimbot && cfg.show_aimbot_fov) {
            ImGui::GetBackgroundDrawList()->AddCircleFilled(
                ImGui::GetIO().DisplaySize * 0.5f,
                ImGui::GetIO().DisplaySize.x * (cfg.aimbot_fov / 90.0f) * 0.5f,
                ImColor(120, 81, 169, 75), 100
            );
            ImGui::GetBackgroundDrawList()->AddCircle(
                ImGui::GetIO().DisplaySize * 0.5f,
                ImGui::GetIO().DisplaySize.x * (cfg.aimbot_fov / 90.0f) * 0.5f,
                ImColor(120, 81, 169, 200), 100, 1.0f
            );
        }

        if (!ui.is_open()) {
            return;
        }

        ImGui::SetNextWindowPos({ 700.0f, 100.0f }, ImGuiCond_Once);
        ImGui::SetNextWindowSize({ 250.0f, 250.0f }, ImGuiCond_Once);
        if (ImGui::Begin("Aimbot")) {
            ImGui::Checkbox("Enabled", &cfg.aimbot);
            ImGui::Checkbox("Show FOV", &cfg.show_aimbot_fov);
            ImGui::SliderFloat("FOV", &cfg.aimbot_fov, 5.0f, 90.0f, "%.0f");
        }
        ImGui::End();
	}
}
