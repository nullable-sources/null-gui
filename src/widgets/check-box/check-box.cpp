#include <null-gui.h>

namespace null::gui {
	bool c_check_box::can_hovered() {
		if(!i_widget::can_hovered()) return false;
		return box_region.contains(input::mouse.pos);
	}

	void c_check_box::setup() {
		size = render::c_font::get_current_font()->calc_text_size(name) + vec2_t{ 10, 5 };
		size.x = std::max(size.x, node.parent->working_region.size().x);

		working_region = rect_t{ vec2_t{ 0.f }, vec2_t{ size.y } };
		box_region = working_region + pos;
		i_widget::setup();
	}

	void c_check_box::draw() {
		color_t box_color{ style.default_color };
		if(state & e_widget_state::active) box_color = style.active_color;
		else if(state & e_widget_state::hovered) box_color = style.hovered_color;

		gui_layer.draw_rect_filled(box_region, box_color);
		if(*value) gui_layer.draw_rect_filled(box_region + rect_t{ style.check_mark_offset, -style.check_mark_offset }, style.check_mark_color);
		gui_layer.draw_text(name, box_region.center() + vec2_t{ box_region.size().x / 2.f + style.text_offset, 0.f }, style.text_color, render::e_text_flags::aligin_center_y);

		i_widget::draw();
	}

	void c_check_box::on_mouse_key_up() {
		*value = !*value;
		i_widget::on_mouse_key_up();
	}
}