#pragma once
#include <widgets/widget.h>

namespace null::gui {
	class c_button : public i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 100, 100, 255 };

			color_t<int> text_color{ 255, 255, 255 };
		} style;

	public:
		c_button(const std::string_view& _name) : i_widget{ _name } { }

	public:
		virtual void setup() override;
		virtual void draw() override;
	};
}