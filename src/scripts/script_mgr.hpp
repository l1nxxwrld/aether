#pragma once
#include <vector>
#include <memory>
#include <mutex>

namespace aether {

	class script;

	class script_mgr {
	public:
		script_mgr() = default;

		bool init();

		void add_script(const std::shared_ptr<script>& script);

		void on_pre_ui();
		void on_post_ui();

	protected:
		bool m_initialized{ false };

		std::vector<std::shared_ptr<script>> m_scripts;
	};
}
