#include <null-gui.h>

namespace null::gui {
	std::vector<i_widget*> i_widget::node_t::parent_node() {
		std::vector<i_widget*> node{ };

		if(parent) {
			node = parent->node.parent_node();
			node.push_back(parent);
		}

		return node;
	}

	std::vector<i_widget*> i_widget::node_t::child_node() {
		std::vector<i_widget*> node{ };

		for(std::shared_ptr<i_widget>& child : childs) {
			auto child_node = child->node.child_node();
			node.push_back(child.get());
			node.insert(node.end(), child_node.begin(), child_node.end());
		}

		return node;
	}

	i_widget* i_widget::add_widget(i_widget* widget) {
		widget->node.parent = this;
		widget->setup();
		node.childs.push_back(std::shared_ptr<i_widget>(widget));
		return node.childs.back().get();
	}

	void i_widget::setup() {
		std::ranges::for_each(node.childs | std::views::filter([this](const std::shared_ptr<i_widget>& widget) { return can_handle_child(widget.get()); }), [&](std::shared_ptr<i_widget>& widget) {
			widget->setup();
			});
	}

	void i_widget::draw() {
		std::vector<i_widget*> draw_on_top_layer{ };
		for(std::shared_ptr<i_widget>& widget : node.childs | std::views::filter([this](const std::shared_ptr<i_widget>& widget) { return can_draw_child(widget.get()) && can_handle_child(widget.get()); })) {
			if(widget->flags & e_widget_flags::draw_on_top_layer) draw_on_top_layer.push_back(widget.get());
			else widget->draw();
		}

		std::ranges::for_each(draw_on_top_layer, [](i_widget* widget) { widget->draw(); });
	}

	bool i_widget::can_handle_child(i_widget* widget) {
		if(!(widget->flags & e_widget_flags::visible)) return false;
		const std::vector<std::any>& results{ callbacks.call<bool(i_widget*)>(e_widget_callbacks::can_handle_child, widget) };
		if(const auto& first_result{ std::ranges::find_if(results, [](const auto& result) { return result.has_value(); }) }; first_result != results.end())
			return std::any_cast<bool>(*first_result);
		return true;
	}

	void i_widget::on_focused() {
		state |= e_widget_state::focused;
		widgets[e_widget_state::focused] = this;

		for(i_widget* parent : node.parent_node() | std::views::reverse)
			parent->on_child_focused(this);

		callbacks.call<void()>(e_widget_callbacks::on_focused);
	}

	void i_widget::on_lost_focus(i_widget* new_focused_widget) {
		if(new_focused_widget == widgets[e_widget_state::focused]) return;

		state &= ~e_widget_state::focused;
		widgets[e_widget_state::focused] = nullptr;

		for(i_widget* parent : node.parent_node() | std::views::reverse)
			parent->on_child_lost_focus(this, new_focused_widget);

		callbacks.call<void()>(e_widget_callbacks::on_lost_focus);
	}

	void i_widget::on_mouse_enter() {
		state |= e_widget_state::hovered;
		widgets[e_widget_state::hovered] = this;

		callbacks.call<void()>(e_widget_callbacks::on_mouse_enter);
	}

	void i_widget::on_mouse_exit() {
		state &= ~e_widget_state::hovered;
		widgets[e_widget_state::hovered] = nullptr;

		callbacks.call<void()>(e_widget_callbacks::on_mouse_exit);
	}

	void i_widget::on_mouse_key_down() {
		state |= e_widget_state::active;
		widgets[e_widget_state::active] = this;

		callbacks.call<void()>(e_widget_callbacks::on_mouse_key_down);
	}

	void i_widget::on_mouse_key_up() {
		state &= ~e_widget_state::active;
		widgets[e_widget_state::active] = nullptr;

		callbacks.call<void()>(e_widget_callbacks::on_mouse_key_up);
	}

	bool i_widget::event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) {
		if(!(flags & e_widget_flags::visible)) return false;
		std::vector<i_widget*> top_layer_childs{ };
		for(std::shared_ptr<i_widget>& child_widget : node.childs | std::views::filter([this](const std::shared_ptr<i_widget>& widget) { return can_draw_child(widget.get()) && can_handle_child(widget.get()); })) {
			if(child_widget->flags & e_widget_flags::draw_on_top_layer) top_layer_childs.push_back(child_widget.get());
			else if(child_widget->event_handling(event, w_param, l_param)) {
				on_child_event_handled(child_widget.get());
				return true;
			}
		}

		for(i_widget* child : top_layer_childs) {
			if(child->event_handling(event, w_param, l_param)) {
				on_child_event_handled(child);
				return true;
			}
		}

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
					on_mouse_key_down();

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
					on_mouse_key_up();

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
}