#pragma once
#include <widgets/widget.h>

namespace null::gui {
	class i_selectable : public i_widget {
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
		i_selectable(const std::string_view& _name, const int& _id) : i_widget{ _name }, id{ _id } { }

	public:
		virtual bool selected() { return false; }
		virtual void select() { }

	public:
		void draw() override;
		void setup() override;

		void on_mouse_key_down(const input::e_key_id& key) override { select(); i_widget::on_mouse_key_down(key); }
	};

	class c_single_selectable : public i_selectable {
	public:
		int* value{ };

	public:
		c_single_selectable(const std::string_view& _name, int* _value, const int& _id) : i_selectable{ _name, _id }, value{ _value } { }

	public:
		bool selected() override { return *value == id; }
		void select() override { *value = id; }
	};

	class c_multi_selectable : public i_selectable {
	public:
		bool* value{ };

	public:
		c_multi_selectable(const std::string_view& _name, bool* _value, const int& _id) : i_selectable{ _name, _id }, value{ _value } { }

	public:
		bool selected() override { return *value; }
		void select() override { *value = !*value; }
	};
}