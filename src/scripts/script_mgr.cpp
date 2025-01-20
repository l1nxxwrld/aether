#include "script.hpp"
#include "script_mgr.hpp"

namespace aether {
	bool script_mgr::init() {
		for (const auto& script : m_scripts) {
			script->init();
		}

		m_initialized = true;
		return true;
	}
	void script_mgr::add_script(const std::shared_ptr<script>& script) {
		m_scripts.push_back(script);
		if (m_initialized) {
			script->init();
		}
	}

	void script_mgr::on_pre_ui() {
		for (const auto& script : m_scripts) {
			script->on_pre_ui();
		}
	}
	void script_mgr::on_post_ui() {
		for (const auto& script : m_scripts) {
			script->on_post_ui();
		}
	}
}
