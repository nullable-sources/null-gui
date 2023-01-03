#pragma once
#include <interfaces/container/container.h>
#include <widgets/layout/layout.h>

namespace null::gui::interfaces {
	class i_list_box : public virtual i_container {
	public:
		class c_main_layout : public c_layout {
		public: using c_layout::c_layout;
			  struct style_t {
				  int max_show_items{ 8 };
			  } style{ };

		public:
			float clamped_size{ };

		public:
			virtual void append_auto_positioning(i_widget* child) override;
		};

		class c_internal_layout : public c_layout {
		public: using c_layout::c_layout;
			  c_main_layout* layout{ };
			  c_layout* scrollbar_layout{ };

		public:
			virtual void append_auto_size() override;

		protected:
			virtual void setup_scrollbar() override;
		};

	public:
		virtual void create_default_internal_layout(const e_layout_flags& _flags, const c_layout::style_t& _style) override;
		virtual void create_default_layout(const e_layout_flags& _flags, const c_layout::style_t& _style) override;
		virtual void create_default_scrollbar(const e_layout_flags& _flags, const c_layout::style_t& _style) override;

	public:
		virtual void build(int* value, const std::vector<std::string>& items);
		virtual void build(std::vector<std::pair<std::string, bool>>* items);
	};
}