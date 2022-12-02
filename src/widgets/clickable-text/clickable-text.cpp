#include <null-gui.h>

namespace null::gui {
	bool clickable_text(std::string_view name) {
		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(text_size) };

		c_window::current->add_widget(name, text_size);
		if(!c_window::current->can_draw_widget(widget_rect))
			return false;

		bool hovered{ }, pressed{ };
		bool active{ impl::behaviors::button(name, widget_rect, hovered, pressed) };

		c_window::current->draw_list->draw_text(text, widget_rect.min, hovered || pressed ? style::main_color : style::text);

		return active;
	}
}