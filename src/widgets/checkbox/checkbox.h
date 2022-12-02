#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::checkbox {
		inline float box_size{ 16.f };
		inline float check_mark_size{ 4.f };
		inline float body_offset{ 2.f };
		inline float rounding{ 0.f };
		inline bool hovered_with_text{ };
	}

	void checkbox(std::string_view name, bool* value);
}