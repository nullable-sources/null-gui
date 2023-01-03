#include <null-gui.h>

namespace null::gui::interfaces {
	bool i_popup::can_close_on_lost_focus(i_widget* new_focused_widget) const {
		if(new_focused_widget) {
			if(auto parent_node{ new_focused_widget->node.parent_node() };
				std::ranges::find(parent_node, this) != parent_node.end()) return false;
		}

		if(auto parent_node{ node.parent_node() };
			std::ranges::find_if(parent_node, [=](i_widget* widget) {
				if(dynamic_cast<i_container*>(widget) && !dynamic_cast<i_popup*>(widget))
				return false;

		return widget == new_focused_widget;
				}) != parent_node.end()) return false;

		return true;
	}

	void i_popup::open() {
		i_container::open();

		node.parent->flags |= e_widget_flags::on_top_layer;
	}

	void i_popup::close() {
		i_container::close();

		node.parent->flags &= ~e_widget_flags::on_top_layer;
	}

	void i_popup::on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) {
		i_widget::on_child_lost_focus(child, new_focused_widget);

		if(new_focused_widget == this) return;
		if(can_close_on_lost_focus(new_focused_widget)) close();
	}

	void i_popup::on_focused() {
		for(i_widget* widget : node.child_node()) { //@note: closing all child popups
			if(i_popup * popup{ dynamic_cast<i_popup*>(widget) })
				popup->close();
		}

		i_widget::on_focused();
	}

	void i_popup::on_lost_focus(i_widget* new_focused_widget) {
		i_widget::on_lost_focus(new_focused_widget);

		if(can_close_on_lost_focus(new_focused_widget)) close();
	}
}