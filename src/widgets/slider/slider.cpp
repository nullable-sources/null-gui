#include <null-gui.h>

namespace null::gui {
	void slider_int(std::string_view name, int* value, int min, int max, std::string_view format) {
		impl::slider(name, value, min, max, format, impl::e_data_type::integer);
	}

	void slider_float(std::string_view name, float* value, float min, float max, std::string_view format) {
		impl::slider(name, value, min, max, format, impl::e_data_type::floating_point);
	}

	namespace impl {
		template <typename value_t>
			requires std::is_arithmetic_v<value_t>
		void slider(std::string_view name, value_t* value, const value_t& min, const value_t& max, std::string_view format, e_data_type data_type) {
			std::string_view text{ impl::format_widget_name(name) };
			std::string formated_value{ std::vformat(format, std::make_format_args(*value)) };

			vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) }, value_size{ render::c_font::get_current_font()->calc_text_size(formated_value) };
			vec2_t min_size{ text_size + style::text_spacing + vec2_t{ render::c_font::get_current_font()->calc_text_size(std::vformat(format, std::make_format_args(max))).x, style::slider::thickness } };
			rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y }) };
			rect_t body_rect{ rect_t{ widget_rect.max }.from_max(vec2_t{ widget_rect.size().x, style::slider::thickness }) };
			rect_t value_rect{ rect_t{ vec2_t{ -value_size.x, 0.f }, vec2_t{ 0.f, value_size.y } } + vec2_t{ widget_rect.max.x, widget_rect.min.y } };

			c_window::current->add_widget(name, widget_rect.size());
			if(!c_window::current->can_draw_widget(widget_rect))
				return;

			if(std::string text_input_name{ std::string{ name }.append("## text input") }; behaviors::slider_text_input(name, text_input_name, value_rect)) {
				c_window::current->autopositioning.back_to_last_pos();
				text_input(text_input_name, value, false, data_type);
				return;
			}

			bool hovered{ }, pressed{ };
			impl::behaviors::slider(name, body_rect, hovered, pressed);

			value_t new_value{ std::clamp(min + (max - min) * (value_t)((input::mouse.pos.x - widget_rect.min.x) / widget_rect.size().x), min, max) };
			if(hovered) formated_value = std::vformat(format, std::make_format_args(new_value));
			if(pressed) *value = new_value;

			c_window::current->draw_list->draw_rect_filled(body_rect, hovered || pressed ? style::button::box_hovered : style::button::box, style::slider::rounding);
			c_window::current->draw_list->draw_text(text, widget_rect.min, style::text);
			c_window::current->draw_list->draw_text(formated_value, vec2_t{ widget_rect.max.x, widget_rect.min.y }, hovered ? color_t<int>{ 100, 255 } : style::text, render::e_text_flags::aligin_right);

			if(hovered) {
				vec2_t slider_size{ (new_value - min) / (max - min) * widget_rect.size().x, style::slider::thickness };
				c_window::current->draw_list->push_clip_rect(rect_t{ body_rect }.from_min(vec2_t{ slider_size }), true); {
					c_window::current->draw_list->draw_rect_filled(body_rect, color_t<int>{ style::main_color, 100 }, style::slider::rounding);
				} c_window::current->draw_list->pop_clip_rect();
			}

			vec2_t slider_size{ (std::clamp(*value, min, max) - min) / (max - min) * widget_rect.size().x, style::slider::thickness };
			c_window::current->draw_list->push_clip_rect(rect_t{ body_rect }.from_min(vec2_t{ slider_size }), true); {
				c_window::current->draw_list->draw_rect_filled(body_rect.min, body_rect.max, style::main_color, style::slider::rounding);
			} c_window::current->draw_list->pop_clip_rect();
		}

		namespace behaviors {
			void slider(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed) {
				if(c_window::current->can_use_widget(widget_name, widget_rect)) {
					hovered_widget = widget_name;

					if(input::keys[input::e_key_id::mouse_left].is_up()) hovered = true;
					if(input::keys[input::e_key_id::mouse_left].is_pressed()) active_widget = widget_name;
				}

				if(active_widget == widget_name) {
					pressed = true;
					if(input::keys[input::e_key_id::mouse_left].is_up()) active_widget.clear();
				}
			}

			bool slider_text_input(std::string_view widget_name, std::string_view input_name, const rect_t& widget_rect) {
				std::string open_widget_name{ std::string{ "##" }.append(widget_name).append(" text input open button") };
				if(c_window::current->can_use_widget(open_widget_name, widget_rect)) {
					hovered_widget = open_widget_name;
					if(input::keys[input::e_key_id::mouse_left].is_pressed() && input::keys[input::e_key_id::ctrl].is_down()) active_widget = open_widget_name;
				}

				if(active_widget == open_widget_name && input::keys[input::e_key_id::mouse_left].is_released()) {
					active_widget = input_name;
				}

				return active_widget == input_name;
			}
		}
	}
}