#include <null-gui.h>

namespace null::gui {
	//@credits: https://github.com/KN4CK3R/OSHGui
	int wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
		e_widget_event widget_msg{ };

		switch(msg) { //@note: transform WM message in e_widget_msg
			case WM_MOUSEMOVE:
				widget_msg = e_widget_event::mouse_move;
				break;
			
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
				widget_msg = e_widget_event::mouse_key_down;
			break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
				widget_msg = e_widget_event::mouse_key_up;
				break;

			case WM_MOUSEWHEEL:
				widget_msg = e_widget_event::mouse_wheel;
				break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
				widget_msg = e_widget_event::key_down;
				break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
				widget_msg = e_widget_event::key_up;
				break;

			case WM_CHAR:
				widget_msg = e_widget_event::char_add;
				break;

			default: return -1;
		}

		switch(msg) {
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				w_param |= (int)input::e_key_id::mouse_left << 8;
				break;

			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
				w_param |= (int)input::e_key_id::mouse_right << 8;
				break;

			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
				w_param |= (int)input::e_key_id::mouse_middle << 8;
				break;
		}

		if(interfaces::i_widget* widget{ interfaces::i_widget::widgets[e_widget_state::active] ? interfaces::i_widget::widgets[e_widget_state::active] : interfaces::i_widget::widgets[e_widget_state::focused] }) {
			if(std::vector<interfaces::i_widget*> node{ widget->node.parent_node() }; std::ranges::find_if(node, [](interfaces::i_widget* parent) { return !(parent->flags & e_widget_flags::visible); }) == node.end()) {
				if(widget->event_handling(widget_msg, w_param, l_param)) {
					if(widget->node.parent) widget->node.parent->on_child_event_handled(widget_msg, widget);
					return 1;
				}
				for(interfaces::i_widget* parent : widget->node.parent_node() | std::views::reverse) {
					if(parent->event_handling(widget_msg, w_param, l_param)) {
						if(parent->node.parent) parent->node.parent->on_child_event_handled(widget_msg, parent);
						return 1;
					}
				}
			}
		}

		for(std::shared_ptr<c_window> window : c_window::window_stack | std::views::reverse) {
			if(window->event_handling(widget_msg, w_param, l_param))
				return 1;
		}

		switch(widget_msg) {
			case e_widget_event::mouse_key_down: {
				if(interfaces::i_widget::widgets[e_widget_state::focused])
					interfaces::i_widget::widgets[e_widget_state::focused]->on_lost_focus(nullptr);
			} break;

			case e_widget_event::mouse_move: {
				if(interfaces::i_widget::widgets[e_widget_state::hovered])
					interfaces::i_widget::widgets[e_widget_state::hovered]->on_mouse_exit();
			} break;
		}

		return -1;
	}
}