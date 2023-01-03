#pragma once
#include <iostream>
#include <map>

namespace null::gui {
	enum class e_widget_flags {
		none,
		visible = 1 << 0,
		ignore_auto_positioning = 1 << 1,
		ignore_scroll_offset = 1 << 2,
		on_top_layer = 1 << 3
	}; enum_create_bit_operators(e_widget_flags, true);
	enum_create_cast_operator(e_widget_flags, -);

	enum class e_widget_event {
		mouse_move,
		mouse_key_down,
		mouse_key_up,
		mouse_wheel,

		key_down,
		key_up,
		char_add
	};

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

		can_handle_child,
	};
	namespace interfaces { class i_widget; }
	using widget_callbacks_t = utils::callbacks_tuple_t<
		utils::callbacks_t<e_widget_callbacks::on_focused, void()>,
		utils::callbacks_t<e_widget_callbacks::on_lost_focus, void(interfaces::i_widget*)>,

		utils::callbacks_t<e_widget_callbacks::on_mouse_move, void()>,
		utils::callbacks_t<e_widget_callbacks::on_mouse_enter, void()>,
		utils::callbacks_t<e_widget_callbacks::on_mouse_exit, void()>,
		utils::callbacks_t<e_widget_callbacks::on_mouse_key_down, void(const input::e_key_id&)>,
		utils::callbacks_t<e_widget_callbacks::on_mouse_key_up, void(const input::e_key_id&)>,
		utils::callbacks_t<e_widget_callbacks::on_mouse_wheel, void()>,

		utils::callbacks_t<e_widget_callbacks::on_key_down, void(const input::e_key_id&)>,
		utils::callbacks_t<e_widget_callbacks::on_key_up, void(const input::e_key_id&)>,
		utils::callbacks_t<e_widget_callbacks::on_char_add, void(const std::uint32_t&)>,

		utils::callbacks_t<e_widget_callbacks::can_handle_child, bool(interfaces::i_widget*)>
	>;

	enum class e_widget_state {
		none,
		active = 1 << 0,
		hovered = 1 << 1,
		focused = 1 << 2
	}; enum_create_bit_operators(e_widget_state, true);
	enum_create_cast_operator(e_widget_state, -);

	namespace interfaces {
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

				std::vector<i_widget*> parent_node() const;
				std::vector<i_widget*> child_node() const;
			};

		public:
			e_widget_flags flags{ e_widget_flags::visible };
			e_widget_state state{ };

			std::string name{ };
			vec2_t pos{ }, size{ };

			rect_t working_region{ };
			rect_t bounds{ vec2_t{ std::numeric_limits<float>::min() }, vec2_t{ std::numeric_limits<float>::max() } }; //@note: offset relative to pos, same as working_region

			node_t node{ };
			widget_callbacks_t callbacks{ };

		public:
			i_widget() { }
			i_widget(const std::string_view& _name) : name{ _name } { }
			virtual ~i_widget() { }

		public:
			virtual i_widget* add_widget(i_widget* widget);

			virtual void handle() { setup(); draw(); }

			virtual void setup_self() { }
			virtual void setup_childs();
			virtual void setup_child(i_widget* child) { child->setup(); }
			virtual void setup_bounds() { }
			virtual void setup();
			virtual void draw();

		public:
			virtual rect_t get_real_bounds() const { return bounds + pos; }

			virtual bool can_hovered() const {
				if(node.parent && node.parent->bounds != 0.f && !node.parent->get_real_bounds().contains(input::mouse.pos)) return false;
				return rect_t{ pos, pos + size }.contains(input::mouse.pos);
			}

			virtual bool can_draw_child(const std::shared_ptr<i_widget>& child) const {
				return !(bounds != 0.f && !get_real_bounds().intersects({ child->pos, child->pos + child->size }));
			}

			virtual bool can_handle_child(const std::shared_ptr<i_widget>& child) {
				if(!(child->flags & e_widget_flags::visible)) return false;
				return callbacks.at<e_widget_callbacks::can_handle_child>().empty() ? true : callbacks.at<e_widget_callbacks::can_handle_child>().call(child.get()).front();
			}

		public:
			virtual void on_child_in_node_event_handled(const e_widget_event& event, i_widget* child) { if(node.parent) node.parent->on_child_in_node_event_handled(event, child); }
			virtual void on_child_event_handled(const e_widget_event& event, i_widget* child) { on_child_in_node_event_handled(event, child); }
			virtual void on_child_focused(i_widget* child) { }
			virtual void on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) { }

			virtual void on_focused();
			virtual void on_lost_focus(i_widget* new_focused_widget);

			virtual void on_mouse_move() { callbacks.at<e_widget_callbacks::on_mouse_move>().call(); }
			virtual void on_mouse_enter();
			virtual void on_mouse_exit();

			virtual void on_mouse_key_down(const input::e_key_id& key);
			virtual void on_mouse_key_up(const input::e_key_id& key);

			virtual void on_mouse_wheel() { callbacks.at<e_widget_callbacks::on_mouse_wheel>().call(); }

			virtual void on_key_down(const input::e_key_id& key) { callbacks.at<e_widget_callbacks::on_key_down>().call(key); }
			virtual void on_key_up(const input::e_key_id& key) { callbacks.at<e_widget_callbacks::on_key_up>().call(key); }

			virtual void on_char_add(const std::uint32_t& added_char) { callbacks.at<e_widget_callbacks::on_char_add>().call(added_char); }

			virtual bool handling_child_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param);
			virtual bool handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param);
			virtual bool event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param);
		};
	}
}