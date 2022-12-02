#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::scrollbar {
		inline float thickness{ 2.f };
		inline float rounding{ };
		inline vec2_t padding{ 2, 2 };
		inline bool show_background{ true };
	}

	void begin_scroll();
	void end_scroll();

	namespace impl::behaviors {
		void scroll(const rect_t& widget_size, bool& hovered, bool& pressed);
	}
}