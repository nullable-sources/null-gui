#pragma once
#include <containers/popup/popup.h>
#include <widgets/selectable/selectable.h>
#include <widgets/widget.h>

namespace null::gui {
	class c_combo_box : public i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };

			float bar_offset{ 3.f };
			vec2_t selected_name_offset{ 4.f, 2.f };
		} style{ };

	public:
		class c_list_popup : public c_popup {
		public:
			struct style_t {
				float offset_from_box{ 4.f };
				color_t<int> background_color{ 45, 45, 45 };

				vec2_t scroll_bar_padding{ 5.f, 10.f };

				int max_show_items{ 5 };
			} style{ };
			
		public:
			float clamped_size{ };

		public:
			c_list_popup() : c_popup{ "combo popup" } {
				scroll_bar = (c_scroll_bar*)add_widget(new c_scroll_bar{ });
				scroll_bar->style.container_working_rect_padding = style.scroll_bar_padding.x;
				scroll_bar->style.container_padding.y = style.scroll_bar_padding.y;
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

		public:
			virtual void setup_bounds() override;
			virtual void append_auto_size() override;
			void append_auto_positioning(i_widget* widget) override;

			virtual void setup() override;

			virtual void draw() override;
		};

	public:
		c_list_popup* popup{ };

		rect_t bar_region{ };

	public:
		c_combo_box(const std::string_view& _name, int* value, const std::vector<std::string>& items) : c_combo_box{ _name } { popup->build(value, items); }
		c_combo_box(const std::string_view& _name, std::vector<std::pair<std::string, bool>>* multi_items) : c_combo_box{ _name } { popup->build(multi_items); }
		c_combo_box(const std::string_view& _name) : i_widget{ _name } {
			//@note: or
			//	popup = new c_list_popup{ };
			//  add_widget(popup);
			popup = (c_list_popup*)add_widget(new c_list_popup);
		}

	public:
		std::string selected_items();

	public:
		virtual bool can_hovered() override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_key_up(const input::e_key_id& key) override;
	};
}