#pragma once
#include <iostream>

namespace null::gui {
	enum class e_widget_flags {
		none,
		visible = 1 << 0,
		ignore_auto_positioning = 1 << 1,
		draw_on_top_layer = 1 << 2
	}; enum_create_bit_operators(e_widget_flags, true);
	enum_create_cast_operator(e_widget_flags, -);

	enum class e_widget_callbacks {
		on_focused,
		on_lost_focus,

		on_mouse_move,
		on_mouse_enter,
		on_mouse_exit,
		on_mouse_key_down,
		on_mouse_key_up,

		on_mouse_wheel,

		on_key_down,
		on_key_up,
		on_char_add,

		can_handle_child, //@note bool(i_widget* child)
	};

	enum class e_widget_event {
		mouse_move,
		mouse_key_down,
		mouse_key_up,
		mouse_wheel,

		key_down,
		key_up,
		char_add
	};

	enum class e_widget_state {
		none,
		active = 1 << 0,
		hovered = 1 << 1,
		focused = 1 << 2
	}; enum_create_bit_operators(e_widget_state, true);
	enum_create_cast_operator(e_widget_state, -);

	class i_widget {
	public:
		static inline std::map<e_widget_state, i_widget*> widgets{ };

#ifdef _DEBUG
	private:
		std::array<int, 100000> debug_memory_array{ }; //@note: for check the memory release
#endif

	public:
		struct node_t {
			i_widget* parent{ };
			std::vector<std::shared_ptr<i_widget>> childs{ };

			std::vector<i_widget*> parent_node();
			std::vector<i_widget*> child_node();
		};

	public:
		e_widget_flags flags{ e_widget_flags::visible };
		e_widget_state state{ };

		std::string name{ };
		vec2_t pos{ }, size{ };

		rect_t working_region{ };
		rect_t bounds{ vec2_t{ std::numeric_limits<float>::min() }, vec2_t{ std::numeric_limits<float>::max() } };

		node_t node{ };
		array_callbacks_t<e_widget_callbacks> callbacks{ };

	public:
		i_widget(std::string_view _name) : name{ _name } { }

	public:
		virtual bool can_hovered() {
			if(node.parent && node.parent->bounds != 0.f && !node.parent->bounds.contains(input::mouse.pos)) return false;
			return rect_t{ pos, pos + size }.contains(input::mouse.pos);
		}

		virtual bool can_draw_child(i_widget* child) { return !(bounds != 0.f && !bounds.intersects({ child->pos, child->pos + child->size })); }
		virtual bool can_handle_child(i_widget* child);

		virtual i_widget* add_widget(i_widget* widget);

		virtual void handle() {
			setup();
			draw();
		}

		virtual void setup();
		virtual void draw();

	public:
		virtual void on_child_in_node_event_handled(i_widget* child) { if(node.parent) node.parent->on_child_in_node_event_handled(child); }
		virtual void on_child_event_handled(i_widget* child) { on_child_in_node_event_handled(child); }
		virtual void on_child_focused(i_widget* child) { }
		virtual void on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) { }

		virtual void on_focused();
		virtual void on_lost_focus(i_widget* new_focused_widget);

		virtual void on_mouse_move() { callbacks.call<void()>(e_widget_callbacks::on_mouse_move); }
		virtual void on_mouse_enter();
		virtual void on_mouse_exit();

		virtual void on_mouse_key_down();
		virtual void on_mouse_key_up();

		virtual void on_mouse_wheel() { callbacks.call<void()>(e_widget_callbacks::on_mouse_wheel); }

		virtual void on_key_down(input::e_key_id key) { callbacks.call<void()>(e_widget_callbacks::on_key_down); }
		virtual void on_key_up(input::e_key_id key) { callbacks.call<void()>(e_widget_callbacks::on_key_up); }

		virtual void on_char_add(const std::uint32_t& added_char) { callbacks.call<void()>(e_widget_callbacks::on_char_add); }

		virtual bool event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param);
	};
}