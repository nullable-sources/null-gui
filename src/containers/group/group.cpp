#include <null-gui.h>

namespace null::gui {
	void c_group::setup_bounds() {
		bounds = { pos + vec2_t{ 0.f, style.titlebar_height }, pos + size };
		i_container::setup_bounds();
	}

	void c_group::append_auto_size() {
		if(container_flags & e_container_flags::auto_size_y && auto_positioning.max_position.y)
			size.y = auto_positioning.max_position.y + style.padding.y;
	}

	void c_group::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		auto_positioning.next_position.y += style.widget_padding;
	}

	void c_group::setup() {
		if(container_flags & e_container_flags::auto_size_x)
			size.x = node.parent->working_region.size().x;

		working_region = { vec2_t{ 0.f, style.titlebar_height } + style.padding, size - style.padding };
		i_container::setup();
	}

	void c_group::draw() {
		gui_layer.push_clip_rect(pos, pos + size, true);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background
		gui_layer.draw_rect_filled(pos, pos + vec2_t{ size.x, style.titlebar_height }, style.titlebar_color); //@note: draw titlebar
		gui_layer.draw_text(name, rect_t{ pos, pos + vec2_t{ style.titlebar_text_offset, style.titlebar_height } }.origin(rect_t::center), { }, render::e_text_flags::aligin_center_y); //@note: draw titlebar text

		gui_layer.push_clip_rect(bounds, true);
		i_container::draw();
		gui_layer.pop_clip_rect();

		gui_layer.pop_clip_rect();
	}
}