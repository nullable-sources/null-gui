#include <null-gui.h>

namespace null::gui {
	void c_group::setup_self() {
		if(flags & e_layout_flags::auto_size_x) size.x = node.parent->working_region.size().x;
		i_layout::setup_self();
	}

	void c_group::draw() {
		gui_layer.push_clip_rect(pos, pos + size, true);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background

		gui_layer.push_clip_rect(get_real_bounds(), true);
		i_widget::draw();
		gui_layer.pop_clip_rect();

		gui_layer.pop_clip_rect();
	}
}