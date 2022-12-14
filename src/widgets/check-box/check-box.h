#pragma once
#include <widgets/widget.h>

namespace null::gui {
	class c_check_box : public i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };
			float text_offset{ 4.f }; //@note: offset of box

			color_t<int> check_mark_color{ 100, 100, 255 };
			float check_mark_offset{ 3.f };
		} style;

	public:
		rect_t box_region{ };
		bool* value{ };

	public:
		c_check_box(std::string_view _name, bool* _value) : i_widget(_name), value(_value) { }

	public:
		virtual bool can_hovered() override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_key_up() override;
	};
}