#include <null-gui.h>

namespace null::gui {
	bool c_check_box::can_hovered() const {
		if(!i_widget::can_hovered()) return false;
		return working_region.contains(input::mouse.pos - pos);
	}

	void c_check_box::setup_self() {
		size = render::c_font::get_current_font()->calc_text_size(name) + vec2_t{ style.text_offset + style.box_size.x, 0.f };
		size = math::max(size, vec2_t{ node.parent->working_region.size().x, style.box_size.y });

		working_region = rect_t{ vec2_t{ 0.f }, style.box_size };
		i_widget::setup_self();
	}

	void c_check_box::draw() {
		color_t box_color{ style.default_color };
		if(state & e_widget_state::active) box_color = style.active_color;
		else if(state & e_widget_state::hovered) box_color = style.hovered_color;

		gui_layer.draw_rect_filled(working_region + pos, box_color);
		if(*value) gui_layer.draw_rect_filled(working_region + rect_t{ style.check_mark_offset, -style.check_mark_offset } + pos, style.check_mark_color);
		gui_layer.draw_text(name, working_region.origin(rect_t::center) + vec2_t{ working_region.size().x / 2.f + style.text_offset, 0.f } + pos, style.text_color, render::e_text_flags::aligin_center_y);

		i_widget::draw();
	}

	void c_check_box::on_mouse_key_up(const input::e_key_id& key) {
		*value = !*value;
		i_widget::on_mouse_key_up(key);
	}
}