#include <null-gui.h>

namespace null::gui {
	void multicombo(std::string_view name, std::vector<bool>* values, const std::vector<std::string>& items) {
		static const std::array<vec2_t, 3> arrow_points{ vec2_t{ 0.f, 0.75f }, vec2_t{ -0.866f, -0.75f }, vec2_t{ 0.866f, -0.75f } };
		static const float arrow_size{ vec2_t{ arrow_points[0] - arrow_points[2] }.length() };

		if(values->size() != items.size())
			throw std::runtime_error{ "values and items cannot be of different sizes" };

		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		vec2_t min_size{ text_size + style::combo::box_size + vec2_t(arrow_size, style::text_spacing) };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y }) };
		rect_t box_rect{ widget_rect.min + vec2_t{ 0.f, text_size.y + style::text_spacing }, widget_rect.max };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		std::string popup_window{ std::string{ "##" }.append(name).append(" multicombo popup") };

		bool hovered{ }, pressed{ };
		impl::behaviors::combo(popup_window, box_rect, hovered, pressed);

		float max_size{ widget_rect.size().x - box_rect.size().y };
		float ellipsis_size{ render::c_font::get_current_font()->calc_text_size("...").x };
		std::string enable_items{ };
		for(int i : std::views::iota(0, (int)values->size()) | std::views::filter([&](int i) { return values->at(i); })) {
			if(!enable_items.empty()) enable_items.append(", ");
			enable_items.append(items[i]);
		}

		if(!enable_items.empty()) {
			float max_size{ widget_rect.size().x - box_rect.size().y };
			float ellipsis_size{ render::c_font::get_current_font()->calc_text_size("...").x };
			if(render::c_font::get_current_font()->calc_text_size(enable_items).x + ellipsis_size > max_size) {
				for(std::string_view str : std::views::iota(enable_items.begin(), enable_items.end()) | std::views::reverse | std::views::transform([&](const auto& iterator) { return std::string_view{ enable_items.begin(), iterator }; })) {
					if(render::c_font::get_current_font()->calc_text_size(str).x + ellipsis_size < max_size) {
						enable_items = std::string{ str }.append("...");
						break;
					}
				}
			}
		}

		c_window::current->draw_list->draw_text(text, widget_rect.min, style::text);
		c_window::current->draw_list->draw_rect_filled(box_rect, hovered || pressed ? pressed ? style::button::box_active : style::button::box_hovered : style::button::box, style::combo::rounding);
		c_window::current->draw_list->draw_text(enable_items, vec2_t{ box_rect.min.x + style::text_spacing, box_rect.center().y }, style::text, render::e_text_flags::aligin_center_y);

		vec2_t arrow_pos{ rect_t{ box_rect }.from_max(box_rect.size().y).center() };
		c_window::current->draw_list->draw_convex_poly_filled(arrow_points | std::views::transform([=](const vec2_t& point) { return point * style::combo::arrow_size + arrow_pos; }) | std::ranges::to<std::vector>(), style::main_color);

		if(c_window::find(popup_window)) {
			impl::push_var(&style::window::padding, vec2_t{ 0.f }); {
				impl::push_var(&style::window::item_spacing, 0.f); {
					if(begin_window(popup_window, vec2_t{ box_rect.min.x, box_rect.max.y + style::combo::popup_padding }, vec2_t{ box_rect.size().x, 0.f })) {
						for(int i : std::views::iota(0, (int)items.size())) {
							if(i == style::combo::autosize_items) {
								c_window::current->arg_size.y = c_window::current->size.y = c_window::current->max_size.y + style::window::padding.y - style::window::item_spacing;
								c_window::current->flags &= ~e_window_flags::auto_size;
							}

							if(selectable(items[i], values->at(i)))
								values->at(i) = !values->at(i);
						}
						end_window();
					}
				} impl::pop_var();
			} impl::pop_var();
		}
	}

	void multicombo(std::string_view name, std::vector<bool*>& values, const std::vector<std::string>& items) {
		std::vector<bool> _values;
		for(bool* a : values)
			_values.push_back(*a);

		multicombo(name, &_values, items);

		for(int i = 0; i < values.size(); i++)
			*values.at(i) = _values.at(i);
	}
}