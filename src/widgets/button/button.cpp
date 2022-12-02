#include <null-gui.h>

namespace null::gui {
	bool button(std::string_view name, const vec2_t& custom_size) {
		std::string_view text{ impl::format_widget_name(name) };

		vec2_t min_size{ render::c_font::get_current_font()->calc_text_size(text) + style::button::padding * 2 };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(math::max(vec2_t{ style::items_size_full_window ? std::max(min_size.x + c_window::current->get_scroll_thickness(), c_window::current->get_window_size_with_padding().x) : min_size.x + c_window::current->get_scroll_thickness(), min_size.y }, custom_size)) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return false;

		bool hovered{ }, pressed{ };
		bool active{ impl::behaviors::button(name, widget_rect, hovered, pressed) };

		c_window::current->draw_list->draw_rect_filled(widget_rect, hovered || pressed ? pressed ? style::main_color : style::button::box_hovered : style::button::box, style::button::rounding);
		c_window::current->draw_list->draw_text(text, widget_rect.center(), style::text, render::e_text_flags::aligin_center);

		return active;
	}

	namespace impl::behaviors {
		bool button(std::string_view name, const rect_t& widget_rect, bool& hovered, bool& pressed) {
			if(c_window::current->can_use_widget(name, widget_rect)) {
				hovered_widget = name;

				if(input::keys[input::e_key_id::mouse_left].is_up()) hovered = true;
				if(input::keys[input::e_key_id::mouse_left].is_pressed()) active_widget = name;
			}

			if(active_widget == name) {
				if(input::keys[input::e_key_id::mouse_left].is_down()) pressed = true;
				if(widget_rect.contains(input::mouse.pos) && c_window::hovered == c_window::current && input::keys[input::e_key_id::mouse_left].is_released()) {
					active_widget.clear();
					return true;
				}
			}

			if(input::keys[input::e_key_id::mouse_left].is_up() && active_widget == name) {
				active_widget.clear();
			}

			return false;
		}
	}
}