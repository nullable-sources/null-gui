#include <null-gui.h>

namespace null::gui {
	bool text_input(std::string_view name, void* value, bool password, impl::e_data_type data_type) {
		std::string_view text{ impl::format_widget_name(name) };

		vec2_t text_size{ render::c_font::get_current_font()->calc_text_size(text) };
		vec2_t min_size{ text_size + style::text_spacing + style::combo::box_size + vec2_t{ 0.f, style::text_input::line_size } };
		rect_t widget_rect{ rect_t{ c_window::current->get_current_widget_pos() }.from_min(vec2_t{ style::items_size_full_window ? std::max(min_size.x, c_window::current->get_window_size_with_padding().x) : min_size.x, min_size.y }) };
		rect_t body_rect{ widget_rect + rect_t{ vec2_t{ 0.f, text_size.y + style::text_spacing }, vec2_t{ } } };
		rect_t working_rect{ body_rect.min + style::text_input::padding, body_rect.max - vec2_t{ style::text_input::padding.x, style::text_input::padding.y + style::text_input::line_size } };
		
		impl::c_text_input* input{ impl::c_text_input::create(impl::c_text_input{ std::string{ name }, value, working_rect, data_type }) };

		c_window::current->add_widget(name, widget_rect.size());
		if(!c_window::current->can_draw_widget(widget_rect))
			return false;

		bool hovered{ }, pressed{ };
		bool active{ impl::behaviors::text_input(name, body_rect, hovered, pressed) };

		std::wstring buffer{ password ? std::wstring(input->utf8_buffer.size(), '*') : input->utf8_buffer };
		if(active) {
			if(input::keys[input::e_key_id::mouse_left].is_pressed()) {
				input->write_data.offset = input->get_hovered_char(buffer);
				input->write_data.update(style::text_input::write_offset_show);
				input->selection_data.set(input->write_data.offset);
			}

			if(input::keys[input::e_key_id::mouse_left].is_down())
				input->selection_data.end = input->get_hovered_char(buffer);

			if(input->write_data.next_change_time <= std::chrono::steady_clock::now())
				input->write_data.update(input->write_data.show ? style::text_input::write_offset_hide : style::text_input::write_offset_show, !input->write_data.show);
		} else {
			input->update_buffers();
		}

		c_window::current->draw_list->draw_text(text, widget_rect.min, style::text);
		c_window::current->draw_list->draw_rect_filled(body_rect.min, body_rect.max - vec2_t{ 0.f, style::text_input::line_size }, style::button::box, style::text_input::rounding, render::e_corner_flags::top);
		c_window::current->draw_list->draw_rect_filled(vec2_t{ body_rect.min.x, body_rect.max.y - style::text_input::line_size }, body_rect.max, style::main_color, style::text_input::rounding, render::e_corner_flags::bot);

		c_window::current->draw_list->push_clip_rect(body_rect, true); {
			c_window::current->draw_list->draw_text(buffer, vec2_t{ working_rect.min.x, working_rect.center().y }, style::text, render::e_text_flags::aligin_center_y);

			if(render::c_font* current_font{ render::c_font::get_current_font() }; current_font && active) {
				if(input->selection_data.selecting()) {
					vec2_t start_offset{ current_font->calc_text_size(std::wstring_view{ buffer.begin(), std::next(buffer.begin(), input->selection_data.min()) }) };
					vec2_t selected_size{ current_font->calc_text_size(std::wstring_view{ std::next(buffer.begin(), input->selection_data.min()), std::next(buffer.begin(), input->selection_data.max()) }) };
					c_window::current->draw_list->draw_rect_filled(working_rect.min + vec2_t{ start_offset.x, 0.f }, working_rect.min + start_offset + vec2_t{ selected_size.x, 0.f }, style::text_input::select_text);
				} else if(input->write_data.show) {
					vec2_t write_pos_offset{ current_font->calc_text_size(std::wstring_view{ buffer.begin(), std::next(buffer.begin(), input->write_data.offset) }) };
					c_window::current->draw_list->draw_rect_filled(working_rect.min + vec2_t{ write_pos_offset.x - 0.5f, 0.f }, working_rect.min + write_pos_offset + vec2_t{ 0.5f, 0.f }, { });
				}
			}
		} c_window::current->draw_list->pop_clip_rect();

		return active;
	}

	bool text_input(std::string_view text, std::string* value, bool password) {
		return text_input(text, value, password, impl::e_data_type::ascii_string);
	}

	bool text_input(std::string_view text, int* value) {
		return text_input(text, value, false, impl::e_data_type::integer);
	}

	bool text_input(std::string_view text, float* value) {
		return text_input(text, value, false, impl::e_data_type::floating_point);
	}

	namespace impl {
		c_text_input* c_text_input::create(const c_text_input& input) {
			if(c_text_input* finded{ find(input.name) }) {
				finded->working_rect = input.working_rect;
				return finded;
			}

			text_inputs.push_back(input);
			text_inputs.back().update_buffers();
			return &text_inputs.back();
		}

		c_text_input* c_text_input::find(std::string_view name) {
			if(auto finded{ std::ranges::find_if(text_inputs, [&](const c_text_input& text_input) { return text_input.name == name; }) }; finded != text_inputs.end())
				return &(*finded);
			return nullptr;
		}

		void c_text_input::add_char(const std::uint32_t& symbol) {
			if(input::keys[input::e_key_id::ctrl].is_down() || !filter(symbol)) return;

			if(selection_data.selecting())
				write_data.offset = selection_data.remove_selected(utf8_buffer);

			utf8_buffer.insert(std::next(utf8_buffer.begin(), write_data.offset), symbol);
			ascii_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);

			selection_data.set(++write_data.offset);

			set_data_from_buffer();
		}

		void c_text_input::key_down(input::e_key_id key) {
			write_data.update(style::text_input::write_offset_show);

			switch(key) {
				case input::e_key_id::enter: {
					active_widget.clear();
				} break;

				case input::e_key_id::backspace: {
					if(utf8_buffer.empty()) break;

					if(selection_data.selecting()) write_data.offset = selection_data.remove_selected(utf8_buffer);
					else if(write_data.offset > 0) {
						write_data.offset--;
						utf8_buffer.erase(std::next(utf8_buffer.begin(), write_data.offset));
					}

					ascii_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);
					selection_data.set(write_data.offset);
				} break;
				case input::e_key_id::del: {
					if(utf8_buffer.empty()) break;

					if(selection_data.selecting()) write_data.offset = selection_data.remove_selected(utf8_buffer);
					else if(write_data.offset < utf8_buffer.length()) utf8_buffer.erase(std::next(utf8_buffer.begin(), write_data.offset));

					ascii_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);
					selection_data.set(write_data.offset);
				} break;

				case input::e_key_id::left: {
					if(input::keys[input::e_key_id::shift].is_down()) {
						if(!selection_data.selecting()) selection_data.set(write_data.offset);
						selection_data.end = input::keys[input::e_key_id::ctrl].is_down() ? 0 : std::max(selection_data.end - 1, 0);
					} else if(input::keys[input::e_key_id::ctrl].is_down()) write_data.offset = 0;
					else write_data.offset = selection_data.selecting() ? selection_data.min() : std::max(write_data.offset - 1, 0);

					if(input::keys[input::e_key_id::shift].is_up()) selection_data.set(write_data.offset);
				} break;

				case input::e_key_id::right: {
					if(input::keys[input::e_key_id::shift].is_down()) {
						if(!selection_data.selecting()) selection_data.set(write_data.offset);
						selection_data.end = input::keys[input::e_key_id::ctrl].is_down() ? utf8_buffer.length() : std::min(selection_data.end + 1, (int)utf8_buffer.length());
					} else if(input::keys[input::e_key_id::ctrl].is_down()) write_data.offset = utf8_buffer.length();
					else write_data.offset = selection_data.selecting() ? selection_data.max() : std::min(write_data.offset + 1, (int)utf8_buffer.length());

					if(input::keys[input::e_key_id::shift].is_up()) selection_data.set(write_data.offset);
				} break;

				case input::e_key_id::a: {
					if(input::keys[input::e_key_id::ctrl].is_down()) {
						selection_data.start = 0;
						selection_data.end = utf8_buffer.length();
					}
				} break;

				case input::e_key_id::c: {
					if(input::keys[input::e_key_id::ctrl].is_down() && selection_data.selecting()) {
						utils::win::c_window{ }.write_clipboard<wchar_t>(std::wstring_view{ std::next(utf8_buffer.begin(), selection_data.min()), std::next(utf8_buffer.begin(), selection_data.max()) });
					}
				} break;
				case input::e_key_id::v: {
					if(input::keys[input::e_key_id::ctrl].is_down()) {
						std::wstring clipboard{ utils::win::c_window{ }.read_clipboard<wchar_t>() };
						if(selection_data.selecting())
							write_data.offset = selection_data.remove_selected(utf8_buffer);

						utf8_buffer.insert(write_data.offset, clipboard);
						ascii_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);

						write_data.offset += clipboard.length();
						selection_data.set(write_data.offset);
					}
				} break;
			}

			set_data_from_buffer();
			update_buffers();
			write_data.offset = std::clamp(write_data.offset, 0, (int)utf8_buffer.size());
		}

		int c_text_input::get_hovered_char(std::wstring_view buffer) const {
			if(working_rect.min.x >= input::mouse.pos.x) return 0;
			if(working_rect.max.x <= input::mouse.pos.x) return buffer.length();

			render::c_font* current_font{ render::c_font::get_current_font() };
			if(!current_font) throw std::runtime_error{ "current font empty" };
			for(const auto& iterator : std::views::iota(buffer.begin(), buffer.end())) {
				float end_offset{ current_font->calc_text_size(std::wstring_view{ buffer.begin(), std::next(iterator) }).x };
				if(working_rect.min.x + end_offset >= input::mouse.pos.x) {
					float char_size{ current_font->calc_text_size(std::wstring_view{ iterator, std::next(iterator) }).x };
					float start_offset{ current_font->calc_text_size(std::wstring_view{ buffer.begin(), iterator }).x };
					return working_rect.min.x + start_offset + char_size / 2 <= input::mouse.pos.x ? std::distance(buffer.begin(), iterator) + 1 : std::distance(buffer.begin(), iterator);
				}
			}
			return buffer.length();
		}

		bool c_text_input::filter(const std::uint32_t& symbol) const {
			if((symbol <= 0x44F && symbol >= 0x20 && symbol != 0x7F) || symbol == 0x451 || symbol == 0x401) {
				switch(data_type) {
					case impl::e_data_type::integer: { return (symbol >= 0x0 && symbol <= 0xFF) && isdigit((std::uint8_t)symbol) || symbol == '-'; }
					case impl::e_data_type::floating_point: { return (symbol >= 0x0 && symbol <= 0xFF) && isdigit(symbol) || symbol == '-' || symbol == '.'; }
				}
				return true;
			}
			return false;
		}

		void c_text_input::update_buffers() {
			if(std::string converted_data{ data_to_string() }; converted_data != ascii_buffer) {
				ascii_buffer = converted_data;
				utf8_buffer = utils::encoding::ascii_to_utf8(ascii_buffer);
			}
		}

		std::string c_text_input::data_to_string() const {
			switch(data_type) {
				case impl::e_data_type::integer: { return std::to_string(*(int*)data); }
				case impl::e_data_type::floating_point: { return std::to_string(*(float*)data); }
				case impl::e_data_type::ascii_string: { return *(std::string*)data; }
				default: throw std::runtime_error{ "unsupported data type" };
			}
		}

		void c_text_input::set_data_from_buffer() {
			switch(data_type) {
				case impl::e_data_type::integer: { *(int*)data = atoi(ascii_buffer.c_str()); } break;
				case impl::e_data_type::floating_point: { *(float*)data = atof(ascii_buffer.c_str()); } break;
				case impl::e_data_type::ascii_string: { *(std::string*)data = ascii_buffer; } break;
				default: throw std::runtime_error{ "unsupported data type" };
			}
		}

		namespace behaviors {
			bool text_input(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed) {
				if(c_window::current->can_use_widget(widget_name, widget_rect)) {
					hovered_widget = widget_name;

					if(input::keys[input::e_key_id::mouse_left].is_up()) hovered = true;
					if(widget_rect.contains(input::mouse.pos) && input::keys[input::e_key_id::mouse_left].is_pressed()) {
						active_widget = widget_name;
					}
				}

				if(active_widget == widget_name) {
					if(input::keys[input::e_key_id::mouse_left].is_down()) pressed = true;
					if(input::keys[input::e_key_id::mouse_left].is_pressed() && !widget_rect.contains(input::mouse.pos)) {
						active_widget.clear();
					}
				}

				return active_widget == widget_name;
			}
		}
	}
}