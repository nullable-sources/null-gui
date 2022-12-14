#pragma once
#include <containers/container.h>

namespace null::gui {
	class c_group : public i_container {
	public:
		struct style_t {
			float titlebar_height{ 15 };
			float titlebar_text_offset{ 10 };
			color_t<int> titlebar_color{ 100, 100, 255 };

			color_t<int> background_color{ 40, 40, 40 };

			vec2_t padding{ 10, 10 };

			float widget_padding{ 10.f };
		} style{ };

	public:
		c_group(std::string_view _name, vec2_t _size = -1) : i_container(_name) {
			size = _size;
			if(size.x <= 0) container_flags |= e_container_flags::auto_size_x;
			if(size.y <= 0) container_flags |= e_container_flags::auto_size_y;
		}

	protected:
		virtual void setup_bounds() override;

	public:
		virtual void append_auto_size() override;
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;
		virtual void draw() override;
	};
}