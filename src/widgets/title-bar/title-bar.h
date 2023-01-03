#pragma once
#include <interfaces/widget/widget.h>

namespace null::gui {
	class c_title_bar : public interfaces::i_widget {
	public:
		struct style_t {
			float height{ 15 };
			float text_offset{ 10 };
			color_t<int> color{ 100, 100, 255 };
		} style{ };

	public:
		c_title_bar(const std::string_view& _name) : interfaces::i_widget{ _name } { }

	public:
		virtual void setup_self() override;
		virtual void draw() override;
	};
}