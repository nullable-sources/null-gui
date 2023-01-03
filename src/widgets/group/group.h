#pragma once
#include <interfaces/container/container.h>

namespace null::gui {
	class c_group : public interfaces::i_container {
	public:
		struct style_t {
			color_t<int> background_color{ 40, 40, 40 };

			vec2_t padding{ 10.f, 10.f };
			vec2_t spacing{ 5.f, 10.f };
		} style{ };

	public:
		c_group(const std::string_view& _name, const vec2_t& _size = -1) : interfaces::i_widget{ _name } {
			i_layout::style.spacing = 0.f;

			size = _size;
			if(size.x <= 0) flags |= e_layout_flags::auto_size_x;
			if(size.y <= 0) flags |= e_layout_flags::auto_size_y;

			create_default_widgets(size.y <= 0 ? e_layout_flags::auto_size_y : e_layout_flags{ }, { style.spacing, style.padding });
		}

	public:
		virtual void setup_self() override;
		virtual void draw() override;
	};
}