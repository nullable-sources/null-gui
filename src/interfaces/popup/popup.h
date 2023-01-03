#pragma once
#include <interfaces/container/container.h>

namespace null::gui::interfaces {
	class i_popup : public virtual i_container {
	public:
		i_popup() { flags &= ~e_widget_flags::visible; }

	public:
		virtual bool can_close_on_lost_focus(i_widget* new_focused_widget) const;

	public:
		virtual void open() override;
		virtual void close() override;

	public:
		virtual void on_child_lost_focus(i_widget* child, i_widget* new_focused_widget) override;

		virtual void on_focused() override;
		virtual void on_lost_focus(i_widget* new_focused_widget) override;
	};
}