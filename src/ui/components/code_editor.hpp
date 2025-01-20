#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>
#include <memory>
#include <functional>

namespace aether {
	class ui_code_editor {
	public:
		ui_code_editor(
			ImFont* content_font,
			ImFont* output_font
		);

		void render(const char* str_id);

		const std::string& buffer() const;
		std::string& output_buffer();
		const std::string& output_buffer() const;

	protected:
		ImColor m_content_bg{ 0, 0, 0, 0 };
		ImColor m_output_bg{ 0, 0, 0, 0 };

		ImFont* m_content_font;
		ImFont* m_output_font;

		std::string m_buffer;
		std::string m_output_buffer;
	};
}
