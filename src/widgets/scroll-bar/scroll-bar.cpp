#include <null-gui.h>

namespace null::gui {
	void c_scroll_bar::setup_self() {
		size = vec2_t{ style.bar_size, node.parent->working_region.size().y - style.container_padding.y * 2 };

		pixel_per_scroll = size.y / (size.y + max_scroll_value);
		slider_data.pos = vec2_t{ 0.f, pixel_per_scroll * get_scroll_value() };
		slider_data.size = { size.x, std::max(size.y * pixel_per_scroll, 4.f) };
	}

	void c_scroll_bar::draw() {
		if(max_scroll_value <= 0.f || pixel_per_scroll < 0) return;

		color_t background_color{ style.background_default_color };
		if(state & e_widget_state::active) background_color = style.background_active_color;
		else if(state & e_widget_state::hovered) background_color = style.background_hovered_color;

		gui_layer.draw_rect_filled(pos, pos + size, background_color);
		gui_layer.draw_rect_filled(pos + slider_data.pos, pos +slider_data.pos + slider_data.size, { style.slider_default_color });
		i_widget::draw();
	}

	void c_scroll_bar::on_mouse_move() {
		i_widget::on_mouse_move();

		if(state & e_widget_state::active) 
			scroll_value = (int)std::clamp((input::mouse.pos.y - pos.y - slider_data.moving_offset) / pixel_per_scroll, 0.f, max_scroll_value);
	}

	void c_scroll_bar::on_mouse_key_down(const input::e_key_id& key) {
		i_widget::on_mouse_key_down(key);
		slider_data.moving_offset = input::mouse.pos.y - slider_data.pos.y;

		if(slider_data.moving_offset > slider_data.size.y || slider_data.moving_offset < 0.f)
			slider_data.moving_offset = slider_data.size.y / 2.f;
	}

	void c_scroll_bar::on_mouse_key_up(const input::e_key_id& key) {
		i_widget::on_mouse_key_up(key);
		slider_data.moving_offset = 0;
	}

	void c_scroll_bar::on_mouse_wheel() {
		scroll_value = (int)std::clamp(scroll_value - input::mouse.wheel.y * style.wheel_modifyer, 0.f, max_scroll_value);
		i_widget::on_mouse_wheel();
	}

	bool c_scroll_bar::handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		if(max_scroll_value <= 0.f || pixel_per_scroll < 0) return false;
		return i_widget::handling_self_events(event, w_param, l_param);
	}
}