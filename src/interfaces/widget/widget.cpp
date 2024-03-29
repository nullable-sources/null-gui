#include <null-gui.h>

namespace null::gui::interfaces {
	std::vector<i_widget*> i_widget::node_t::parent_node() const {
		std::vector<i_widget*> node{ };

		if(parent) {
			node = parent->node.parent_node();
			node.push_back(parent);
		}

		return node;
	}

	std::vector<i_widget*> i_widget::node_t::child_node() const {
		std::vector<i_widget*> node{ };

		for(const std::shared_ptr<i_widget>& child : childs) {
			auto child_node{ child->node.child_node() };
			node.push_back(child.get());
			node.insert(node.end(), child_node.begin(), child_node.end());
		}

		return node;
	}

	i_widget* i_widget::add_widget(i_widget* widget) {
		widget->node.parent = this;
		setup_child(widget);
		node.childs.push_back(std::shared_ptr<i_widget>(widget));
		return node.childs.back().get();
	}

	void i_widget::setup_childs() {
		std::ranges::for_each(node.childs | std::views::filter([this](const auto& child) { return can_handle_child(child); }), [&](std::shared_ptr<i_widget>& widget) {
			setup_child(widget.get());
			});
	}

	void i_widget::setup() {
		setup_self();
		setup_childs();
	}

	void i_widget::draw() {
		std::vector<i_widget*> on_top_layer{ };
		for(std::shared_ptr<i_widget>& widget : node.childs | std::views::filter([this](const auto& widget) { return can_draw_child(widget) && can_handle_child(widget); })) {
			if(widget->flags & e_widget_flags::on_top_layer) on_top_layer.push_back(widget.get());
			else widget->draw();
		}

		std::ranges::for_each(on_top_layer, [](i_widget* widget) { widget->draw(); });
	}

	void i_widget::on_focused() {
		state |= e_widget_state::focused;
		widgets[e_widget_state::focused] = this;

		for(i_widget* parent : node.parent_node() | std::views::reverse)
			parent->on_child_focused(this);

		callbacks.at<e_widget_callbacks::on_focused>().call();
	}

	void i_widget::on_lost_focus(i_widget* new_focused_widget) {
		if(new_focused_widget == widgets[e_widget_state::focused]) return;

		state &= ~e_widget_state::focused;
		widgets[e_widget_state::focused] = nullptr;

		for(i_widget* parent : node.parent_node() | std::views::reverse)
			parent->on_child_lost_focus(this, new_focused_widget);

		callbacks.at<e_widget_callbacks::on_lost_focus>().call(new_focused_widget);
	}

	void i_widget::on_mouse_enter() {
		state |= e_widget_state::hovered;
		widgets[e_widget_state::hovered] = this;

		callbacks.at<e_widget_callbacks::on_mouse_enter>().call();
	}

	void i_widget::on_mouse_exit() {
		state &= ~e_widget_state::hovered;
		widgets[e_widget_state::hovered] = nullptr;

		callbacks.at<e_widget_callbacks::on_mouse_exit>().call();
	}

	void i_widget::on_mouse_key_down(const input::e_key_id& key) {
		state |= e_widget_state::active;
		widgets[e_widget_state::active] = this;

		callbacks.at<e_widget_callbacks::on_mouse_key_down>().call(key);
	}

	void i_widget::on_mouse_key_up(const input::e_key_id& key) {
		state &= ~e_widget_state::active;
		widgets[e_widget_state::active] = nullptr;

		callbacks.at<e_widget_callbacks::on_mouse_key_up>().call(key);
	}

	bool i_widget::handling_child_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		std::vector<i_widget*> bottom_layer_childs{ };
		for(std::shared_ptr<i_widget>& child_widget : node.childs | std::views::filter([this](const auto& widget) { return can_draw_child(widget) && can_handle_child(widget); })) {
			if(child_widget->flags & e_widget_flags::on_top_layer) {
				if(child_widget->event_handling(event, w_param, l_param)) {
					on_child_event_handled(event, child_widget.get());
					return true;
				}
			} else bottom_layer_childs.push_back(child_widget.get());
		}

		for(i_widget* child : bottom_layer_childs) {
			if(child->event_handling(event, w_param, l_param)) {
				on_child_event_handled(event, child);
				return true;
			}
		}

		return false;
	}

	bool i_widget::handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		switch(event) {
			case e_widget_event::mouse_move: {
				if(widgets[e_widget_state::active] && widgets[e_widget_state::active] != this) return false;
				if(can_hovered() || (widgets[e_widget_state::active] == this)) {
					if(!(state & e_widget_state::hovered)) {
						if(widgets[e_widget_state::hovered]) {
							widgets[e_widget_state::hovered]->on_mouse_exit();
						}

						on_mouse_enter();
					}

					on_mouse_move();

					return true;
				}
			} break;
			case e_widget_event::mouse_key_down: {
				if(state & e_widget_state::hovered && (!widgets[e_widget_state::active] || widgets[e_widget_state::active] == this)) {
					on_mouse_key_down((input::e_key_id)(w_param >> 8));

					if(widgets[e_widget_state::focused] && widgets[e_widget_state::focused] != this) {
						widgets[e_widget_state::focused]->on_lost_focus(this);
					}

					if(!(state & e_widget_state::focused)) {
						on_focused();
					}

					return true;
				}
			} break;
			case e_widget_event::mouse_key_up: {
				if(state & e_widget_state::active) {
					on_mouse_key_up((input::e_key_id)(w_param >> 8));

					return true;
				}
			} break;

			case e_widget_event::mouse_wheel: {
				if(state & e_widget_state::hovered && !widgets[e_widget_state::active]) {
					on_mouse_wheel();

					return true;
				}
			} break;

			case e_widget_event::key_down: {
				if(state & e_widget_state{ -e_widget_state::hovered | -e_widget_state::focused | -e_widget_state::active }) {
					on_key_down((input::e_key_id)w_param);
					return true;
				}
			} break;

			case e_widget_event::key_up: {
				if(state & e_widget_state{ -e_widget_state::hovered | -e_widget_state::focused | -e_widget_state::active }) {
					on_key_up((input::e_key_id)w_param);
					return true;
				}
			} break;

			case e_widget_event::char_add: {
				if(state & e_widget_state{ -e_widget_state::hovered | -e_widget_state::focused | -e_widget_state::active }) {
					on_char_add(w_param);
					return true;
				}
			} break;
		}

		return false;
	}

	bool i_widget::event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		if(!(flags & e_widget_flags::visible)) return false;
		return handling_child_events(event, w_param, l_param) || handling_self_events(event, w_param, l_param);
	}
}