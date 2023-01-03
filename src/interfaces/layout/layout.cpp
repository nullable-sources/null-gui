#include <null-gui.h>

namespace null::gui::interfaces {
	void i_layout::append_auto_size() {
		if(flags & e_layout_flags::auto_size_x && auto_positioning.max_position.x) size.x = auto_positioning.max_position.x + style.padding.x;
		if(flags & e_layout_flags::auto_size_y && auto_positioning.max_position.y) size.y = auto_positioning.max_position.y + style.padding.y;
	}

	void i_layout::append_auto_positioning(i_widget* child) {
		child->pos = pos + auto_positioning.next_position - child->size * rect_t::scale_from_origin(origin);
		if(!(child->flags & e_widget_flags::ignore_scroll_offset)) child->pos.y -= get_scroll_value();
	}

	void i_layout::setup_self() {
		working_region = rect_t{ style.padding, math::max(size - style.padding, style.padding) }; //@todo: it will break if scrollbar is used
		if(flags & e_layout_flags::auto_size_by_childs) {
			if(const auto& childs{ flags & e_layout_flags::dont_skip_invisible_childs ? node.childs : node.childs | std::views::filter([](const auto& widget) { return widget->flags & e_widget_flags::visible; }) | std::ranges::to<std::vector>() }; !childs.empty()) {
				vec2_t new_size{ (size - style.spacing * (childs.size() - 1)) / childs.size() - style.padding };
				if(flags & e_layout_flags::auto_size_by_childs_x) working_region.max.x = new_size.x;
				if(flags & e_layout_flags::auto_size_by_childs_y) working_region.max.y = new_size.y;
			}
		}

		setup_scrollbar();

		auto_positioning = auto_positioning_t{ std::numeric_limits<float>::min(), working_region.origin(origin) };
		setup_bounds();
	}

	void i_layout::setup_childs() {
		i_widget::setup_childs();
		append_auto_size();
	}

	void i_layout::setup_child(i_widget* child) {
		child->setup_self();

		if(!(child->flags & e_widget_flags::ignore_auto_positioning)) append_auto_positioning(child);

		child->setup_childs();

		if(!(child->flags & e_widget_flags::ignore_auto_positioning)) {
			auto_positioning.max_position = math::max(auto_positioning.max_position, child->pos + child->size - vec2_t{ pos.x, pos.y - get_scroll_value() });
			auto_positioning.next_position += (child->size + style.spacing) * (origin == rect_t::center ? rect_t::scale_from_origin<-1.f, 1.f>(direction) : rect_t::scale_from_origin<-0.5f, 0.5f>(direction) - rect_t::scale_from_origin<-0.5f, 0.5f>(origin));

			//@todo: rewrite this trash
			vec2_t origin_scale{ rect_t::scale_from_origin(origin) }, direction_scale{ rect_t::scale_from_origin(direction) };
			vec2_t delta{ working_region.origin(origin) - auto_positioning.next_position };
			if(origin_scale.x <= direction_scale.x) working_region.min.x -= delta.x;
			else working_region.max.x -= delta.x;
			if(origin_scale.y <= direction_scale.y) working_region.min.y -= delta.y;
			else working_region.max.y -= delta.y;
		}

		append_auto_size();
	}

	void i_layout::draw() {
		if(flags & e_layout_flags::clip_bounds) gui_layer.push_clip_rect(get_real_bounds(), true);
		i_widget::draw();
		if(flags & e_layout_flags::clip_bounds) gui_layer.pop_clip_rect();
	}

	void i_layout::setup_scrollbar() {
		if(!scroll_bar) return;
		scroll_bar->max_scroll_value = auto_positioning.max_position.y - working_region.max.y;
		if(scroll_bar->max_scroll_value <= 0.f && scroll_bar->flags & e_widget_flags::visible) scroll_bar->flags &= ~e_widget_flags::visible;
		else if(scroll_bar->max_scroll_value > 0.f && !(scroll_bar->flags & e_widget_flags::visible)) { scroll_bar->flags |= e_widget_flags::visible; }
	}

	float i_layout::get_scroll_value() {
		if(!scroll_bar) return 0.f;
		return scroll_bar->get_scroll_value();
	}
}