#include <null-gui.h>

namespace null::gui::interfaces {
	void i_list_box::c_main_layout::append_auto_positioning(i_widget* child) {
		c_layout::append_auto_positioning(child);
		if(style.max_show_items < node.childs.size() && child == node.childs[style.max_show_items].get())
			clamped_size = auto_positioning.max_position.y;
	}

	void i_list_box::c_internal_layout::append_auto_size() {
		c_layout::append_auto_size();
		if(flags & e_layout_flags::auto_size_y && layout) size.y = layout->clamped_size ? layout->clamped_size : auto_positioning.max_position.y;
	}

	void i_list_box::c_internal_layout::setup_scrollbar() {
		c_layout::setup_scrollbar();

		if(!scroll_bar || !scrollbar_layout) return;
		if(scroll_bar->flags & e_widget_flags::visible) scrollbar_layout->flags |= e_widget_flags::visible;
		else scrollbar_layout->flags &= ~e_widget_flags::visible;
	}

	void i_list_box::create_default_internal_layout(const e_layout_flags& _flags, const c_layout::style_t& _style) {
		internal_layout = new c_internal_layout{ e_layout_flags{ _flags | e_layout_flags::clip_bounds }, rect_t::right, rect_t::left };
		internal_layout->style = c_layout::style_t{ 0.f, 0.f };
	}

	void i_list_box::create_default_layout(const e_layout_flags& _flags, const c_layout::style_t& _style) {
		layout = dynamic_cast<c_main_layout*>(internal_layout->add_widget(new c_main_layout{ e_layout_flags{ _flags | e_layout_flags::auto_size_y } }));
		layout->style.spacing = 0.f;

		((c_internal_layout*)internal_layout)->layout = (c_main_layout*)layout;
	}

	void i_list_box::create_default_scrollbar(const e_layout_flags& _flags, const c_layout::style_t& _style) {
		auto scrollbar_layout{ new c_layout{ e_layout_flags{ e_layout_flags::auto_size_x | -e_widget_flags::ignore_scroll_offset } } }; {
			scrollbar_layout->style.padding = _style.spacing;
			scroll_bar = internal_layout->scroll_bar = (c_scroll_bar*)scrollbar_layout->add_widget(new c_scroll_bar{ });
		} internal_layout->add_widget(scrollbar_layout);
		((c_internal_layout*)internal_layout)->scrollbar_layout = scrollbar_layout;
	}

	void i_list_box::build(int* value, const std::vector<std::string>& items) {
		std::ranges::for_each(std::views::iota(0, (int)items.size()), [&](const int& item_id) {
			add_child(new c_single_selectable{ items[item_id], value, item_id });
			});

		setup();
	}

	void i_list_box::build(std::vector<std::pair<std::string, bool>>* items) {
		std::ranges::for_each(std::views::iota(0, (int)items->size()), [&](const int& item_id) {
			add_child(new c_multi_selectable{ (*items)[item_id].first, &(*items)[item_id].second, item_id });
			});

		setup();
	}
}