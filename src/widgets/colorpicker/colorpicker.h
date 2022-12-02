#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::colorpicker {
		inline float sv_box_size{ 100.f };
		inline float sliders_thickness{ 9.f };
		inline float rounding{ 5.f };
	}

	void colorpicker_sv_box(std::string_view name, hsv_color_t& color);
	void colorpicker_h_slider(std::string_view name, hsv_color_t& color);
	void colorpicker_alpha_slider(std::string_view name, hsv_color_t& color);
	void colorpicker(std::string_view name, color_t<int>* clr, bool alpha_bar = true);

	namespace impl::behaviors {
		bool colorpicker(std::string_view widget_name, std::string_view picker_name, std::string_view tooltip_name, const rect_t& widget_size);
		bool colorpicker_slider(std::string_view widget_name, const rect_t& widget_rect);
	}
}