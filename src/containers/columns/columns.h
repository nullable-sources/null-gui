#pragma once
#include <containers/group/group.h>

namespace null::gui {
	class c_columns : public i_container {
	public:
		struct style_t {
			float column_padding{ 10.f };
		} style{ };

	public:
		class c_column : public i_container {
		public:
			struct style_t {
				float widget_padding{ 10.f };
			} style{ };

		public:
			c_column(const bool& _const_size) : i_container{ "column" } { flags |= (_const_size ? e_container_flags::auto_size : e_container_flags::auto_size_y); }
			c_column(const vec2_t& _size) : c_column{ _size.x == 0.f } { size = _size; }

		protected:
			virtual void setup_bounds() override;

		public:
			virtual void append_auto_positioning(i_widget* widget) override;

			virtual void setup() override;

		public:
			virtual bool handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override { return false; }
		};

	public:
		c_columns(const int& split, const bool& same_column_size = true) : i_container{ "columns" } {
			if(split < 2) throw std::runtime_error{ "there cannot be less than two columns" };
			flags |= e_container_flags::auto_size_y;
			if(!same_column_size) flags |= e_container_flags::auto_size_x;
			std::ranges::for_each(std::views::iota(0, split), [=](const int&) { add_widget(new c_column{ true }); });
		}

		c_columns(const std::vector<int>& sizes) : i_container("columns") {
			if(sizes.size() < 2) throw std::runtime_error{ "there cannot be less than two columns" };
			flags |= e_container_flags::auto_size_y;
			std::ranges::for_each(sizes, [=](const int& size) { add_widget(new c_column{ { size, 0 } }); });
		}

		i_widget* at(const int& i) { return node.childs[i].get(); }

	protected:
		virtual void setup_bounds() override;
		virtual void setup_widget(i_widget* widget) override;

	public:
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;

	public:
		virtual bool handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override { return false; }
	};
}