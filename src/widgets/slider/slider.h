#pragma once
#include <null-sdk.h>

namespace null::gui {
	namespace style::slider {
		inline float thickness{ 5.f };
		inline float rounding{ };
	}

	void slider_int(std::string_view name, int* value, int min, int max, std::string_view format = "{}");
	void slider_float(std::string_view name, float* value, float min, float max, std::string_view format = "{:.0f}");

	namespace impl {
		enum class e_data_type;
		template<typename value_t>
			requires std::is_arithmetic_v<value_t>
		void slider(std::string_view name, value_t* value, const value_t& min, const value_t& max, std::string_view format, e_data_type data_type);

		namespace behaviors {
			void slider(std::string_view widget_name, const rect_t& widget_rect, bool& hovered, bool& pressed);
			bool slider_text_input(std::string_view widget_name, std::string_view input_name, const rect_t& widget_rect);
		}
	}
}