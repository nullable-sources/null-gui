#include <null-gui.h>

namespace null::gui {
	void i_selectable::setup() {
		size = vec2_t{ node.parent->working_region.size().x, render::c_font::get_current_font()->calc_text_size(name).y + style.height_padding };
		i_widget::setup();
	}

	void i_selectable::draw() {
		color_t selectable_color{ style.default_color };
		if(state & e_widget_state::active) selectable_color = style.active_color;
		else if(state & e_widget_state::hovered) selectable_color = style.hovered_color;
		else if(selected()) selectable_color = style.selected_color;

		gui_layer.draw_rect_filled(pos, pos + size, selectable_color);
		gui_layer.draw_text(name, pos + vec2_t{ selected() ? style.selected_text_offset : style.text_offset, size.y / 2.f }, style.text_color, render::e_text_flags::aligin_center_y);

		i_widget::draw();
	}
}