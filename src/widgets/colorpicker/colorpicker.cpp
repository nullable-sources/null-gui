#include <null-gui.h>

std::map<std::string, hsv_color_t> created_colorpickers;

namespace null::gui {
	void colorpicker_sv_box(std::string_view name, hsv_color_t& color) {
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::colorpicker::sv_box_size }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		if(impl::behaviors::colorpicker_slider(name, widget_rect)) {
			vec2_t sv{ (input::mouse.pos - widget_rect.min) / style::colorpicker::sv_box_size };
			color.s() = std::clamp(sv.x, 0.f, 1.f);
			color.v() = std::clamp(1.f - sv.y, 0.f, 1.f);
		}

		color_t<int> hsv{ hsv_color_t{ color.h() } };
		c_window::current->draw_list->draw_rect_filled_multicolor(widget_rect, { color_t<int>::palette_t::white, hsv, color_t<int>{ 255, 0 }, color_t<int>{ hsv, 0 } }, style::colorpicker::rounding);
		c_window::current->draw_list->draw_rect_filled_multicolor(widget_rect, { 0, 0, color_t<int>::palette_t::black, color_t<int>::palette_t::black }, style::colorpicker::rounding);

		vec2_t dot_pos{ math::round(widget_rect.min + vec2_t{ color.s(), 1.f - color.v() } * style::colorpicker::sv_box_size) };
		c_window::current->draw_list->draw_circle_filled(dot_pos, { 45, 200 }, 5.f);
		c_window::current->draw_list->draw_circle_filled(dot_pos, { color_t<float>{ color }, 255 }, 3.f);

	}

	void colorpicker_h_slider(std::string_view name, hsv_color_t& color) {
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::colorpicker::sliders_thickness, style::colorpicker::sv_box_size }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		if(impl::behaviors::colorpicker_slider(name, widget_rect)) {
			color.h() = std::clamp((input::mouse.pos.y - widget_rect.min.y) / style::colorpicker::sv_box_size, 0.f, 1.f) * 360.f;
		}

		vec2_t segment_size{ style::colorpicker::sliders_thickness, style::colorpicker::sv_box_size / 6.f };
		for(int segment : std::views::iota(0, 6)) {
			color_t<int> hsv{ hsv_color_t{ segment * 60.f } }, next_hsv{ hsv_color_t{ (segment + 1) * 60.f } };
			rect_t segment_rect{ rect_t{ widget_rect.min + vec2_t{ 0.f, segment_size.y * segment } }.from_min(segment_size) };
			c_window::current->draw_list->draw_rect_filled_multicolor(segment == 0 || segment == 5 ? math::round(segment_rect) : segment_rect, { hsv, hsv, next_hsv, next_hsv }, style::colorpicker::rounding, segment == 0 ? render::e_corner_flags::top : segment == 5 ? render::e_corner_flags::bot : render::e_corner_flags{ });
		}

		float indicator_pos{ std::round(widget_rect.min.y + (color.h() / 360.f) * widget_rect.size().y) };
		c_window::current->draw_list->draw_rect_filled(vec2_t{ widget_rect.min.x, indicator_pos } - vec2_t{ 2, 1 }, vec2_t{ widget_rect.max.x, indicator_pos } + vec2_t{ 2, 1 }, { });
	}

	void colorpicker_alpha_slider(std::string_view name, hsv_color_t& color) {
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ c_window::current->size.x - (style::window::padding.x * 2), style::colorpicker::sliders_thickness }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		if(impl::behaviors::colorpicker_slider(name, widget_rect))
			color.a() = std::clamp((input::mouse.pos.x - widget_rect.min.x) / widget_rect.size().x, 0.f, 1.f);

		float indicator_pos{ std::round(widget_rect.min.x + color.a() * widget_rect.size().x) };
		color_t<int> rgba{ color_t<float>{ color }, 255 };
		c_window::current->draw_list->draw_rect_filled_multicolor(widget_rect, { color_t<int>::palette_t::white, rgba, color_t<int>::palette_t::white, rgba }, style::colorpicker::rounding);
		c_window::current->draw_list->draw_rect_filled(vec2_t{ indicator_pos, widget_rect.min.y } - vec2_t{ 1.f, 2.f }, vec2_t{ indicator_pos, widget_rect.max.y } + vec2_t{ 1.f, 2.f }, { }, style::colorpicker::rounding);

	}

	void colorpicker(std::string_view name, color_t<int>* clr, bool alpha_bar) {
		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		vec2_t min_size{ text_size.x + style::text_spacing + style::checkbox::box_size * 2, std::max(style::checkbox::box_size, text_size.y) };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y}) };
		rect_t button_rect{ rect_t{ widget_rect }.from_max(vec2_t{ 2.f, 1.f } * widget_rect.size().y) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return;

		hsv_color_t& saved_color{ created_colorpickers[std::string{ name }] };
		std::string picker_name{ std::string{ "##" }.append(name).append(" colorpicker window") }, copy_name{ std::string{ "##" }.append(name).append(" colorpicker tooltip") };
		if(impl::behaviors::colorpicker(name, picker_name, copy_name, button_rect)) {
			saved_color = hsv_color_t{ *clr };
		}

		tooltip(std::format("r: {}; g: {}; b: {}; a: {}", clr->r(), clr->g(), clr->b(), clr->a()));

		c_window::current->draw_list->draw_rect_filled(button_rect, color_t<int>{ *clr, 255 }, style::colorpicker::rounding);
		c_window::current->draw_list->draw_text(text, vec2_t{ widget_rect.min.x, widget_rect.center().y }, style::text, render::e_text_flags::aligin_center_y);

		if(c_window::find(picker_name)) {
			impl::push_var(&style::window::padding, vec2_t{ 5.f }); {
				if(begin_window(picker_name, vec2_t{ widget_rect.max.x, widget_rect.min.y }, 0.f)) {
					colorpicker_sv_box(std::string{ "##" }.append(name).append(" colorpicker sv rect"), saved_color);
					impl::push_var(&style::window::item_spacing, 0.f); {
						same_line();
					} impl::pop_var();
					colorpicker_h_slider(std::string{ "##" }.append(name).append(" colorpicker h slider"), saved_color);
					if(alpha_bar) colorpicker_alpha_slider(std::string{ "##" }.append(name).append(" colorpicker alpha slider"), saved_color);

					if(saved_color != hsv_color_t{ *clr }) *clr = color_t<float>{ saved_color };

					end_window();
				}
			} impl::pop_var();
		}

		if(c_window::find(copy_name)) {
			impl::push_var(&style::window::padding, vec2_t{ 5.f }); {
				impl::push_var(&style::items_size_full_window, true); {
					if(begin_window(copy_name, vec2_t{ widget_rect.max.x, widget_rect.min.y }, 0.f)) {
						static color_t<int> copied_color{ };
						if(button("copy")) {
							copied_color = *clr;
							c_window::current->close_call = true;
						}
						if(button("paste")) {
							*clr = copied_color;
							c_window::current->close_call = true;
						}
						end_window();
					}
				} impl::pop_var();
			} impl::pop_var();
		}
	}

	namespace impl::behaviors {
		bool colorpicker(std::string_view widget_name, std::string_view picker_name, std::string_view tooltip_name, const rect_t& widget_rect) {
			static const e_window_flags flags{ -e_window_flags::no_move | -e_window_flags::no_title_bar | -e_window_flags::popup | -e_window_flags::set_pos | -e_window_flags::auto_size };
			if(c_window::current->can_use_widget(widget_name, widget_rect)) {
				hovered_widget = widget_name;

				if(input::keys[input::e_key_id::mouse_left].is_pressed() || input::keys[input::e_key_id::mouse_right].is_pressed())
					active_widget = widget_name;
			}

			if(active_widget == widget_name && c_window::current->can_open_tooltip()) {
				if(input::keys[input::e_key_id::mouse_left].is_released()) {
					c_window::create({ picker_name, 0.f, 0.f, e_window_flags{ flags | e_window_flags::no_title_line } });
					active_widget.clear();
					return true;
				} else if(input::keys[input::e_key_id::mouse_right].is_released()) {
					c_window::create({ tooltip_name, 0.f, 0.f, flags });
					active_widget.clear();
				}
			}

			return false;
		}

		bool colorpicker_slider(std::string_view widget_name, const rect_t& widget_rect) {
			if(c_window::current->can_use_widget(widget_name, widget_rect)) {
				hovered_widget = widget_name;

				if(input::keys[input::e_key_id::mouse_left].is_pressed() && active_widget.empty()) active_widget = widget_name;
			}


			if(input::keys[input::e_key_id::mouse_left].is_up() && active_widget == widget_name)
				active_widget.clear();

			return active_widget == widget_name;
		}
	}
}