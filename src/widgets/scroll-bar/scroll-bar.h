#pragma once
#include <widgets/widget.h>

namespace null::gui {
	class c_scroll_bar : public i_widget {
	public:
		struct style_t {
			color_t<int> background_default_color{ 50, 50, 50 };
			color_t<int> background_hovered_color{ 60, 60, 60 };
			color_t<int> background_active_color{ 60, 60, 60 };

			color_t<int> slider_default_color{ 100, 100, 255 };
			color_t<int> slider_hovered_color{ 120, 120, 255 };
			color_t<int> slider_active_color{ slider_hovered_color };

			float wheel_modifyer{ 10.f };

			vec2_t container_padding{ 5.f, 0.f };
			float container_working_rect_padding{ 0.f };
			float bar_size{ 5.f };
			float slider_size{ 3.f };
		} style{ };

	private:
		float scroll_value{ };

	public:
		float max_scroll_value{ };

		float pixel_per_scroll{ }; //@note: pixels in container per pixel in scroll bar

		struct {
			vec2_t pos{ }, size{ };
			float moving_offset{ };
		} slider_data{ };

	public:
		c_scroll_bar() : i_widget{ "scroll bar" } { flags |= e_widget_flags::ignore_auto_positioning | e_widget_flags::on_top_layer; }

	public:
		virtual float get_scroll_value() { return std::min(scroll_value, std::max(max_scroll_value, 0.f)); }

	public:
		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_move() override;

		virtual void on_mouse_key_down(const input::e_key_id& key) override;
		virtual void on_mouse_key_up(const input::e_key_id& key) override;

		virtual void on_mouse_wheel() override;

		virtual bool handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override;
	};
}