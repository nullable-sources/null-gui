#include <null-gui.h>

namespace null::gui {
	bool begin_group(std::string_view name, vec2_t arg_size) {
		vec2_t group_size{ c_window::current->get_window_size_with_padding() };
		return begin_window(name, c_window::current->get_current_widget_pos(), vec2_t{ arg_size.x == 0.f ? group_size.x : arg_size.x, arg_size.y == 0.f ? group_size.y : arg_size.y }, e_window_flags{ -e_window_flags::set_pos | -e_window_flags::set_size | -e_window_flags::no_move | -e_window_flags::group });
	}

	void end_group() {
		c_window* group{ c_window::current };

		end_window();

		c_window::current->add_widget(group->name, group->size);
	}
}