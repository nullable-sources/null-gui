#pragma once
#include <widgets/widget.h>
#include <widgets/scroll-bar/scroll-bar.h>

namespace null::gui {
	enum class e_container_flags {
		none,
		auto_size_x = 1 << 0,
		auto_size_y = 1 << 1,
		auto_size = auto_size_x | auto_size_y,

		disable_scroll_bar = 1 << 2,
	}; enum_create_bit_operators(e_container_flags, true);
	enum_create_cast_operator(e_container_flags, -);

	class i_container : public i_widget {
	public:
		struct auto_positioning_t {
			vec2_t max_position{ };
			vec2_t next_position{ };
		} auto_positioning;

		c_scroll_bar scroll_bar{ "scroll_bar" };

		e_container_flags container_flags{ };

	public:
		i_container(std::string_view _name) : i_widget(_name) { scroll_bar.node.parent = this; }

	protected:
		virtual void setup_bounds();
		virtual void setup_widget(i_widget* widget);

		virtual void setup_scroll_bar();
		virtual void draw_scroll_bar();
		virtual bool handle_scroll_bar_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param);

	public:
		virtual void open() { flags |= e_widget_flags::visible; }
		virtual void close() { flags &= ~e_widget_flags::visible; }

		virtual void append_auto_size();
		virtual void setup_auto_positioning();
		virtual void append_auto_positioning(i_widget* widget);

		virtual i_widget* add_widget(i_widget* widget) override;

		virtual void setup() override;
		virtual void draw() override;

		virtual bool event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override;
	};
}