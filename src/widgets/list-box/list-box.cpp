#include <null-gui.h>

namespace null::gui {
	void c_list_box::setup_bounds() {
		bounds = { pos + vec2_t{ 0.f, style.titlebar_height }, pos + size };
		i_container::setup_bounds();
	}

	void c_list_box::setup_scroll_bar() {
		i_container::setup_scroll_bar();
		if(scroll_bar.max_scroll_value > 0) working_region.max.x -= style.scrollbar_padding.x;
	}

	void c_list_box::append_auto_size() {
		if(container_flags & e_container_flags::auto_size_y) size.y = clamped_size ? clamped_size : auto_positioning.max_position.y;
	}

	void c_list_box::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		if(style.max_show_items < node.childs.size() && widget == node.childs[style.max_show_items - 1].get())
			clamped_size = auto_positioning.max_position.y;
	}

	void c_list_box::setup() {
		if(container_flags & e_container_flags::auto_size_x)
			size.x = node.parent->working_region.size().x;

		working_region = { vec2_t{ 0.f, style.titlebar_height }, size };
		i_container::setup();
	}

	void c_list_box::draw() {
		gui_layer.push_clip_rect(pos, pos + size, true);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background
		gui_layer.draw_rect_filled(pos, pos + vec2_t{ size.x, style.titlebar_height }, style.titlebar_color); //@note: draw titlebar
		gui_layer.draw_text(name, rect_t{ pos, pos + vec2_t{ style.titlebar_text_offset, style.titlebar_height } }.center(), { }, render::e_text_flags::aligin_center_y); //@note: draw titlebar text

		gui_layer.push_clip_rect(bounds, true);
		i_container::draw();
		gui_layer.pop_clip_rect();

		gui_layer.pop_clip_rect();
	}
}