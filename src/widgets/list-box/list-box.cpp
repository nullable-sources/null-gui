#include <null-gui.h>

namespace null::gui {
	void c_list_box::setup_self() {
		if(flags & e_layout_flags::auto_size_x && node.parent)
			size.x = node.parent->working_region.size().x;

		working_region = { vec2_t{ 0.f }, size };
		i_container::setup_self();
	}

	void c_list_box::draw() {
		gui_layer.push_clip_rect(pos, pos + size, true);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background

		gui_layer.push_clip_rect(get_real_bounds(), true);
		i_container::draw();
		gui_layer.pop_clip_rect();

		gui_layer.pop_clip_rect();
	}
}