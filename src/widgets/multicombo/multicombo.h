#pragma once
#include <null-sdk.h>

namespace null::gui {
	void multicombo(std::string_view name, std::vector<bool>* values, const std::vector<std::string>& items);
	void multicombo(std::string_view name, std::vector<bool*>& values, const std::vector<std::string>& items);
}