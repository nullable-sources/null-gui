#pragma once
#include <interfaces/selectable/selectable.h>

namespace null::gui {
	class c_single_selectable : public interfaces::i_selectable {
	public:
		int* value{ };

	public:
		c_single_selectable(const std::string_view& _name, int* _value, const int& _id) : interfaces::i_widget{ _name }, interfaces::i_selectable{ _id }, value{ _value } { }

	public:
		bool selected() const override { return *value == id; }
		void select() override { *value = id; }
	};

	class c_multi_selectable : public interfaces::i_selectable {
	public:
		bool* value{ };

	public:
		c_multi_selectable(const std::string_view& _name, bool* _value, const int& _id) : interfaces::i_widget{ _name }, interfaces::i_selectable{ _id }, value{ _value } { }

	public:
		bool selected() const override { return *value; }
		void select() override { *value = !*value; }
	};
}