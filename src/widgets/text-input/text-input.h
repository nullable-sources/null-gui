#pragma once
#include <widgets/widget.h>

namespace null::gui {
	class c_text_input : public i_widget {
	public:
		struct style_t {
			color_t<int> default_color{ 50, 50, 50 };
			color_t<int> hovered_color{ 60, 60, 60 };
			color_t<int> active_color{ 60, 60, 60 };

			color_t<int> text_color{ 255, 255, 255 };

			color_t<int> selection_rect{ 100, 100, 255, 120 };

			float field_offset{ 3.f }; //@note: offset for textinput field at widget name
			vec2_t input_text_offset{ 4, 2 }; //@note: offset in input bar for text

			std::chrono::milliseconds write_pos_show_duration{ 600 };
			std::chrono::milliseconds write_pos_hide_duration{ 600 };
		} style;

	public:
		struct write_data_t {
		public:
			bool show{ };
			int offset{ };

			std::chrono::steady_clock::time_point next_change_time{ };

		public:
			void update(const std::chrono::milliseconds& change_time_offset, bool _show = true) { show = _show; next_change_time = std::chrono::steady_clock::now() + change_time_offset; }
		};

		struct selection_data_t {
		public:
			int start{ }, end{ };

		public:
			int min() { return std::min(start, end); }
			int max() { return std::max(start, end); }

			void set(int value) { start = end = value; }
			bool selecting() { return start != end; }
			int remove_selected(std::wstring& str) { str.erase(std::next(str.begin(), min()), std::next(str.begin(), max())); return min(); }
		};

	public:
		rect_t bar_region{ };
		rect_t field_region{ };

		std::string* output_buffer{ };
		std::wstring utf8_buffer{ };

		write_data_t write_data{ };
		selection_data_t selection_data{ };

	public:
		c_text_input(std::string_view _name, std::string* _output_buffer) : i_widget(_name), output_buffer(_output_buffer), write_data{ false, 0, std::chrono::steady_clock::now() } { utf8_buffer = utils::encoding::ascii_to_utf8(*output_buffer); write_data.offset = utf8_buffer.length(); }

	private:
		virtual bool filter(const std::uint32_t& symbol);

		virtual int get_hovered_char();

	public:
		virtual bool can_hovered() override;

		virtual void setup() override;
		virtual void draw() override;

	public:
		virtual void on_mouse_move() override;

		virtual void on_mouse_key_down() override;
		virtual void on_mouse_key_up() override { callbacks.call<void()>(e_widget_callbacks::on_mouse_key_up); }

		virtual void on_key_down(input::e_key_id key) override;

		virtual void on_char_add(const std::uint32_t& added_char) override;
	};
}