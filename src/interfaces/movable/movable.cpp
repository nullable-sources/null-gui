#include <null-gui.h>

namespace null::gui::interfaces {
	void i_movable::on_mouse_move() {
		i_container::on_mouse_move();

		if(state & e_widget_state::active && !style.moving_only_by_titlebar) handle_moving();
	}

	void i_movable::on_mouse_key_down(const input::e_key_id& key) {
		i_container::on_mouse_key_down(key);

		if(!style.moving_only_by_titlebar) start_moving();
	}

	void i_movable::on_child_event_handled(const e_widget_event& event, i_widget* child) {
		i_widget::on_child_event_handled(event, child);

		if(child != title_bar || !style.moving_by_titlebar) return;
		switch(event) {
			case e_widget_event::mouse_key_down: start_moving(); break;
			case e_widget_event::mouse_move: if(child->state & e_widget_state::active) handle_moving(); break;
		}
	}
}