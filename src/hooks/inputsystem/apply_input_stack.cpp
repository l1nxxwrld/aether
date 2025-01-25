#include "../../cs2/inputsystem/input_stack_system.hpp"
#include "../../ui/ui_manager.hpp"
#include "../../context.hpp"

namespace aether {
    void __fastcall context::apply_input_stack(cs2::CInputStackSystem* iss, std::uint32_t state) {

        const auto& ui{ context::get().ui() };
        if (!ui or !ui->is_open()) {
            return context::get().m_apply_input_stack(iss, state);
        }

        auto& engine{ *iss->get_entry("engine2") };
        auto& mouse_control{ *iss->get_entry("Mouse Control") };

        const auto original_mouse_control_enabled{ mouse_control.enabled };
        const auto original_engine_relative_mouse_mode{ engine.is_relative_mouse_mode };
        const auto original_engine_cursor_clip{ engine.pcursor_clip };

        mouse_control.enabled = true;
        engine.is_cursor_visible = true;
        engine.is_relative_mouse_mode = false;
        engine.pcursor_clip = nullptr;

        context::get().m_apply_input_stack(iss, state);

        mouse_control.enabled = original_mouse_control_enabled;
        engine.is_relative_mouse_mode = original_engine_relative_mouse_mode;
        engine.pcursor_clip = original_engine_cursor_clip;
    }
}
