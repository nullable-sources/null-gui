#include <null-gui.h>

namespace null::gui {
	void c_window::focus() {
		if(auto finded = std::ranges::find_if(window_stack, [=](const std::shared_ptr<c_window>& winodw) { return winodw.get() == this; });
			finded != window_stack.end())
			std::rotate(finded, std::next(finded), window_stack.end());
	}

	void c_window::setup_bounds() {
		bounds = { pos + vec2_t{ 0.f, style.titlebar_height }, pos + size };
		i_container::setup_bounds();
	}

	void c_window::append_auto_size() {
		i_container::append_auto_size();
		if(flags & e_container_flags::auto_size_x) size.x += style.padding.x;
		if(flags & e_container_flags::auto_size_y) size.y += style.padding.y;
	}

	void c_window::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		auto_positioning.next_position.y += style.widget_padding;
	}

	void c_window::setup() {
		working_region = { vec2_t{ 0.f, style.titlebar_height } + style.padding, size - style.padding };
		i_container::setup();
	}

	void c_window::draw() {
		gui_layer.push_clip_rect(pos, pos + size);
		
		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background
		gui_layer.draw_rect_filled(pos, pos + vec2_t{ size.x, style.titlebar_height }, style.titlebar_color); //@note: draw titlebar
		gui_layer.draw_text(name, rect_t{ pos, pos + vec2_t{ style.titlebar_text_offset, style.titlebar_height } }.origin(rect_t::center), { }, render::e_text_flags::aligin_center_y); //@note: draw titlebar text

		gui_layer.push_clip_rect(bounds, true);
		i_container::draw();
		gui_layer.pop_clip_rect();
		
		gui_layer.pop_clip_rect();
	}

	void c_window::on_child_focused(i_widget* child) {
		i_container::on_child_focused(child);

		if(!is_topmost()) focus();
	}

	void c_window::on_focused() {
		i_container::on_focused();

		if(!is_topmost()) focus();
	}

	void c_window::on_mouse_move() {
		i_container::on_mouse_move();

		if(state & e_widget_state::active) {
			pos = input::mouse.pos - clicked_offset;
		}
	}

	void c_window::on_mouse_key_down(const input::e_key_id& key) {
		i_container::on_mouse_key_down(key);

		clicked_offset = input::mouse.pos - pos;
	}
}