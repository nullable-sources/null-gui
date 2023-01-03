#pragma once
#include <interfaces/list-box/list-box.h>
#include <widgets/selectable/selectable.h>

namespace null::gui {
	class c_list_box : public interfaces::i_list_box {
	public:
		struct style_t {
			color_t<int> background_color{ 45, 45, 45 };

			vec2_t scroll_bar_padding{ 5.f, 10.f };
		} style{ };

	private:
		float clamped_size{ };

	public:
		c_list_box(const std::string_view& _name, int* value, const std::vector<std::string>& items, const vec2_t& _size = -1) : c_list_box{ _name, _size } { build(value, items); }
		c_list_box(const std::string_view& _name, std::vector<std::pair<std::string, bool>>* items, const vec2_t& _size = -1) : c_list_box{ _name, _size } { build(items); }
		c_list_box(const std::string_view& _name, const vec2_t& _size = -1) : interfaces::i_widget{ _name } {
			i_layout::style.spacing = 0.f;
			
			size = _size;
			if(size.x <= 0) flags |= e_layout_flags::auto_size_x;
			if(size.y <= 0) flags |= e_layout_flags::auto_size_y;

			create_default_widgets(size.y <= 0 ? e_layout_flags::auto_size_y : e_layout_flags{ }, { style.scroll_bar_padding, { 0.f } });
		}

	public:
		virtual void setup_self() override;
		virtual void draw() override;
	};
}