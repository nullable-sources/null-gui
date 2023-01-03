#include <null-gui.h>

namespace null::gui {
	void c_title_bar::setup_self() {
		size = vec2_t{ node.parent->working_region.size().x, render::c_font::get_current_font()->calc_text_size(name).y + style.height };
	}

	void c_title_bar::draw() {
		gui_layer.draw_rect_filled(pos, pos + size, style.color);
		gui_layer.draw_text(name, rect_t{ pos, pos + vec2_t{ style.text_offset, size.y } }.origin(rect_t::center), { }, render::e_text_flags::aligin_center_y);

		i_widget::draw();
	}
}