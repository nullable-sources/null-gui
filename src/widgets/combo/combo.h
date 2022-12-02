#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::combo {
		inline float box_size{ 16.f };
		inline float arrow_size{ 5.f };
		inline float popup_padding{ 0.f };
		inline int autosize_items{ 5 };
		inline float rounding{ 0.f };
	}

	void combo(std::string_view name, int* value, const std::vector<std::string>& items);

	namespace impl::behaviors {
		bool combo(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed);
	}
}