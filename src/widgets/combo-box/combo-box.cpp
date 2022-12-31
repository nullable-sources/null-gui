#include <null-gui.h>

namespace null::gui {
	void c_combo_box::c_list_popup::setup_bounds() {
		bounds = { pos, pos + size };
	}

	void c_combo_box::c_list_popup::setup_scroll_bar() {
		i_container::setup_scroll_bar();
		if(scroll_bar.max_scroll_value > 0) working_region.max.x -= style.scrollbar_padding.x;
	}

	void c_combo_box::c_list_popup::append_auto_size() {
		size.y = clamped_size ? clamped_size : auto_positioning.max_position.y;
	}

	void c_combo_box::c_list_popup::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		if(style.max_show_items < node.childs.size() && widget == node.childs[style.max_show_items - 1].get())
			clamped_size = auto_positioning.max_position.y;
	}

	void c_combo_box::c_list_popup::setup() {
		pos = vec2_t{ node.parent->working_region.min.x, node.parent->working_region.max.y + style.offset_from_box } + node.parent->pos;
		size.x = node.parent->working_region.size().x;
		working_region = { vec2_t{ 0 }, size };
		i_container::setup();
	}

	void c_combo_box::c_list_popup::draw() {
		gui_layer.push_clip_rect(pos, pos + size);

		gui_layer.draw_rect_filled(pos, pos + size, style.background_color);

		i_container::draw();

		gui_layer.pop_clip_rect();
	}

	std::string c_combo_box::selected_items() {
		std::string result{ };
		for(i_selectable* selectable : popup->node.childs
			| std::views::transform([](const std::shared_ptr<i_widget>& widget) { return dynamic_cast<i_selectable*>(widget.get()); })
			| std::views::filter([](i_selectable* selectable) { return selectable && selectable->selected(); })) {
			result.append(result.empty() ? "" : ", ").append(selectable->name);
		}
		return result;
	}

	bool c_combo_box::can_hovered() {
		if(!i_widget::can_hovered()) return false;
		return bar_region.contains(input::mouse.pos);
	}

	void c_combo_box::setup() {
		vec2_t name_size{ render::c_font::get_current_font()->calc_text_size(name) };
		size = vec2_t{ std::max(name_size.x, node.parent->working_region.size().x), name_size.y * 2 + style.bar_offset + style.selected_name_offset.y * 2 };

		working_region.min = vec2_t{ 0.f, name_size.y + style.bar_offset };
		working_region.max = working_region.min + vec2_t{ size.x, name_size.y + style.selected_name_offset.y * 2 };

		bar_region = working_region + pos;
		i_widget::setup();
	}

	void c_combo_box::draw() {
		color_t field_color{ style.default_color };
		if(state & e_widget_state::active) field_color = style.active_color;
		else if(state & e_widget_state::hovered) field_color = style.hovered_color;

		gui_layer.draw_text(name, pos, style.text_color);
		gui_layer.draw_rect_filled(bar_region, field_color);

		gui_layer.push_clip_rect(bar_region, true);
		gui_layer.draw_text(selected_items(), vec2_t{ bar_region.min.x + style.selected_name_offset.x, bar_region.origin(rect_t::center).y }, { }, render::e_text_flags::aligin_center_y);
		gui_layer.pop_clip_rect();

		i_widget::draw();
	}

	void c_combo_box::on_mouse_key_up(const input::e_key_id& key) {
		if(!(popup->flags & e_widget_flags::visible)) {
			on_lost_focus(popup);
			popup->open();
			popup->on_focused();
		} else {
			popup->close();
		}
		i_widget::on_mouse_key_up(key);
	}
}