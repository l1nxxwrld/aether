#pragma once
#include <mutex>
#include <sol/sol.hpp>

namespace aether {
	class script {
	public:
		virtual ~script() = default;

		virtual const char* script_language() const = 0;

		virtual bool init() = 0;

		virtual bool start() = 0;

		virtual bool stop() = 0;

		virtual bool is_running() const = 0;

		virtual void on_pre_ui() = 0;

		virtual void on_post_ui() = 0;

		virtual std::string submit(const std::string_view& code) = 0;
	};

	class lua_script
		: public script {
	public:
		lua_script(const std::string& code = "");

		const char* script_language() const override;

		bool init() override;

		bool start() override;

		bool stop() override;

		bool is_running() const override;

		void on_pre_ui() override;

		void on_post_ui() override;

		std::string submit(const std::string_view& code) override;

		using on_pre_ui_t = std::function<void()>;

		using on_post_ui_t = std::function<void()>;

	protected:
		bool m_running{ false };

		std::unique_ptr<sol::state> m_state;
		std::string m_code;

		std::vector<on_pre_ui_t> m_on_pre_ui;
		std::vector<on_post_ui_t> m_on_post_ui;
	};
}
