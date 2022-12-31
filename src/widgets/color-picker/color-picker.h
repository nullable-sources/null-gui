#pragma once
#include <containers/popup/popup.h>
#include <containers/columns/columns.h>
#include <widgets/widget.h>

namespace null::gui {
	class c_color_picker : public i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };
			float text_offset{ 4.f }; //@note: offset of box

			vec2_t picker_popup_offset{ 10.f, 0.f };
			vec2_t box_size{ 40.f, 20.f };
		} style{ };

	public:
		class c_sv_box : public i_widget {
		public:
			struct style_t {
				float size{ 100.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_sv_box(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, i_widget{ std::string{ parent_name}.append(" sv box") } { }

		public:
			virtual void setup() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

		class c_h_bar : public i_widget {
		public:
			struct style_t {
				float thickness{ 10.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_h_bar(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, i_widget{ std::string{ parent_name}.append(" h bar") } { }

		public:
			virtual void setup() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

		class c_alpha_bar : public i_widget {
		public:
			struct style_t {
				float thickness{ 10.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_alpha_bar(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, i_widget { std::string{ parent_name }.append(" alpha bar") } { }

		public:
			virtual void setup() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

	public:
		c_popup* popup{ };

		c_columns* columns{ };

		c_sv_box* sv_box{ };
		c_h_bar* h_bar{ };
		c_alpha_bar* alpha_bar{ };

		rect_t box_region{ };

		hsv_color_t hsv{ };
		color_t<int>* color{ };

	public:
		c_color_picker(const std::string_view& _name, color_t<int>* _color, const bool& withour_alpha_bar = false) : i_widget{ _name }, color{ _color }, hsv{ *_color } {
			popup = new c_popup{ std::string{ name }.append(" picker popup") }; {
				popup->flags |= e_container_flags::auto_size;
			} {
				columns = new c_columns{ 2, false }; {
					sv_box = (c_sv_box*)columns->at(0)->add_widget(new c_sv_box{ name, &hsv });
					h_bar = (c_h_bar*)columns->at(1)->add_widget(new c_h_bar{ name, &hsv });
				} popup->add_widget(columns);
				alpha_bar = (c_alpha_bar*)popup->add_widget(new c_alpha_bar{ name, &hsv });
				if(withour_alpha_bar) alpha_bar->flags &= ~e_widget_flags::visible;
			} add_widget(popup);
		}

	public:
		virtual bool can_hovered() override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_child_in_node_event_handled(i_widget* child) override;
		virtual void on_mouse_key_up(const input::e_key_id& key) override;
	};
}