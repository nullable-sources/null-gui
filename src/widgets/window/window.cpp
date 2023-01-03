#include <null-gui.h>

namespace null::gui {
	void c_window::focus() {
		if(auto finded{ std::ranges::find_if(window_stack, [=](const std::shared_ptr<c_window>& winodw) { return winodw.get() == this; }) }; finded != window_stack.end())
			std::rotate(finded, std::next(finded), window_stack.end());
	}

	void c_window::setup_self() {
		working_region = { style.padding, size - style.padding };
		i_layout::setup_self();
	}

	void c_window::draw() {
		gui_layer.push_clip_rect(pos, pos + size);
		
		gui_layer.draw_rect_filled(pos, pos + size, style.background_color); //@note: draw background

		gui_layer.push_clip_rect(get_real_bounds(), true);
		i_widget::draw();
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
}