#pragma once
#include <widgets/scroll-bar/scroll-bar.h>

namespace null::gui {
	enum class e_layout_flags {
		auto_size_x = 1 << 4,
		auto_size_y = 1 << 5,
		auto_size = auto_size_x | auto_size_y,

		auto_size_by_childs_x = 1 << 6,
		auto_size_by_childs_y = 1 << 7,
		auto_size_by_childs = auto_size_by_childs_x | auto_size_by_childs_y, //@note: if the flag is set, then the working_region will grow by the number of child widgets (the maximum size of each widget will be occupied)
		dont_skip_invisible_childs = 1 << 8, //@note: works together with auto_size_by_childs, all widgets will be taken into account for size division
		set_size = 1 << 9,
		clip_bounds = 1 << 10
	}; enum_create_bit_operators(e_layout_flags, true);
	enum_create_cast_operator(e_layout_flags, -);

	namespace interfaces {
		class i_layout : public virtual i_widget {
		public:
			struct style_t {
				vec2_t spacing{ 5.f, 10.f };
				vec2_t padding{ 0.f };
			} style{ };

			struct auto_positioning_t {
				vec2_t max_position{ }, next_position{ };
			} auto_positioning{ };

		public:
			c_scroll_bar* scroll_bar{ };

			rect_t::e_origin origin{ rect_t::top }, direction{ rect_t::bottom };

		public:
			i_layout() { }
			i_layout(const e_layout_flags& _flags, const rect_t::e_origin& _origin = rect_t::top, const rect_t::e_origin& _direction = rect_t::bottom) : i_layout{ } { flags |= _flags; origin = _origin; direction = _direction; }

		public:
			virtual void append_auto_size();
			virtual void append_auto_positioning(i_widget* child);

			virtual void setup_self() override;
			virtual void setup_childs() override;
			virtual void setup_child(i_widget* child) override;
			virtual void setup_bounds() override { bounds = { { 0.f }, size }; }
			virtual void draw() override;

		public:
			virtual rect_t get_real_bounds() const override {
				if(node.parent) return { math::max(bounds.min + pos, node.parent->bounds.min + node.parent->pos), math::min(bounds.max + pos, node.parent->bounds.max + node.parent->pos) };
				return i_widget::get_real_bounds();
			}

		protected:
			virtual void setup_scrollbar();
			virtual float get_scroll_value();
		};
	}
}