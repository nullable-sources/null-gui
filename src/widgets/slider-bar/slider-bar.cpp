#include <null-gui.h>

namespace null::gui {
	bool c_slider_bar::can_hovered() const {
		if(!i_widget::can_hovered()) return false;
		return working_region.contains(input::mouse.pos - pos);
	}

	void c_slider_bar::setup_self() {
		vec2_t name_size{ render::c_font::get_current_font()->calc_text_size(name) };
		size = vec2_t{ std::max(name_size.x, node.parent->working_region.size().x), name_size.y + style.bar_height + style.bar_offset };

		working_region.min = vec2_t{ 0.f, name_size.y + style.bar_offset };
		working_region.max = working_region.min + vec2_t{ size.x, style.bar_height };

		pixels_per_value = size.x / (max_value - min_value);
		i_widget::setup_self();
	}

	void c_slider_bar::draw() {
		color_t field_color{ style.default_color };
		if(state & e_widget_state::active) field_color = style.active_color;
		else if(state & e_widget_state::hovered) field_color = style.hovered_color;

		gui_layer.draw_text(std::vformat(format, std::make_format_args(*value)), pos + vec2_t{ size.x, 0.f }, style.value_color, render::e_text_flags::aligin_right);

		gui_layer.draw_text(name, pos, { });
		gui_layer.draw_rect_filled(working_region + pos, field_color);

		gui_layer.push_clip_rect(working_region + pos, true);
		gui_layer.draw_rect_filled(working_region.min + pos, pos + vec2_t{ working_region.min.x + (pixels_per_value * (*value)), working_region.max.y }, style.bar_color);
		gui_layer.pop_clip_rect();

		i_widget::draw();
	}

	void c_slider_bar::on_mouse_move() {
		if(state & e_widget_state::active) {
			*value = std::clamp((input::mouse.pos.x - pos.x) / pixels_per_value, min_value, max_value);
		}
		i_widget::on_mouse_move();
	}
}