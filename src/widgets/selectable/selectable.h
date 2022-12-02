#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::selectable {
		inline float offset{ 4.f };
		inline float active_offset{ 10.f };
	}

	bool selectable(std::string_view name, bool selected);
}