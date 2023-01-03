#pragma once
#include <interfaces/widget/widget.h>

namespace null::gui {
	class c_check_box : public interfaces::i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };
			float text_offset{ 4.f }; //@note: offset of box

			color_t<int> check_mark_color{ 100, 100, 255 };
			float check_mark_offset{ 3.f };

			vec2_t box_size{ 20.f };
		} style;

	public:
		bool* value{ };

	public:
		c_check_box(const std::string_view& _name, bool* _value) : interfaces::i_widget{ _name }, value{ _value } { }

	public:
		virtual bool can_hovered() const override;

		virtual void setup_self() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_key_up(const input::e_key_id& key) override;
	};
}