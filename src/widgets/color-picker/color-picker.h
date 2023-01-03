#pragma once
#include <widgets/popup/popup.h>

namespace null::gui {
	class c_color_picker : public interfaces::i_widget {
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
		class c_sv_box : public interfaces::i_widget {
		public:
			struct style_t {
				float size{ 100.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_sv_box(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, interfaces::i_widget{ std::string{ parent_name}.append(" sv box") } { }

		public:
			virtual void setup_self() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

		class c_h_bar : public interfaces::i_widget {
		public:
			struct style_t {
				float thickness{ 10.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_h_bar(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, interfaces::i_widget{ std::string{ parent_name }.append(" h bar") } { }

		public:
			virtual void setup_self() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

		class c_alpha_bar : public interfaces::i_widget {
		public:
			struct style_t {
				float thickness{ 10.f };
			} style{ };

		public:
			hsv_color_t* hsv{ };

		public:
			c_alpha_bar(const std::string_view& parent_name, hsv_color_t* _hsv) : hsv{ _hsv }, interfaces::i_widget{ std::string{ parent_name }.append(" alpha bar") } { }

		public:
			virtual void setup_self() override;
			virtual void draw() override;

		public:
			virtual void on_mouse_move() override;
		};

	public:
		c_popup* popup{ };

		c_sv_box* sv_box{ };
		c_h_bar* h_bar{ };
		c_alpha_bar* alpha_bar{ };

		hsv_color_t hsv{ };
		color_t<int>* color{ };

	public:
		c_color_picker(const std::string_view& _name, color_t<int>* _color, const bool& withour_alpha_bar = false) : interfaces::i_widget{ _name }, color{ _color }, hsv{ *_color } {
			popup = new c_popup{ std::string{ name }.append(" picker popup"), e_layout_flags::auto_size }; {
				auto layout{ new c_layout{ e_layout_flags::auto_size, rect_t::left, rect_t::right } }; {
					sv_box = (c_sv_box*)layout->add_widget(new c_sv_box{ name, &hsv });
					h_bar = (c_h_bar*)layout->add_widget(new c_h_bar{ name, &hsv });
				} popup->add_child(layout);
				alpha_bar = (c_alpha_bar*)popup->add_child(new c_alpha_bar{ name, &hsv });
				if(withour_alpha_bar) alpha_bar->flags &= ~e_widget_flags::visible;
			} add_widget(popup);
		}

	public:
		virtual bool can_hovered() const override;

		virtual void setup_self() override;
		virtual void draw() override;

	public:
		virtual void on_child_in_node_event_handled(const e_widget_event& event, i_widget* child) override;
		virtual void on_mouse_key_up(const input::e_key_id& key) override;
	};
}