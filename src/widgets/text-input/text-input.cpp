#include <null-gui.h>

namespace null::gui {
	bool c_text_input::filter(const std::uint32_t& symbol) {
		return (symbol <= 0x44F && symbol >= 0x20 && symbol != 0x7F) || symbol == 0x451 || symbol == 0x401;
	}

	int c_text_input::get_hovered_char() {
		if(field_region.min.x >= input::mouse.pos.x - pos.x) return 0;
		if(field_region.max.x <= input::mouse.pos.x - pos.x) return utf8_buffer.length();

		render::c_font* current_font{ render::c_font::get_current_font() };
		if(!current_font) throw std::runtime_error{ "current font empty" };
		for(const auto& iter : std::views::iota(utf8_buffer.begin(), utf8_buffer.end())) {
			float end_offset{ current_font->calc_text_size(std::wstring_view{ utf8_buffer.begin(), std::next(iter) }).x };
			if(field_region.min.x + end_offset >= input::mouse.pos.x - pos.x) {
				float char_size{ current_font->calc_text_size(std::wstring_view{ iter, std::next(iter) }).x };
				float start_offset{ current_font->calc_text_size(std::wstring_view{ utf8_buffer.begin(), iter }).x };
				return field_region.min.x + start_offset + char_size / 2 <= input::mouse.pos.x - pos.x ? std::distance(utf8_buffer.begin(), iter) + 1 : std::distance(utf8_buffer.begin(), iter);
			}
		}
		return utf8_buffer.length();
	}

	bool c_text_input::can_hovered() const {
		if(!i_widget::can_hovered()) return false;
		return working_region.contains(input::mouse.pos - pos);
	}

	void c_text_input::setup_self() {
		if(state & e_widget_state::active && write_data.next_change_time <= std::chrono::steady_clock::now())
			write_data.update(write_data.show ? style.write_pos_hide_duration : style.write_pos_show_duration, !write_data.show);
		
		vec2_t name_size{ render::c_font::get_current_font()->calc_text_size(name) };
		size = vec2_t{ std::max(name_size.x, node.parent->working_region.size().x), name_size.y * 2 + style.field_offset + style.input_text_offset.y * 2 };

		working_region.min = vec2_t{ 0.f, name_size.y + style.field_offset };
		working_region.max = working_region.min + vec2_t{ size.x, name_size.y + style.input_text_offset.y * 2};
		field_region = { working_region.min + style.input_text_offset, working_region.max - style.input_text_offset };
	}

	void c_text_input::draw() {
		color_t field_color{ style.default_color };
		if(state & e_widget_state::active) field_color = style.active_color;
		else if(state & e_widget_state::hovered) field_color = style.hovered_color;

		gui_layer.draw_text(name, pos, { });
		gui_layer.draw_rect_filled(working_region + pos, field_color);

		gui_layer.push_clip_rect(working_region + pos, true);
		gui_layer.draw_text(utf8_buffer, vec2_t{ working_region.min.x + style.input_text_offset.x, working_region.origin(rect_t::center).y } + pos, { }, render::e_text_flags::aligin_center_y);

		if(render::c_font* current_font{ render::c_font::get_current_font() }; current_font && state & e_widget_state::active) {
			if(selection_data.selecting()) {
				vec2_t start_offset{ current_font->calc_text_size(std::wstring_view{ utf8_buffer.begin(), utf8_buffer.begin() + selection_data.min() }) };
				vec2_t selected_size{ current_font->calc_text_size(std::wstring_view{ utf8_buffer.begin() + selection_data.min(), utf8_buffer.begin() + selection_data.max() }) };
				gui_layer.draw_rect_filled(field_region.min + vec2_t{ start_offset.x, 0.f } + pos, field_region.min + start_offset + vec2_t{ selected_size.x, 0.f } + pos, style.selection_rect);
			} else if(write_data.show) {
				vec2_t write_pos_offset{ current_font->calc_text_size(std::wstring_view{ utf8_buffer.begin(), utf8_buffer.begin() + write_data.offset }) };
				gui_layer.draw_rect_filled(field_region.min + vec2_t{ write_pos_offset.x - 0.5f, 0.f } + pos, field_region.min + write_pos_offset + vec2_t{ 0.5f, 0.f } + pos, { });
			}
		}

		gui_layer.pop_clip_rect();

		i_widget::draw();
	}

	void c_text_input::on_mouse_move() {
		if(!(state & e_widget_state::active) || input::keys[input::e_key_id::mouse_left].is_up()) return;

		selection_data.end = get_hovered_char();
		i_widget::on_mouse_move();
	}

	void c_text_input::on_mouse_key_down(const input::e_key_id& key) {
		write_data.offset = get_hovered_char();
		write_data.update(style.write_pos_show_duration);
		selection_data.set(write_data.offset);

		if(!(state & e_widget_state::active)) {			
			state |= e_widget_state::active;
			widgets[e_widget_state::active] = this;
		} else if(!(working_region + pos).contains(input::mouse.pos)) {
			state &= ~e_widget_state::active;
			widgets[e_widget_state::active] = nullptr;
		}

		callbacks.at<e_widget_callbacks::on_mouse_key_down>().call(key);
	}

	void c_text_input::on_key_down(const input::e_key_id& key) {
		if(!(state & e_widget_state::active)) return;

		write_data.update(style.write_pos_show_duration);

		switch(key) {
			case input::e_key_id::enter: {
				state &= ~e_widget_state::active;
				widgets[e_widget_state::active] = nullptr;
			} break;

			case input::e_key_id::backspace: {
				if(utf8_buffer.empty()) break;

				if(selection_data.selecting()) write_data.offset = selection_data.remove_selected(utf8_buffer);
				else if(write_data.offset > 0) {
					write_data.offset--;
					utf8_buffer.erase(std::next(utf8_buffer.begin(), write_data.offset));
				}

				*output_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);
				selection_data.set(write_data.offset);
			} break;

			case input::e_key_id::del: {
				if(utf8_buffer.empty()) break;

				if(selection_data.selecting()) write_data.offset = selection_data.remove_selected(utf8_buffer);
				else if(write_data.offset < utf8_buffer.length()) utf8_buffer.erase(std::next(utf8_buffer.begin(), write_data.offset));

				*output_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);
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

			case input::e_key_id::v: {
				if(input::keys[input::e_key_id::ctrl].is_down()) {
					std::wstring clipboard{ utils::win::c_window{ }.read_clipboard<wchar_t>() };
					if(selection_data.selecting())
						write_data.offset = selection_data.remove_selected(utf8_buffer);

					utf8_buffer.insert(write_data.offset, clipboard);
					*output_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);

					write_data.offset += clipboard.length();
					selection_data.set(write_data.offset);
				}
			} break;

			case input::e_key_id::c: {
				if(input::keys[input::e_key_id::ctrl].is_down() && selection_data.selecting()) {
					utils::win::c_window{ }.write_clipboard<wchar_t>(std::wstring_view{ std::next(utf8_buffer.begin(), selection_data.min()), std::next(utf8_buffer.begin(), selection_data.max()) });
				}
			} break;
		}

		i_widget::on_key_down(key);
	}

	void c_text_input::on_char_add(const std::uint32_t& added_char) {
		if(!(state & e_widget_state::active) || !filter(added_char)) return;

		if(selection_data.selecting())
			write_data.offset = selection_data.remove_selected(utf8_buffer);

		utf8_buffer.insert(std::next(utf8_buffer.begin(), write_data.offset), added_char);
		*output_buffer = utils::encoding::utf8_to_ascii(utf8_buffer);
		
		selection_data.set(++write_data.offset);

		i_widget::on_char_add(added_char);
	}
}