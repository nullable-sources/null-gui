#include <null-gui.h>

void calc_scroll(rect_t& bar_rect, rect_t& draw_rect, float& bar_size, float& pos) {
	null::gui::c_window* wnd = null::gui::c_window::current;
	bar_rect = rect_t{ wnd->pos + vec2_t(wnd->size.x - null::gui::style::scrollbar::thickness - null::gui::style::scrollbar::padding.x, wnd->get_title_size() + null::gui::style::scrollbar::padding.y), wnd->pos + wnd->size - null::gui::style::scrollbar::padding };
	bar_size = std::max(bar_rect.size().y * std::clamp(wnd->size.y / wnd->max_size.y, 0.f, 1.f), 2.f);
	draw_rect = bar_rect + rect_t{ vec2_t(0.f, bar_size / 2.f), vec2_t(0.f, bar_size / -2.f) };
	pos = draw_rect.size().y * (std::abs(wnd->scrollbar_data.offset) / wnd->scrollbar_data.max_scroll);
}

namespace null::gui {
	void begin_scroll() {
		if((c_window::current->flags & e_window_flags::auto_size && c_window::current->arg_size.y < 1.f) || !c_window::current->scrollbar_data.max_scroll) return;

		rect_t bar_rect{ }, draw_rect{ };
		float bar_size{ }, pos{ };
		calc_scroll(bar_rect, draw_rect, bar_size, pos);

		bool hovered{ }, pressed{ };
		impl::behaviors::scroll(bar_rect, hovered, pressed);

		if(!pressed) {
			c_window::current->scrollbar_data.pressed_mouse_offset = 0.f;

			if(draw_rect.contains(input::mouse.pos)) {
				if(rect_t(draw_rect.min + vec2_t(0.f, pos - bar_size / 2), vec2_t(draw_rect.max.x, draw_rect.min.y) + vec2_t(0.f, pos + bar_size / 2)).contains(input::mouse.pos)) {
					c_window::current->scrollbar_data.pressed_mouse_offset = pos - (input::mouse.pos.y - draw_rect.min.y);
				}
			} else if(rect_t(vec2_t(vec2_t(draw_rect.min.x, draw_rect.min.y + pos - bar_size / 2)), vec2_t(draw_rect.max.x, draw_rect.min.y + pos + bar_size / 2)).contains(input::mouse.pos)) {
				c_window::current->scrollbar_data.pressed_mouse_offset = draw_rect.min.y + pos - input::mouse.pos.y;
			} else if(bar_rect.contains(input::mouse.pos)) {
				c_window::current->scrollbar_data.pressed_mouse_offset = (input::mouse.pos.y < draw_rect.min.y ? draw_rect.min.y : draw_rect.max.y) - input::mouse.pos.y;
			}
		} else c_window::current->scrollbar_data.target = -((input::mouse.pos.y - draw_rect.min.y + c_window::current->scrollbar_data.pressed_mouse_offset) / draw_rect.size().y * c_window::current->scrollbar_data.max_scroll);
	}

	void end_scroll() {
		if(c_window::current->max_size.y > c_window::current->size.y && (!(c_window::current->flags & e_window_flags::auto_size) || c_window::current->arg_size.y > 1.f)) {
			c_window::current->scrollbar_data.offset = std::clamp(roundf(math::lerp(c_window::current->scrollbar_data.offset, c_window::current->scrollbar_data.target, 0.2f)), -c_window::current->scrollbar_data.max_scroll, 0.f);

			c_window::current->scrollbar_data.max_scroll = ((c_window::current->max_size.y + style::window::padding.y - style::window::item_spacing) - c_window::current->size.y);
			c_window::current->scrollbar_data.target = std::clamp(c_window::current->scrollbar_data.target, -c_window::current->scrollbar_data.max_scroll, 0.f);
			c_window::current->scrollbar_data.offset = std::clamp(c_window::current->scrollbar_data.offset, -c_window::current->scrollbar_data.max_scroll, 0.f);
		} else {
			c_window::current->scrollbar_data.max_scroll = 0.f;
		}

		if((!(c_window::current->flags & e_window_flags::auto_size) || c_window::current->arg_size.y > 1.f) && c_window::current->scrollbar_data.max_scroll != 0) {
			rect_t bar_rect{ }, draw_rect{ };
			float bar_size{ }, pos{ };
			calc_scroll(bar_rect, draw_rect, bar_size, pos);

			if(style::scrollbar::show_background) c_window::current->draw_list->draw_rect_filled(bar_rect, style::button::box, style::scrollbar::rounding);
			c_window::current->draw_list->draw_rect_filled(vec2_t(draw_rect.min.x, draw_rect.min.y + pos - bar_size / 2), vec2_t(draw_rect.max.x, draw_rect.min.y + pos + bar_size / 2), style::main_color, style::scrollbar::rounding);
		}
	}

	namespace impl::behaviors {
		void scroll(const rect_t& widget_size, bool& hovered, bool& pressed) {
			std::string name{ std::format("scrollbar##{}", c_window::current->name.c_str()) };

			if(c_window::current->can_use_widget(name, widget_size)) {
				hovered_widget = name;

				if(input::keys[input::e_key_id::mouse_left].is_up()) hovered = true;
				if(input::keys[input::e_key_id::mouse_left].is_pressed()) active_widget = name;

				if(active_widget == name)
					pressed = true;
			}

			if(input::keys[input::e_key_id::mouse_left].is_up() && active_widget == name) {
				active_widget.clear();
			}
		}
	}
}