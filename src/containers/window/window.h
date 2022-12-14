#pragma once
#include <containers/container.h>

namespace null::gui {
	class c_window : public i_container {
	public:
		static inline std::vector<std::shared_ptr<c_window>> window_stack{ };

		static void add_window(c_window* window) {
			window->setup();
			window_stack.push_back(std::shared_ptr<c_window>(window));
		}

	public:
		struct style_t {
			float titlebar_height{ 15 };
			float titlebar_text_offset{ 10 };
			color_t<int> titlebar_color{ 100, 100, 255 };

			color_t<int> background_color{ 30, 30, 30 };

			vec2_t padding{ 10, 10 };

			float widget_padding{ 10.f };
		} style{ };

	public:
		vec2_t clicked_offset{ };

	public:
		c_window(std::string_view _name, vec2_t _pos, vec2_t _size) : i_container(_name) {
			pos = _pos; size = _size;
		}

		virtual bool is_topmost() { return window_stack.back().get() == this; }
		virtual void focus();

	protected:
		virtual void setup_bounds() override;

	public:
		virtual void append_auto_size() override;
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_child_focused(i_widget* child) override;
		virtual void on_focused() override;
		virtual void on_mouse_move() override;
		virtual void on_mouse_key_down() override;
	};
}