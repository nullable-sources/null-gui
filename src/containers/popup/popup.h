#pragma once
#include <containers/container.h>

namespace null::gui {
	class c_popup : public i_container {
	public:
		struct style_t {
			color_t<int> titlebar_color{ 255, 100, 100 };
			float titlebar_height{ 20.f };

			color_t<int> background_color{ 10, 10, 10 };

			vec2_t padding{ 10, 10 };

			float widget_padding{ 10.f };
		} style{ };

	public:
		c_popup(const std::string_view& _name) : i_container{ _name } {
			scroll_bar = (c_scroll_bar*)add_widget(new c_scroll_bar{ });
			flags &= ~e_widget_flags::visible;
		}

		virtual bool can_close_on_lost_focus(i_widget* new_focused_widget);

	public:
		virtual void open() override;
		virtual void close() override;

		virtual void append_auto_size() override;
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) override;
		
		virtual void on_focused() override;
		virtual void on_lost_focus(i_widget* new_focused_widget) override;
	};
}