#pragma once
#include <widgets/widget.h>
#include <widgets/scroll-bar/scroll-bar.h>

namespace null::gui {
	enum class e_container_flags {
		auto_size_x = 1 << 3,
		auto_size_y = 1 << 4,
		auto_size = auto_size_x | auto_size_y,
	}; enum_create_bit_operators(e_container_flags, true);
	enum_create_cast_operator(e_container_flags, -);

	class i_container : public i_widget {
	public:
		struct auto_positioning_t {
			vec2_t max_position{ }, next_position{ };
		} auto_positioning;

		c_scroll_bar* scroll_bar{ };

	public:
		i_container(const std::string_view& _name) : i_widget{ _name } { }
		i_container(const std::string_view& _name, const vec2_t& _pos, const vec2_t& _size) : i_widget{ _name } { pos = _pos; size = _size; }

	protected:
		virtual void setup_bounds();
		virtual void setup_widget(i_widget* widget);

	public:
		virtual void open() { flags |= e_widget_flags::visible; }
		virtual void close() { flags &= ~e_widget_flags::visible; }

		virtual void append_auto_size();
		virtual void setup_auto_positioning();
		virtual void append_auto_positioning(i_widget* widget);

		virtual i_widget* add_widget(i_widget* widget) override;

		virtual void setup() override;
	};
}