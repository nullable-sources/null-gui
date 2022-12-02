#include <null-gui.h>

namespace null::gui {
	bool selectable(std::string_view name, bool selected) {
		std::string_view text{ impl::format_widget_name(name) };
		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		vec2_t min_size{ style::selectable::active_offset + style::selectable::offset + text_size.x, text_size.y };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return false;

		bool hovered{ }, pressed{ };
		bool active{ impl::behaviors::button(name, widget_rect, hovered, pressed) };

		c_window::current->draw_list->draw_text(text, vec2_t{ widget_rect.min.x + (selected ? style::selectable::active_offset : style::selectable::offset), widget_rect.center().y }, selected ? style::main_color : hovered || pressed ? style::text_hovered : style::text, render::e_text_flags::aligin_center_y);

		return active;
	}
}