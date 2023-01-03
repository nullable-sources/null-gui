#include <null-gui.h>

namespace null::gui::interfaces {
	void i_container::create_default_widgets(const e_layout_flags& _flags, const i_layout::style_t& _style) {
		create_default_titlebar();

		create_default_internal_layout(_flags, _style); {
			create_default_scrollbar(_flags, _style);
			create_default_layout(_flags, _style);

			if(_flags & e_layout_flags::auto_size_x) {
				std::ranges::reverse(internal_layout->node.childs);
				std::swap(internal_layout->origin, internal_layout->direction);
			}
		} add_widget(internal_layout);
	}

	void i_container::create_default_titlebar() {
		title_bar = add_widget(new c_title_bar{ name });
	}

	void i_container::create_default_internal_layout(const e_layout_flags& _flags, const i_layout::style_t& _style) {
		internal_layout = new c_layout{ e_layout_flags{ _flags | e_layout_flags::clip_bounds }, rect_t::right, rect_t::left };
		internal_layout->style = _style;
	}

	void i_container::create_default_layout(const e_layout_flags& _flags, const i_layout::style_t& _style) {
		layout = dynamic_cast<c_layout*>(internal_layout->add_widget(new c_layout{ e_layout_flags{ _flags | e_layout_flags::auto_size_y } }));
		layout->style.spacing = _style.spacing;
	}

	void i_container::create_default_scrollbar(const e_layout_flags& _flags, const i_layout::style_t& _style) {
		scroll_bar = internal_layout->scroll_bar = (c_scroll_bar*)internal_layout->add_widget(new c_scroll_bar{ });
	}
}