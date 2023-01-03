#pragma once
#include <interfaces/popup/popup.h>

namespace null::gui {
	class c_popup : public interfaces::i_popup {
	public:
		struct style_t {
			color_t<int> background_color{ 10, 10, 10 };

			vec2_t padding{ 10, 10 };
			vec2_t spacing{ 5.f, 10.f };
		} style{ };

	public:
		c_popup(const std::string_view& _name, const e_layout_flags& _flags = { }) : interfaces::i_widget{ _name }, interfaces::i_popup{ } {
			flags |= _flags;
			i_layout::style.spacing = 0.f;
			create_default_widgets(_flags, { style.spacing, style.padding });
		}

	public:
		virtual void draw() override;// { }
	};
}