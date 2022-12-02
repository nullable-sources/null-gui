#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::group {
		inline color_t<int> background{ 31, 255 };
	}

	bool begin_group(std::string_view name, vec2_t arg_size = vec2_t(0, 0));
	void end_group();
}