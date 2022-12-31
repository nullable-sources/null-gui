#pragma once
#include <null-render.h>
#include <null-input.h>
#include <null-sdk.h>

#include <widgets/button/button.h>
#include <widgets/check-box/check-box.h>
#include <widgets/slider-bar/slider-bar.h>
#include <widgets/text-input/text-input.h>
#include <widgets/combo-box/combo-box.h>
#include <widgets/list-box/list-box.h>
#include <widgets/selectable/selectable.h>
#include <widgets/color-picker/color-picker.h>

#include <containers/window/window.h>
#include <containers/group/group.h>
#include <containers/popup/popup.h>
#include <containers/columns/columns.h>

namespace null::gui {
	inline render::c_draw_list gui_layer{ };

	static void initialize() {
		render::custom_lists.push_back(&gui_layer);
	}

	static void end_frame() {
		std::ranges::for_each(c_window::window_stack | std::views::filter([](std::shared_ptr<c_window>& window) { return window->flags & e_widget_flags::visible; }), [](std::shared_ptr<c_window>& window) {
			window->handle();
			});
	}

	int wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
}