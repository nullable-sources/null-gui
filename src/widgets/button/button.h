#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::button {
		inline color_t<int> box{ 44, 44, 44, 255 };
		inline color_t<int> box_hovered{ 46, 255 };
		inline color_t<int> box_active{ 48, 255 };
		inline float rounding{ };
		inline vec2_t padding{ 5, 1 };
	}

	bool button(std::string_view name, const vec2_t& size = { });

	namespace impl::behaviors {
		bool button(std::string_view name, const rect_t& widget_rect, bool& hovered, bool& pressed);
	}
}