#include <null-gui.h>

namespace null::gui {
	void c_button::setup_self() {
		size = render::c_font::get_current_font()->calc_text_size(name) + vec2_t{ 10, 5 };
		size.x = std::max(size.x, node.parent->working_region.size().x);
	}

	void c_button::draw() {
		color_t button_color{ style.default_color };
		if(state & e_widget_state::active) button_color = style.active_color;
		else if(state & e_widget_state::hovered) button_color = style.hovered_color;

		gui_layer.draw_rect_filled(pos, pos + size, button_color);
		gui_layer.draw_text(name, rect_t{ pos, pos + size }.origin(rect_t::center), style.text_color, render::e_text_flags::aligin_center);

		i_widget::draw();
	}
}