#include <null-gui.h>

namespace null::gui {
	void c_layout::setup_self() {
		switch(size_data.type) {
			case e_size_type::procentage: {
				if(size_data.size != -1.f) {
					vec2_t parent_working_rect{ node.parent->working_region.size() };
					if(size_data.size.x != -1.f) size.x = parent_working_rect.x * size_data.size.x;
					if(size_data.size.y != -1.f) size.y = parent_working_rect.y * size_data.size.y;
				}
			} break;
			case e_size_type::absoulute: {
				if(size_data.size.x != -1.f) size.x = size_data.size.x;
				if(size_data.size.y != -1.f) size.y = size_data.size.y;
			} break;
			case e_size_type::none: {
				if(!(flags & e_layout_flags::auto_size_x)) size.x = node.parent->working_region.size().x;
				if(!(flags & e_layout_flags::auto_size_y)) size.y = node.parent->working_region.size().y;
			} break;
		}
		i_layout::setup_self();
	}
}