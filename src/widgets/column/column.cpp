#include <null-gui.h>

float widget_pos_before{ };
float column_step{ };
float current_offset{ };

namespace null::gui {
	void begin_columns(int count) {
		c_window* window{ c_window::current };
		float offset{ window->autopositioning.current_pos.x - window->pos.x - window->get_padding().x };
		if(window->in_column && window->column_offset > 0.f) offset = (window->column_offset - window->get_padding().x) - offset;
		impl::push_var(&window->size.x, (window->size.x - offset) / count);
		impl::push_var(&window->autopositioning.current_pos.x, window->autopositioning.current_pos.x);
		impl::push_var(&widget_pos_before, window->autopositioning.current_pos.y);
		impl::push_var(&column_step, window->size.x + offset);
		impl::push_var(&current_offset, column_step + (window->in_column && window->column_offset > 0.f ? window->column_offset - window->get_padding().x : 0.f));
		impl::push_var(&window->in_column, true);
		impl::push_var(&window->column_offset, window->column_offset);
	}

	void next_column() {
		c_window::current->column_offset = current_offset + c_window::current->get_padding().x;
		c_window::current->autopositioning.current_pos = vec2_t{ c_window::current->pos.x + c_window::current->column_offset, widget_pos_before };

		current_offset += column_step;
	}

	void end_columns() {
		impl::pop_var(); //window->column_offset
		impl::pop_var(); //window->in_column
		impl::pop_var(); //current_offset
		impl::pop_var(); //column_step
		impl::pop_var(); //widget_pos_before
		impl::pop_var(); //window->autopositioning.current_pos.x
		impl::pop_var(); //window->size.x
	}
}