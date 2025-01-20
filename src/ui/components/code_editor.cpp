
#include <imgui_internal.h>
#include "code_editor.hpp"

namespace aether {
	ui_code_editor::ui_code_editor(
		ImFont* content_font,
		ImFont* output_font
	) : m_content_font(content_font),
		m_output_font(output_font) {}

	void ui_code_editor::render(const char* str_id) {

		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 10.0f);
		if (ImGui::BeginChild(str_id)) {

			const auto window_content_size{ ImGui::GetWindowContentRegionMax() };

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 10.0f, 10.0f });

			ImGui::PushFont(m_output_font);

			const float output_height{
				(ImGui::GetStyle().FramePadding.y * 2.0f) +
				(ImGui::GetFontSize() * 4.0f)
			};

			const ImVec2 content_size{
				window_content_size.x,
				window_content_size.y
				- (ImGui::GetStyle().ItemSpacing.y * 2.0f)
				- output_height
			};

			ImGui::PushFont(m_content_font); 
			ImGui::PushStyleColor(ImGuiCol_FrameBg, m_content_bg.Value);
			ImGui::InputTextMultiline("##content", &m_buffer, content_size);
			ImGui::PopStyleColor(1);
			ImGui::PopFont();

			ImGui::GetWindowDrawList()->AddLine(
				ImGui::GetWindowPos() + ImGui::GetCursorPos(),
				ImGui::GetWindowPos() + ImGui::GetCursorPos() + ImVec2{ ImGui::GetContentRegionAvail().x, 0.0f },
				ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Separator)), 1.0f);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, m_output_bg.Value);

			ImGui::InputTextMultiline("##output", &m_output_buffer, ImVec2{ ImGui::GetContentRegionAvail().x, output_height }, ImGuiInputTextFlags_ReadOnly);

			ImGui::PopStyleColor(1);
			ImGui::PopFont();

			ImGui::PopStyleVar(1);
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(1);
	}

	const std::string& ui_code_editor::buffer() const {
		return m_buffer;
	}


	std::string& ui_code_editor::output_buffer() {
		return m_output_buffer;
	}

	const std::string& ui_code_editor::output_buffer() const {
		return m_output_buffer;
	}
}
