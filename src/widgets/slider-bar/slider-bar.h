#pragma once
#include <interfaces/widget/widget.h>

namespace null::gui {
	class c_slider_bar : public interfaces::i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };
			color_t<int> value_color{ 255, 255, 255 };

			color_t<int> bar_color{ 100, 100, 255 };

			float bar_offset{ 3.f };
			float bar_height{ 10.f };
		} style{ };

	public:
		float pixels_per_value{ };

		float min_value{ }, max_value{ };
		float* value{ };

		std::string format{ "{:.0f}" };

	public:
		c_slider_bar(const std::string_view& _name, float* _value, const float& _min_value, const float& _max_value) : interfaces::i_widget{ _name }, value{ _value }, min_value{ std::min(_min_value, _max_value) }, max_value{ std::max(_min_value, _max_value) } { }

	public:
		virtual bool can_hovered() const override;

		virtual void setup_self() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_move() override;
	};
}