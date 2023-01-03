#pragma once
#include <interfaces/container/container.h>

namespace null::gui::interfaces {
	class i_movable : public virtual i_container {
	public:
		struct style_t {
			bool moving_by_titlebar{ true };
			bool moving_only_by_titlebar{ false };
		} style{ };

	protected:
		vec2_t clicked_offset{ };

	protected:
		virtual void start_moving() { clicked_offset = input::mouse.pos - pos; }
		virtual void handle_moving() { pos = input::mouse.pos - clicked_offset; }

	public:
		virtual void on_mouse_move() override;
		virtual void on_mouse_key_down(const input::e_key_id& key) override;
		virtual void on_child_event_handled(const e_widget_event& event, i_widget* child) override;
	};
}