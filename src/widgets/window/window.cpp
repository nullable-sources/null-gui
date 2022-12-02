#include <null-gui.h>

namespace null::gui {
	bool begin_window(std::string_view name, vec2_t pos, vec2_t size, bool* open) {
		return begin_window(name, pos, size, { }, open);
	}

	bool begin_window(std::string_view name, vec2_t pos, vec2_t size, e_window_flags flags, bool* open) {
		c_window* window{ flags & e_window_flags::group ? c_window::current->find_group(name) : c_window::find(name) };
		if(!window) window = c_window::create({ name, pos, size, flags });

		if(open) window->visible = *open;
		if(!window->visible) return false;

		if(window->flags & e_window_flags::child_mask && c_window::current != window && window->parent != c_window::current) {
			window->parent = c_window::current;
		}

		if(window->flags & e_window_flags::set_pos) window->pos = pos;
		if(window->flags & e_window_flags::set_size) {
			if(size.x > 1.f) window->size.x = size.x;
			if(size.y > 1.f) window->size.y = size.y;
		}

		if(window->flags & e_window_flags::popup || window->close_call) { //logic for popups
			if((input::keys[input::e_key_id::mouse_left].is_pressed() && !window->in_popup_region()) || window->close_call) {
				window->close();
				if(c_window::active == window) c_window::active = nullptr;
				return false;
			}
		}

		c_window::current = window;

		if(window->moving_offset != std::numeric_limits<float>::min() && impl::active_widget.empty()) {
			if(input::keys[input::e_key_id::mouse_left].is_up() || c_window::active != window) {
				window->moving_offset = std::numeric_limits<float>::min();
				if(c_window::active == window) c_window::active = nullptr;
			} else {
				window->pos = input::mouse.pos - window->moving_offset;
			}
		}

		window->clamp_on_screen();

		if(c_window::active == window || (input::keys[input::e_key_id::mouse_left].is_pressed() && rect_t{ window->pos }.from_min(window->size).contains(input::mouse.pos) && c_window::hovered == c_window::current)) {
			window->focus();
		}

		window->draw_list->draw_rect_filled(rect_t{ window->pos }.from_min(window->size), window->flags& e_window_flags::popup ? style::popup::background : window->flags & e_window_flags::group ? style::group::background : style::window::background, style::window::rounding, window->get_title_size() == 0.f ? render::e_corner_flags::all : render::e_corner_flags::bot);

		if(!(window->flags & e_window_flags::no_title_line)) window->draw_list->draw_rect_filled(rect_t{ window->pos }.from_min(vec2_t{ window->size.x, style::window::titlebar::line_size }), style::main_color, style::window::titlebar::rounding, render::e_corner_flags::top);
		if(!(window->flags & e_window_flags::popup) && !(window->flags & e_window_flags::no_title_bar)) {
			impl::push_var(&window->pos.y, window->pos.y + (window->flags & e_window_flags::no_title_line ? 0.f : style::window::titlebar::line_size)); {
				window->draw_list->draw_rect_filled(rect_t{ window->pos }.from_min(vec2_t{ window->size.x, style::window::titlebar::size }), style::window::titlebar::background);

				window->draw_list->push_clip_rect(window->pos, window->pos + vec2_t(window->size.x, style::window::titlebar::size), window->flags& e_window_flags::group); {
					window->draw_list->push_clip_rect(window->pos, window->pos + vec2_t(window->size.x - style::window::titlebar::size, style::window::titlebar::size), true); {
						window->draw_list->draw_text(impl::format_widget_name(window->name), window->pos + vec2_t(5.f, style::window::titlebar::size / 2), color_t<int>(255, 255, 255, 255), render::e_text_flags::aligin_center_y);
					} window->draw_list->pop_clip_rect();

					impl::push_var(&window->autopositioning.current_pos, window->pos + vec2_t(window->size.x - (style::window::titlebar::size / 2) - (render::c_font::get_current_font()->calc_text_size("x").x / 2), (style::window::titlebar::size / 2) - (render::c_font::get_current_font()->calc_text_size("x").y / 2))); {
						impl::push_var(&window->flags, e_window_flags{ flags | e_window_flags::ignore_scroll }); {
							impl::push_var(&style::items_size_full_window, false); {
								if(open && clickable_text(std::string{ "x##" }.append(window->name))) {
									if(c_window::active == window) c_window::active = nullptr;
									*open = false;
								}
							} impl::pop_var();
						} impl::pop_var();
					} impl::pop_var();
				} window->draw_list->pop_clip_rect();
			} impl::pop_var();
		}

		window->autopositioning.current_pos = window->pos + vec2_t{ 0.f, window->get_title_size() }  + style::window::padding;
		window->draw_list->push_clip_rect(rect_t{ window->pos } + rect_t{ vec2_t{ 0.f, window->get_title_size() }, window->size }, true);

		begin_scroll();

		window->max_size = 0.f;

		return true;
	}

	void end_window() {
		end_scroll();

		c_window::current->draw_list->pop_clip_rect();

		if(c_window::current->flags & e_window_flags::auto_size) { //auto-size logic
			if(c_window::current->arg_size.x < 1.f) c_window::current->size.x = c_window::current->max_size.x + style::window::padding.x - style::window::item_spacing;
			if(c_window::current->arg_size.y < 1.f) c_window::current->size.y = c_window::current->max_size.y + style::window::padding.y - style::window::item_spacing;
		}

		if(c_window::current->flags & e_window_flags::child_mask) c_window::current = c_window::current->parent;
		else c_window::current = nullptr;
	}

	c_window* c_window::find(std::string_view name) {
		if(auto finded{ std::ranges::find_if(windows, [=](const std::shared_ptr<c_window>& window) { return window->name == name; }) }; finded != windows.end())
			return (*finded).get();
		return nullptr;
	}

	c_window* c_window::create(const c_window& window) {
		std::shared_ptr<c_window> new_window{ std::make_shared<c_window>(window) };

		if(windows.empty() && !(window.flags & e_window_flags::group)) {
			windows.push_back(new_window);
		} else {
			if(window.flags & e_window_flags::popup) {
				current->child_popups.push_back(new_window.get());
				windows.push_back(new_window);
			} else if(window.flags & e_window_flags::group) {
				current->child_groups.push_back(new_window);
			} else windows.push_back(new_window);
		}

		return new_window.get();
	}

	void c_window::windows_handle() {
		for(std::shared_ptr<c_window>& window : windows | std::views::reverse | std::views::filter([](const std::shared_ptr<c_window>& window) { return window->visible; })) {
			if(rect_t(window->pos, window->pos + window->size).contains(input::mouse.pos)) {
				if(!(hovered = window->get_hovered_group()))
					hovered = window.get();
				if(input::keys[input::e_key_id::mouse_left].is_pressed() && (impl::active_widget.empty() && !active) && !(window->flags & e_window_flags::no_move)) {
					if(window->moving_offset == std::numeric_limits<float>::min() && rect_t { window->pos, window->pos + vec2_t(window->size.x, style::window::move_on_titlebar ? window->get_title_size() : window->size.y) }.contains(input::mouse.pos)) {
						window->moving_offset = input::mouse.pos - window->pos;
						active = window.get();
					}
				}
				break;
			}
		}

		for(std::shared_ptr<c_window>& window : windows | std::views::filter([](const std::shared_ptr<c_window>& window) { return window->flags & e_window_flags::popup; })) {
			if(c_window* first_parent{ window->first_parent() }; first_parent && (!find(first_parent->name) || !first_parent->visible)) {
				window->close();
				if(active == window.get()) active = nullptr;
			}
		}
	}

	c_window::c_window(std::string_view _name, vec2_t _pos, vec2_t _size, e_window_flags _flags)
		: name{ _name }, pos{ _pos }, size{ _size }, arg_size{ _size }, flags{ _flags } {
		if(current && flags & e_window_flags::child_mask)
			parent = current;

		if(flags & e_window_flags::group && parent) {
			draw_list = first_parent()->draw_list;
		} else {
			draw_list = std::make_shared<render::c_draw_list>();
			draw_list->reset();
		}
	}

	c_window* c_window::in_popup_region() {
		if(!parent) return nullptr;
		for(c_window* group : parent->child_popups) {
			if(rect_t(group->pos, group->pos + group->size).contains(input::mouse.pos)) return group;
		}
		return nullptr;
	}

	c_window* c_window::get_hovered_group() {
		for(std::shared_ptr<c_window>& group : child_groups) {
			if(group->get_hovered_group() != nullptr) return group->get_hovered_group();
			if(rect_t(group->pos, group->pos + group->size).contains(input::mouse.pos)) return group.get();
		}
		return nullptr;
	}

	c_window* c_window::first_parent() {
		c_window* last{ this };

		if(!last->parent) return last;

		while(last->flags & e_window_flags::child_mask) {
			if(!last->parent) break;
			last = last->parent;
		}

		return last;
	}

	c_window* c_window::find_group(std::string_view name) {
		if(auto finded{ std::ranges::find_if(child_groups, [=](const std::shared_ptr<c_window>& group) { return group->name == name; }) }; finded != child_groups.end())
			return (*finded).get();
		return nullptr;
	}

	void c_window::focus() {
		if(flags & e_window_flags::group && parent) { first_parent()->focus(); return; }
		if(this == windows.back().get()) return;
		if(auto finded{ std::ranges::find_if(windows, [=](const std::shared_ptr<c_window> window) { return window->name == name; }) }; finded != windows.end())
			std::rotate(finded, std::next(finded), windows.end());
	}

	void c_window::clamp_on_screen() {
		if(style::window::clamp_on_screen && !(flags & e_window_flags::group)) {
			pos = math::clamp(pos, vec2_t{ 0.f }, renderer::draw_data_t::screen_size - size);
		}
	}

	vec2_t c_window::get_window_size_with_padding() {
		vec2_t offset{ autopositioning.current_pos - pos - get_padding() };
		return size - (get_padding() * 2) - (vec2_t{ get_scroll_thickness(), 0.f } + vec2_t{ in_column ? (in_same_line ? offset.x - (column_offset - get_padding().x) : 0.f) : offset.x, offset.y});
	}

	float c_window::get_scroll_thickness() { return scrollbar_data.max_scroll != 0 ? style::scrollbar::padding.x + style::scrollbar::thickness : 0.f; }

	float c_window::get_title_size() {
		return (flags & e_window_flags::no_title_bar || flags & e_window_flags::popup ? 0.f : style::window::titlebar::size) + (flags & e_window_flags::no_title_line ? 0.f : style::window::titlebar::line_size);
	}

	bool c_window::can_open_tooltip() {
		if(child_popups.size() > 1) return false;
		else if(child_popups.size() == 1 && child_popups.back()->name == "##tooltip") return true;
		else if(child_popups.empty()) return true;
		return false;
	}

	bool c_window::can_use_widget(std::string_view name, const rect_t& rect) {
		return (impl::hovered_widget.empty() || impl::hovered_widget == name) && ((rect.contains(input::mouse.pos) && draw_list->clips.back().contains(input::mouse.pos) && c_window::hovered == c_window::current) || impl::active_widget == name);
	}

	bool c_window::can_draw_widget(const rect_t& rect) {
		return (rect_t{ pos, pos + size } + vec2_t{ column_offset, 0.f }).intersects(rect);
	}

	void c_window::close() {
		if(flags & e_window_flags::popup && parent) {
			if(auto finded{ std::ranges::find_if(parent->child_popups, [=](c_window* popup) { return popup == this; }) }; finded != parent->child_popups.end())
				parent->child_popups.erase(finded);
		}

		if(auto finded{ std::ranges::find_if(windows, [=](const std::shared_ptr<c_window> window) { return window.get() == this; }) }; finded != windows.end())
			windows.erase(finded);
	}

	void c_window::add_widget(std::string_view name, const vec2_t& size) {
		if(dont_add_item) return;
		autopositioning.set_last_pos();
		autopositioning.prev_pos = autopositioning.current_pos + vec2_t{ size.x + style::window::item_spacing, 0.f };

		autopositioning.current_pos.x = pos.x + (column_offset == 0.f ? style::window::padding.x : column_offset);
		autopositioning.current_pos.y += size.y + style::window::item_spacing;

		if(max_size.x < autopositioning.prev_pos.x - pos.x + get_scroll_thickness()) max_size.x = autopositioning.prev_pos.x - pos.x + get_scroll_thickness();
		if(max_size.y < autopositioning.current_pos.y - pos.y) max_size.y = autopositioning.current_pos.y - pos.y;

		in_same_line = false;
		if(!name.empty()) impl::last_widget = name;
	}
}