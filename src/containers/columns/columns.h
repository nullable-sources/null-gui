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
			c_column(vec2_t _size = 0.f) : i_container{ "column" } { size = _size; container_flags |= (size.x == 0.f ? e_container_flags::auto_size : e_container_flags::auto_size_y); }

		protected:
			virtual void setup_bounds() override;

		public:
			virtual void append_auto_positioning(i_widget* widget) override;

			virtual void setup() override;

		public:
			virtual bool event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override;
		};

	public:
		c_columns(int split) : i_container{ "columns" } {
			if(split < 2) throw std::runtime_error{ "there cannot be less than two columns" };
			container_flags |= e_container_flags::auto_size_y;
			std::ranges::for_each(std::views::iota(0, split), [=](const int&) { add_widget(new c_column{ }); });
		}

		c_columns(std::vector<int> sizes) : i_container("columns") {
			if(sizes.size() < 2) throw std::runtime_error{ "there cannot be less than two columns" };
			container_flags |= e_container_flags::auto_size_y;
			std::ranges::for_each(sizes, [=](int& size) { add_widget(new c_column{ { size, 0 } }); });
		}

		i_widget* at(int i) { return node.childs[i].get(); }

	protected:
		virtual void setup_bounds() override;
		virtual void setup_widget(i_widget* widget) override;

	public:
		virtual void append_auto_positioning(i_widget* widget) override;

		virtual void setup() override;

	public:
		virtual bool event_handling(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override;
	};
}