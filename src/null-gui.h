#pragma once
#include <null-input.h>

#include <widgets/button/button.h>
#include <widgets/checkbox/checkbox.h>
#include <widgets/clickable-text/clickable-text.h>
#include <widgets/colorpicker/colorpicker.h>
#include <widgets/column/column.h>
#include <widgets/combo/combo.h>
#include <widgets/group/group.h>
#include <widgets/multicombo/multicombo.h>
#include <widgets/window/window.h>
#include <widgets/text-input/text-input.h>
#include <widgets/text/text.h>
#include <widgets/slider/slider.h>
#include <widgets/selectable/selectable.h>
#include <widgets/scrollbar/scrollbar.h>

namespace null {
	namespace gui {
		namespace style {
			inline color_t<int> main_color = color_t<int>(83, 100, 255, 255);
			inline color_t<int> text = color_t<int>(255, 255, 255, 255);
			inline color_t<int> text_hovered = color_t<int>(130, 130, 130, 255);

			inline float text_spacing{ 4.f };
			inline float new_line_size{ 16.f };

			inline bool items_size_full_window{ true };
		}

		namespace impl {
			inline std::string active_widget{ }, hovered_widget{ }, last_widget{ };

			enum class e_data_type {
				integer,
				floating_point,
				ascii_string
			};

			template <typename var_t>
			struct pushed_var_t {
			public:
				var_t* var{ }, old_value{ };

			public:
				pushed_var_t(var_t* _var, var_t _new_value) : var{ _var }, old_value{ *_var } { *var = _new_value; }
				~pushed_var_t() { *var = old_value; }
			}; static inline std::vector<std::shared_ptr<void>> pushed_vars{ };

			template <typename var_t>
			static void push_var(var_t* var, var_t new_value) { pushed_vars.push_back(std::make_shared<pushed_var_t<var_t>>(var, new_value)); }
			static void pop_var() { pushed_vars.pop_back(); }

			std::string_view format_widget_name(std::string_view text);
		}

		void begin_frame();
		void end_frame();

		void tooltip(const std::function<void()>& func);
		static void tooltip(std::string_view tooltip_text) { tooltip([&]() { text(tooltip_text); }); }

		void new_line();
		void same_line();

		int wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	}
}