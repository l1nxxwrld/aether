#include "config.hpp"

namespace aether {
	config::config(context& cfg)
		: m_cfg(cfg) {

		this->aimbot = std::make_unique<aimbot_state>();
		this->esp = std::make_unique<esp_state>();
	}
}
