#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::text_input {
		inline color_t<int> select_text{ 100, 100, 255, 100 };
		inline float line_size{ 2.f };
		inline float rounding{ };
		inline std::chrono::milliseconds write_offset_show{ 400 };
		inline std::chrono::milliseconds write_offset_hide{ 400 };
		inline vec2_t padding{ 3.f, 0.f };
	}

	bool text_input(std::string_view name, std::string* value, bool password = false);
	bool text_input(std::string_view name, int* value);
	bool text_input(std::string_view name, float* value);

	namespace impl {
		enum class e_data_type;
		class c_text_input {
		public:
			static inline std::vector<c_text_input> text_inputs{ };

			static c_text_input* find(std::string_view name);
			static c_text_input* create(const c_text_input& text_input);

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
			std::string name{ };
			void* data{ };
			rect_t working_rect{ };
			e_data_type data_type{ };

			std::string ascii_buffer{ };
			std::wstring utf8_buffer{ };

			write_data_t write_data{ };
			selection_data_t selection_data{ };

		public:
			void key_down(input::e_key_id key);
			void add_char(const std::uint32_t& symbol);

			int get_hovered_char(std::wstring_view buffer) const;
			bool filter(const std::uint32_t& symbol) const;

			void update_buffers();
			std::string data_to_string() const;
			void set_data_from_buffer();
		};

		namespace behaviors {
			bool text_input(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed);
		}
	}

	bool text_input(std::string_view name, void* value, bool password, impl::e_data_type data_type);
}