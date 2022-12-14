#pragma once
#include <widgets/widget.h>
#include <widgets/selectable/selectable.h>
#include <containers/container.h>

namespace null::gui {
	class c_list_box : public i_container {
	public:
		struct style_t {
			float titlebar_height{ 15 };
			float titlebar_text_offset{ 10 };
			color_t<int> titlebar_color{ 100, 100, 255 };

			color_t<int> background_color{ 45, 45, 45 };

			vec2_t scrollbar_padding{ 5.f, 10.f };

			int max_show_items{ 5 };
		} style{ };

	private:
		float clamped_size{ };

	public:
		c_list_box(std::string_view _name, int* value, const std::vector<std::string>& items, vec2_t _size = -1) : c_list_box(_name, _size) { build(value, items); }
		c_list_box(std::string_view _name, std::vector<std::pair<std::string, bool>>* items, vec2_t _size = -1) : c_list_box(_name, _size) { build(items); }
		c_list_box(std::string_view _name, vec2_t _size = -1) : i_container(_name) {
			size = _size;
			if(size.x <= 0) container_flags |= e_container_flags::auto_size_x;
			if(size.y <= 0) container_flags |= e_container_flags::auto_size_y;
			scroll_bar.style.container_padding.y = style.scrollbar_padding.y;
		}


	public:
		void build(int* value, const std::vector<std::string>& items) {
			std::ranges::for_each(std::views::iota(0, (int)items.size()), [&](const int& item_id) {
				add_widget(new c_single_selectable{ items[item_id], value, item_id });
				});
		}

		void build(std::vector<std::pair<std::string, bool>>* items) {
			std::ranges::for_each(std::views::iota(0, (int)items->size()), [&](const int& item_id) {
				add_widget(new c_multi_selectable{ (*items)[item_id].first, &(*items)[item_id].second, item_id });
				});
		}

	protected:
		virtual void setup_bounds() override;

		virtual void setup_scroll_bar() override;

	public:
		virtual void append_auto_size() override;
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;
		virtual void draw() override;
	};
}