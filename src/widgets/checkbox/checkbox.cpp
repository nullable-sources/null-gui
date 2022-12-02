#include <null-gui.h>

namespace null::gui {
	void checkbox(std::string_view name, bool* value) {
		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::checkbox::box_size + style::text_spacing + text_size.x, std::max(style::checkbox::box_size, text_size.y) }) };
		rect_t box_rect{ rect_t{ vec2_t{ widget_rect.min.x, widget_rect.center().y - style::checkbox::box_size / 2 } }.from_min(vec2_t{ style::checkbox::box_size }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		bool hovered{ }, pressed{ };
		if(impl::behaviors::button(name, style::checkbox::hovered_with_text ? widget_rect : box_rect, hovered, pressed))
			*value = !*value;

		c_window::current->draw_list->draw_text(text, vec2_t(widget_rect.min.x + style::checkbox::box_size + style::text_spacing, widget_rect.center().y), style::text, render::e_text_flags::aligin_center_y);
		c_window::current->draw_list->draw_rect_filled(box_rect.min, box_rect.max, hovered || pressed ? style::button::box_hovered : style::button::box, style::checkbox::rounding);
		if(*value) {
			c_window::current->draw_list->draw_rect_filled(box_rect.min, box_rect.max, style::main_color, style::checkbox::rounding);
			float pad{ std::max(std::floor(style::checkbox::box_size / style::checkbox::check_mark_size), 1.f) };
			float size{ style::checkbox::box_size - pad * 2.f };
			float thickness{ std::max(size / 5.0f, 0.f) };
			size -= thickness * 0.5f;

			float third = size / 3.0f;
			vec2_t b{ box_rect.min + pad + thickness * 0.25f + vec2_t{ third, size - third * 0.5f } };
			c_window::current->draw_list->draw_poly_line({ b - third, b, b + vec2_t{ 2.f, -2.f } * third }, color_t<int>::palette_t::white, false, thickness);
		}
	}
}