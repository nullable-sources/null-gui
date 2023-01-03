#pragma once
#include <interfaces/movable/movable.h>

namespace null::gui {
	class c_window : public interfaces::i_movable {
	public:
		static inline std::vector<std::shared_ptr<c_window>> window_stack{ };

		static void add_window(c_window* window) {
			window->setup();
			window_stack.push_back(std::shared_ptr<c_window>(window));
		}

	public:
		struct style_t {
			color_t<int> background_color{ 30, 30, 30 };

			vec2_t padding{ 10.f, 10.f };
			vec2_t spacing{ 5.f, 10.f };
		} style{ };

	public:
		c_window(const std::string_view& _name, const vec2_t& _pos, const vec2_t& _size, const e_layout_flags& _flags = { }) : interfaces::i_container{ _pos, _size }, interfaces::i_widget{ _name } {
			flags |= _flags;
			i_layout::style.spacing = 0.f;
			create_default_widgets(_flags, { style.spacing, style.padding });
		}

	public:
		virtual bool is_topmost() const { return window_stack.back().get() == this; }
		virtual void focus();

	public:
		virtual void setup_self() override;
		virtual void draw() override;
	
	public:
		virtual void on_child_focused(i_widget* child) override;
		virtual void on_focused() override;
	};
}