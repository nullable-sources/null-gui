#include <null-gui.h>

namespace null::gui {
	void i_container::setup_bounds() {
		if(node.parent) {
			bounds = { math::max(bounds.min, node.parent->bounds.min), math::min(bounds.max, node.parent->bounds.max) };
		}
	}

	void i_container::setup_widget(i_widget* widget) {
		if(!(widget->flags & e_widget_flags::ignore_auto_positioning)) {
			widget->pos = pos + auto_positioning.next_position;
			widget->pos.y -= scroll_bar.get_scroll_value();
		}

		widget->setup();

		if(!(widget->flags & e_widget_flags::ignore_auto_positioning)) {
			append_auto_positioning(widget);
		}

		append_auto_size();
	}

	void i_container::setup_scroll_bar() {
		if(container_flags & e_container_flags::disable_scroll_bar) return;

		scroll_bar.max_scroll_value = auto_positioning.max_position.y - working_region.max.y;
		scroll_bar.setup();
		if(scroll_bar.max_scroll_value > 0) working_region.max.x -= (pos.x + size.x) - scroll_bar.pos.x;
	}

	void i_container::draw_scroll_bar() {
		if(container_flags & e_container_flags::disable_scroll_bar) return;
		scroll_bar.draw();
	}

	bool i_container::handle_scroll_bar_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		if(container_flags & e_container_flags::disable_scroll_bar) return false;
		return scroll_bar.event_handling(event, w_param, l_param);
	}

	void i_container::append_auto_size() {
		if(container_flags & e_container_flags::auto_size_x && auto_positioning.max_position.x) size.x = auto_positioning.max_position.x;
		if(container_flags & e_container_flags::auto_size_y && auto_positioning.max_position.y) size.y = auto_positioning.max_position.y;
	}

	void i_container::setup_auto_positioning() {
		auto_positioning = auto_positioning_t{ 0.f, working_region.min };
	}

	void i_container::append_auto_positioning(i_widget* widget) {
		auto_positioning.max_position = math::max(auto_positioning.max_position, widget->pos + widget->size - vec2_t{ pos.x, pos.y - scroll_bar.get_scroll_value() });
		auto_positioning.next_position.y += widget->size.y;
	}

	i_widget* i_container::add_widget(i_widget* widget) {
		widget->node.parent = this;
		node.childs.push_back(std::shared_ptr<i_widget>(widget));

		setup_widget(widget);
		return node.childs.back().get();
	}

	void i_container::setup() {
		setup_bounds();
		setup_scroll_bar();
		setup_auto_positioning();

		std::ranges::for_each(node.childs | std::views::filter([this](const std::shared_ptr<i_widget>& widget) { return can_handle_child(widget.get()); }), [this](const std::shared_ptr<i_widget>& widget) { setup_widget(widget.get()); });

		append_auto_size();
	}

	void i_container::draw() {
		draw_scroll_bar();
		i_widget::draw();
		/*if(dynamic_cast<c_columns*>(this))
			gui_layer.draw_rect_filled(pos + working_region.min, pos + working_region.max, { 255, 0, 0, 100 });
		else if(dynamic_cast<c_columns::c_column*>(this)) {
			gui_layer.draw_rect_filled(pos + working_region.min, pos + working_region.max, { 0, 0, 255, 100 });
		}*/
	}

	bool i_container::event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		return handle_scroll_bar_events(event, w_param, l_param) || i_widget::event_handling(event, w_param, l_param);
	}
}