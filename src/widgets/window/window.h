#pragma once
#include <null-render.h>

namespace null::gui {
	namespace style {
		namespace window {
			namespace titlebar {
				inline color_t<int> background{ 40, 255 };
				inline float size{ 20.f };
				inline float line_size{ 2.f };
				inline float rounding{ };
			}

			inline color_t<int> background{ 26, 255 };
			inline float item_spacing{ 5.f };
			inline float rounding{ };
			inline vec2_t padding{ 10 };
			inline bool clamp_on_screen{ true };
			inline bool move_on_titlebar{ true };
		}

		namespace popup {
			inline color_t<int> background{ 33, 255 };
		}
	}

	enum class e_window_flags {
		none = 0,
		group = 1 << 1,
		no_title_bar = 1 << 2,
		no_title_line = 1 << 3,
		popup = 1 << 4,
		no_move = 1 << 5,
		set_size = 1 << 6,
		set_pos = 1 << 7,
		auto_size = 1 << 8,
		no_padding = 1 << 9,
		ignore_scroll = 1 << 10,

		child_mask = group | popup
	}; enum_create_bit_operators(e_window_flags, true);
	enum_create_cast_operator(e_window_flags, -);

	class c_window {
	public:
		static inline c_window* current{ }, * hovered{ }, * active{ };
		static inline std::vector<std::shared_ptr<c_window>> windows;

		static c_window* find(std::string_view name);
		static c_window* create(const c_window& window);

		static void windows_handle();

	public:
		struct autopositioning_t {
		public:
			vec2_t current_pos{ }, last_pos{ }, prev_pos{ };

		public:
			void back_to_last_pos() { current_pos = last_pos; }
			void set_last_pos() { last_pos = current_pos; }
		};

	public:
		std::string name{ };
		vec2_t pos{ }, size{ }, arg_size{ }, max_size{ };

		e_window_flags flags{ };

		vec2_t moving_offset{ std::numeric_limits<float>::min() };

		autopositioning_t autopositioning{ };

		bool in_same_line{ };

		bool in_column{ };
		float column_offset{ };

		struct scrollbar_data_t {
		public:
			float max_scroll{ }, target{ }, offset{ };
			float pressed_mouse_offset{ };
		} scrollbar_data{ };

		bool visible{ true };
		bool close_call{ };
		bool dont_add_item{ };

		c_window* parent{ };
		std::vector<c_window*> child_popups;
		std::vector<std::shared_ptr<c_window>> child_groups;

		std::shared_ptr<render::c_draw_list> draw_list;

	public:
		c_window(std::string_view _name, vec2_t _pos, vec2_t _size, e_window_flags wnd_flags);

	public:
		c_window* in_popup_region();
		c_window* get_hovered_group();
		c_window* first_parent();
		c_window* find_group(std::string_view name);
		void focus();
		void clamp_on_screen();

		vec2_t get_current_widget_pos() { return autopositioning.current_pos + (!scrollbar_data.max_scroll || flags & e_window_flags::ignore_scroll ? 0.f : vec2_t{ 0.f, scrollbar_data.offset }); }
		vec2_t get_padding() { return flags & e_window_flags::no_padding ? vec2_t{ 0.f } : style::window::padding; }
		float get_scroll_thickness();
		vec2_t get_window_size_with_padding();
		float get_title_size();
		bool can_open_tooltip();
		bool can_use_widget(std::string_view name, const rect_t& rect);
		bool can_draw_widget(const rect_t& rect);

		void close();
		void add_widget(std::string_view name, const vec2_t& size);
	};

	bool begin_window(std::string_view name, vec2_t pos, vec2_t size, bool* open = nullptr);
	bool begin_window(std::string_view name, vec2_t pos, vec2_t size, e_window_flags flags, bool* open = nullptr);
	void end_window();
}