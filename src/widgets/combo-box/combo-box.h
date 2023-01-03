#pragma once
#include <interfaces/popup/popup.h>
#include <interfaces/list-box/list-box.h>
#include <widgets/selectable/selectable.h>

namespace null::gui {
	class c_combo_box : public interfaces::i_widget {
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
		class c_list_popup : public interfaces::i_popup, public interfaces::i_list_box {
		public:
			struct style_t {
				float offset_from_box{ 4.f };
				color_t<int> background_color{ 45, 45, 45 };

				vec2_t scroll_bar_padding{ 5.f, 10.f };
			} style{ };
			
		public:
			float clamped_size{ };

		public:
			c_list_popup() : interfaces::i_popup{ }, interfaces::i_widget{ "combo popup" } {
				flags |= e_layout_flags::auto_size_y;
				i_layout::style.spacing = 0.f;
				create_default_widgets(e_layout_flags::auto_size_y, { style.scroll_bar_padding, { 0.f } });
			}

		protected:
			virtual void create_default_titlebar() override { }

		public:
			virtual void setup_self() override;
			virtual void draw() override;
		};

	public:
		c_list_popup* popup{ };

	public:
		c_combo_box(const std::string_view& _name, int* value, const std::vector<std::string>& items) : c_combo_box{ _name } { popup->build(value, items); }
		c_combo_box(const std::string_view& _name, std::vector<std::pair<std::string, bool>>* multi_items) : c_combo_box{ _name } { popup->build(multi_items); }
		c_combo_box(const std::string_view& _name) : interfaces::i_widget{ _name } {
			popup = dynamic_cast<c_list_popup*>(add_widget(new c_list_popup));
		}

	public:
		std::string selected_items();

	public:
		virtual bool can_hovered() const override;

		virtual void setup_self() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_key_up(const input::e_key_id& key) override;
	};
}