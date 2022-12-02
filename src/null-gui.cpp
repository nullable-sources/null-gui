#include <null-gui.h>

namespace null::gui {
	namespace impl {
		std::string_view format_widget_name(std::string_view name) {
			if(auto finded{ std::ranges::search(name, std::string_view{ "##" }) }; !finded.empty())
				return name.substr(0, std::distance(name.begin(), finded.begin()));
			return name;
		}
	}

	void begin_frame() {
		if(c_window::hovered && !rect_t{ c_window::hovered->pos, c_window::hovered->pos + c_window::hovered->size }.contains(input::mouse.pos))
			c_window::hovered = nullptr;

		impl::hovered_widget.clear();
		impl::last_widget.clear();
		c_window::windows_handle();

		std::ranges::for_each(c_window::windows, [](const std::shared_ptr<c_window>& window) { window->draw_list->reset(); });
	}

	void end_frame() {
		std::ranges::for_each(c_window::windows, [](const std::shared_ptr<c_window>& window) { render::fast_lists.push_back(window->draw_list.get()); });
	}

	void tooltip(const std::function<void()>& func) {
		if(c_window::active || !impl::active_widget.empty() || !c_window::current->can_open_tooltip()) return;
		bool close{ impl::hovered_widget == impl::last_widget };
		impl::push_var(&style::window::padding, vec2_t{ 5.f }); {
			if(begin_window("##tooltip", input::mouse.pos + 10.f, 0.f, e_window_flags{ -e_window_flags::no_move | -e_window_flags::no_title_bar | -e_window_flags::popup | -e_window_flags::set_pos | -e_window_flags::auto_size }, &close)) {
				c_window::current->focus();
				func();
				end_window();
			}
		} impl::pop_var();
	}

	void new_line() {
		c_window::current->add_widget("", vec2_t(0.f, style::new_line_size));
	}

	void same_line() {
		c_window::current->autopositioning.current_pos = c_window::current->autopositioning.prev_pos + vec2_t(style::window::item_spacing, 0.f);
		c_window::current->in_same_line = true;
	}

	int wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
		switch(msg) {
			case WM_CHAR: {
				if(impl::c_text_input * text_input{ impl::c_text_input::find(impl::active_widget) })
					text_input->add_char(w_param);
			} break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN: {
				if(impl::c_text_input* text_input{ impl::c_text_input::find(impl::active_widget) })
					text_input->key_down((input::e_key_id)w_param);
			} break;

			case WM_MOUSEWHEEL: {
				if(c_window::hovered)
					c_window::hovered->scrollbar_data.target += input::mouse.wheel.y * 20.f;
			} break;

			default: return -1;
		}

		return 1;
	}
}