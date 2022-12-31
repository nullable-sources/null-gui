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
			if(scroll_bar) widget->pos.y -= scroll_bar->get_scroll_value();
		}

		widget->setup();

		if(!(widget->flags & e_widget_flags::ignore_auto_positioning)) {
			append_auto_positioning(widget);
		}

		append_auto_size();
	}

	void i_container::append_auto_size() {
		if(flags & e_container_flags::auto_size_x && auto_positioning.max_position.x) size.x = auto_positioning.max_position.x;
		if(flags & e_container_flags::auto_size_y && auto_positioning.max_position.y) size.y = auto_positioning.max_position.y;
	}

	void i_container::setup_auto_positioning() {
		auto_positioning = auto_positioning_t{ 0.f, working_region.min };
	}

	void i_container::append_auto_positioning(i_widget* widget) {
		auto_positioning.max_position = math::max(auto_positioning.max_position, widget->pos + widget->size - vec2_t{ pos.x, pos.y - (scroll_bar ? scroll_bar->get_scroll_value() : 0.f) });
		auto_positioning.next_position.y += widget->size.y;
	}

	i_widget* i_container::add_widget(i_widget* widget) {
		widget->node.parent = this;
		node.childs.push_back(std::shared_ptr<i_widget>(widget));

		setup_widget(widget);
		return node.childs.back().get();
	}

	void i_container::setup() {
		if(scroll_bar) scroll_bar->max_scroll_value = auto_positioning.max_position.y - working_region.max.y;
		setup_bounds();
		setup_auto_positioning();

		std::ranges::for_each(node.childs | std::views::filter([this](const std::shared_ptr<i_widget>& widget) { return can_handle_child(widget.get()); }), [this](const std::shared_ptr<i_widget>& widget) { setup_widget(widget.get()); });

		append_auto_size();
	}
}