#include <null-gui.h>

namespace null::gui {
	void c_popup::draw() {
		gui_layer.push_clip_rect(pos, pos + size);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color);

		gui_layer.push_clip_rect(get_real_bounds(), true);
		i_widget::draw();
		gui_layer.pop_clip_rect();

		gui_layer.pop_clip_rect();
	}
}