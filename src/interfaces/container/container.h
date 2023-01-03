#pragma once
#include <interfaces/layout/layout.h>
#include <widgets/title-bar/title-bar.h>

namespace null::gui::interfaces {
	class i_container : public virtual i_layout {
	public:
		i_widget* title_bar{ };
		i_layout* internal_layout{ }, *layout{ }; //@note: main body layout

	public:
		i_container() { }
		i_container(const vec2_t& _pos, const vec2_t& _size) { pos = _pos; size = _size; }

	protected:
		virtual void create_default_widgets(const e_layout_flags& _flags, const i_layout::style_t& _style = { });
		virtual void create_default_titlebar();
		virtual void create_default_internal_layout(const e_layout_flags& _flags, const i_layout::style_t& _style);
		virtual void create_default_layout(const e_layout_flags& _flags, const i_layout::style_t& _style);
		virtual void create_default_scrollbar(const e_layout_flags& _flags, const i_layout::style_t& _style);

	public:
		virtual i_widget* add_child(i_widget* child) { return layout->add_widget(child); }

	public:
		virtual void open() { flags |= e_widget_flags::visible; }
		virtual void close() { flags &= ~e_widget_flags::visible; }

	protected:
		virtual void setup_scrollbar() override { }
		virtual float get_scroll_value() override { return 0.f; }
	};
}