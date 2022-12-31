#include <null-gui.h>

namespace null::gui {
	bool c_popup::can_close_on_lost_focus(i_widget* new_focused_widget) {
		if(new_focused_widget) {
			if(auto parent_node = new_focused_widget->node.parent_node();
				std::ranges::find(parent_node, this) != parent_node.end()) return false;
		}

		if(auto parent_node = node.parent_node();
			std::ranges::find_if(parent_node, [=](i_widget* widget) {
				if(dynamic_cast<i_container*>(widget) && !dynamic_cast<c_popup*>(widget))
					return false;

				return widget == new_focused_widget;
				}) != parent_node.end()) return false;

		return true;
	}

	void c_popup::open() {
		i_container::open();

		node.parent->flags |= e_widget_flags::on_top_layer;
	}

	void c_popup::close() {
		i_container::close();

		node.parent->flags &= ~e_widget_flags::on_top_layer;
	}

	void c_popup::append_auto_size() {
		i_container::append_auto_size();
		if(flags & e_container_flags::auto_size_x) size.x += style.padding.x;
		if(flags & e_container_flags::auto_size_y) size.y += style.padding.y;
	}

	void c_popup::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		auto_positioning.next_position.y += style.widget_padding;
	}

	void c_popup::setup() {
		working_region = { vec2_t{ 0.f, style.titlebar_height } + style.padding, size - style.padding };
		i_container::setup();
	}

	void c_popup::draw() {
		gui_layer.push_clip_rect(pos, pos + size);
		
		gui_layer.draw_rect_filled(pos, pos + size, style.background_color);
		gui_layer.draw_rect_filled(pos, pos + vec2_t{ size.x, style.titlebar_height }, style.titlebar_color);
		gui_layer.draw_text(name, rect_t{ pos, pos + vec2_t{ size.x, style.titlebar_height } }.origin(rect_t::center), { }, render::e_text_flags::aligin_center);

		i_container::draw();
		
		gui_layer.pop_clip_rect();
	}

	void c_popup::on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) {
		i_widget::on_child_lost_focus(child, new_focused_widget);
		
		if(new_focused_widget == this) return;
		if(can_close_on_lost_focus(new_focused_widget)) close();
	}

	void c_popup::on_focused() {
		for(i_widget* widget : node.child_node()) { //@note: closing all child popups
			if(c_popup * popup{ dynamic_cast<c_popup*>(widget) })
				popup->close();
		}

		i_widget::on_focused();
	}

	void c_popup::on_lost_focus(i_widget* new_focused_widget) {
		i_widget::on_lost_focus(new_focused_widget);

		if(can_close_on_lost_focus(new_focused_widget)) close();
	}
}