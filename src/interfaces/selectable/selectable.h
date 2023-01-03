#pragma once
#include <interfaces/widget/widget.h>

namespace null::gui::interfaces {
	class i_selectable : public virtual i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 65, 65, 65 };
			color_t<int> active_color{ 100, 100, 255 };
			color_t<int> selected_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };

			float text_offset{ 2.f }, selected_text_offset{ 5.f };
			float height_padding{ 2.f };
		} style{ };

	public:
		int id{ };

	public:
		i_selectable(const int& _id) : id{ _id } { }

	public:
		virtual bool selected() const { return false; }
		virtual void select() { }

	public:
		void setup_self() override;
		void draw() override;

		void on_mouse_key_down(const input::e_key_id& key) override { select(); i_widget::on_mouse_key_down(key); }
	};
}