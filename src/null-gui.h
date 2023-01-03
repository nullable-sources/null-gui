#pragma once
#include <null-render.h>
#include <null-input.h>
#include <null-sdk.h>

#include <widgets/layout/layout.h>

#include <widgets/button/button.h>
#include <widgets/check-box/check-box.h>
#include <widgets/slider-bar/slider-bar.h>
#include <widgets/text-input/text-input.h>
#include <widgets/combo-box/combo-box.h>
#include <widgets/list-box/list-box.h>
#include <widgets/selectable/selectable.h>
#include <widgets/color-picker/color-picker.h>
#include <widgets/title-bar/title-bar.h>
#include <widgets/window/window.h>
#include <widgets/group/group.h>
#include <widgets/popup/popup.h>

#include <interfaces/movable/movable.h>
#include <interfaces/popup/popup.h>
#include <interfaces/list-box/list-box.h>
#include <interfaces/layout/layout.h>

namespace null::gui {
	inline render::c_draw_list gui_layer{ };

	static void initialize() {
		render::custom_lists.push_back(&gui_layer);
	}

	static void end_frame() {
		std::ranges::for_each(c_window::window_stack | std::views::filter([](const std::shared_ptr<c_window>& window) { return window->flags & e_widget_flags::visible; }), [](const std::shared_ptr<c_window>& window) {
			window->handle();
			});
	}

	int wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
}