#include <null-gui.h>

//@todo: rewrite this trash
namespace null::gui {
	void c_columns::c_column::setup_bounds() {
		bounds = node.parent->bounds;
		i_container::setup_bounds();
	}

	void c_columns::c_column::append_auto_positioning(i_widget* widget) {
		i_container::append_auto_positioning(widget);
		auto_positioning.next_position.y += style.widget_padding;
	}

	void c_columns::c_column::setup() {
		//@note: after that, all child columns will be the same height, perhaps it is better to add a flag to enable/disable this function?
		size.y = std::max(size.y, node.parent->working_region.size().y);
		working_region = rect_t{ vec2_t{ 0 }, size };
		i_container::setup();
	}

	void c_columns::setup_bounds() {
		bounds = node.parent->bounds;
		i_container::setup_bounds();
	}

	void c_columns::setup_widget(i_widget* widget) {
		if(!(flags & e_container_flags::auto_size_x)) {
			if(c_column * column{ static_cast<c_column*>(widget) }) { //@note: that's fucked up
				if(column->flags & e_container_flags::auto_size_x) {
					column->size.x = std::max(0.f, (size.x - (style.column_padding * (node.childs.size() - 1))) / node.childs.size());
					float custom_offsets{ };
					int count{ };
					for(c_column* child_column : node.childs | std::views::transform([](std::shared_ptr<i_widget>& child) { return static_cast<c_column*>(child.get()); })
						| std::views::filter([&](c_column* column) { return column && !(column->flags & e_container_flags::auto_size_x); })) {
						custom_offsets += column->size.x - child_column->size.x;
						count++;
					}
					column->size.x += custom_offsets / (node.childs.size() - count);
				}
			} else throw std::runtime_error{ "childs can contain only columns" };
		}
		i_container::setup_widget(widget);
	}

	void c_columns::append_auto_positioning(i_widget* widget) {
		auto_positioning.max_position = math::max(auto_positioning.max_position, widget->pos + widget->size - pos);
		auto_positioning.next_position.x += widget->size.x + style.column_padding;
	}

	void c_columns::setup() {
		size.x = node.parent->working_region.size().x;
		i_container::setup();
		working_region = rect_t{ vec2_t{ 0 }, size };
	}
}