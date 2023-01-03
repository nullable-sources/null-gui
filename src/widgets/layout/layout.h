#pragma once
#include <interfaces/layout/layout.h>

namespace null::gui {
	class c_layout : public interfaces::i_layout {
	public: using i_layout::i_layout;
		  enum class e_size_type {
			  none,
			  procentage,
			  absoulute
		  };

		  struct size_data_t {
			  e_size_type type{ e_size_type::none };
			  vec2_t size{ -1.f };
		  } size_data{ }; //@note: dont use with any auto_size

	public:
		c_layout(const e_layout_flags& _flags, const rect_t::e_origin& _origin, const rect_t::e_origin& _direction, const size_data_t& _size_data)
			: interfaces::i_layout{ _flags, _origin, _direction }, size_data{ _size_data } { }

	public:
		virtual void setup_self() override;

	public:
		virtual bool handling_self_events(const e_widget_event& event, const std::uintptr_t& w_param, const std::uintptr_t& l_param) override { return false; }
	};
}