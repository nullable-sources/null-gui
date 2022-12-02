#include <null-gui.h>

namespace null::gui {
	void combo(std::string_view name, int* value, const std::vector<std::string>& items) {
		static const std::array<vec2_t, 3> arrow_points{ vec2_t{ 0.f, 0.75f }, vec2_t{ -0.866f, -0.75f }, vec2_t{ 0.866f, -0.75f } };
		static const float arrow_size{ vec2_t{ arrow_points[0] - arrow_points[2] }.length() };

		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		vec2_t min_size{ text_size + style::combo::box_size + vec2_t{ arrow_size * style::combo::arrow_size, style::text_spacing} };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y }) };
		rect_t box_rect{ widget_rect.min + vec2_t{ 0.f, text_size.y + style::text_spacing }, widget_rect.max };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		std::string popup_window{ std::string{ "##" }.append(name).append(" combo popup") };

		bool hovered{ }, pressed{ };
		impl::behaviors::combo(popup_window, box_rect, hovered, pressed);

		c_window::current->draw_list->draw_text(text, widget_rect.min, style::text);
		c_window::current->draw_list->draw_rect_filled(box_rect, hovered || pressed ? pressed ? style::button::box_active : style::button::box_hovered : style::button::box, style::combo::rounding);

		vec2_t arrow_pos{ rect_t{ box_rect }.from_max(box_rect.size().y).center() };
		c_window::current->draw_list->draw_convex_poly_filled(arrow_points | std::views::transform([=](const vec2_t& point) { return point * style::combo::arrow_size + arrow_pos; }) | std::ranges::to<std::vector>(), style::main_color);

		c_window::current->draw_list->push_clip_rect(box_rect.min, box_rect.max - vec2_t{ box_rect.size().y, 0.f }, true); {
			c_window::current->draw_list->draw_text(items[std::clamp(*value, 0, (int)items.size() - 1)], vec2_t{ box_rect.min.x + style::text_spacing, box_rect.center().y }, style::text, render::e_text_flags::aligin_center_y);
		} c_window::current->draw_list->pop_clip_rect();

		if(c_window::find(popup_window)) {
			impl::push_var(&style::window::padding, vec2_t{ 0.f }); {
				impl::push_var(&style::window::item_spacing, 0.f); {
					if(begin_window(popup_window, vec2_t{ box_rect.min.x, box_rect.max.y + style::combo::popup_padding }, vec2_t{ box_rect.size().x, 0.f })) {
						for(int i : std::views::iota(0, (int)items.size())) {
							if(i == style::combo::autosize_items) {
								c_window::current->arg_size.y = c_window::current->size.y = c_window::current->max_size.y + style::window::padding.y - style::window::item_spacing;
								c_window::current->flags &= ~e_window_flags::auto_size;
							}

							if(selectable(items[i], *value == i)) *value = i;
						}
						end_window();
					}
				} impl::pop_var();
			} impl::pop_var();
		}
	}

	namespace impl::behaviors {
		bool combo(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed) {
			if(c_window::current->can_use_widget(widget_name, widget_rect)) {
				hovered_widget = widget_name;

				if(input::keys[input::e_key_id::mouse_left].is_up()) hovered = true;
				if(input::keys[input::e_key_id::mouse_left].is_pressed()) active_widget = widget_name;
			}

			if(active_widget == widget_name) {
				pressed = true;
				if(input::keys[input::e_key_id::mouse_left].is_released() && widget_rect.contains(input::mouse.pos) && c_window::current->can_open_tooltip()) {
					c_window::create({ widget_name, 0.f, 0.f, e_window_flags{ -e_window_flags::popup | -e_window_flags::set_pos | -e_window_flags::set_size | -e_window_flags::auto_size | -e_window_flags::no_title_line } });
					active_widget.clear();
					return true;
				}
			}

			return false;
		}
	}
}