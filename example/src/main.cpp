#include <iostream>
#include <mutex>
#include <null-gui.h>
#include <null-renderer-directx9.h>

null::renderer::c_window window{ };

void create_ui() {
	auto window{ new null::gui::c_window{ "test window", { 100 }, { 400, 300 } } };
	//window->container_flags |= null::e_container_flags::auto_size;
	{
		static color_t<int> color{ 255, 255, 255 };
		window->add_widget(new null::gui::c_color_picker{ "color picker", &color });

		static bool checl_box_value{ };
		window->add_widget(new null::gui::c_check_box{ "check box", &checl_box_value });
		
		static float slider_bar_value{ 55 };
		window->add_widget(new null::gui::c_slider_bar{ "slider bar", &slider_bar_value, 0, 100 });

		static std::string buffer{ "null-gui" };
		window->add_widget(new null::gui::c_text_input{ "text input", &buffer });

		static int selected_id{ };
		std::vector<std::string> single_items(10);
		std::iota(single_items.begin(), single_items.end(), '0');
		window->add_widget(new null::gui::c_list_box{ "list box single selectable", &selected_id, single_items, { -1, 100 } });

		static std::vector<std::pair<std::string, bool>> multi_items{ };
		if(multi_items.empty()) std::ranges::transform(single_items, std::back_inserter(multi_items), [](const std::string& a) { return std::make_pair(a, false); });
		window->add_widget(new null::gui::c_list_box{ "list box multi selectable", &multi_items });

		window->add_widget(new null::gui::c_combo_box{ "combo", &selected_id, single_items });
		window->add_widget(new null::gui::c_combo_box{ "multi selectable combo", &multi_items });

		std::ranges::for_each(std::views::iota(0, 6), [&](int id) {
			window->add_widget(new null::gui::c_button{ std::format("window button {}", id) });
			});

		auto column_with_size{ new null::gui::c_columns{ { 100, 0, 40 } } }; {
			std::ranges::for_each(std::views::iota(size_t{ }, column_with_size->node.childs.size()), [&](const size_t& id) {
				std::ranges::for_each(std::views::iota(0, 3), [&](int id2) {
					column_with_size->at(id)->add_widget(new null::gui::c_button{ std::to_string(id2) });
					});
				});
		} window->add_widget(column_with_size);

		auto column{ new null::gui::c_columns{ 3 } }; {
			std::ranges::for_each(std::views::iota(0, 3), [&](int id) {
				column->at(0)->add_widget(new null::gui::c_button{ std::to_string(id) });
				});

			std::ranges::for_each(std::views::iota(0, 2), [&](int id) {
				column->at(1)->add_widget(new null::gui::c_button{ std::to_string(id) });
				});

			std::ranges::for_each(std::views::iota(0, 4), [&](int id) {
				column->at(2)->add_widget(new null::gui::c_button{ std::to_string(id) });
				});
		} window->add_widget(column);

		auto group{ new null::gui::c_group{ "group", { -1, -1 } } }; {
			std::ranges::for_each(std::views::iota(0, 17), [&](int id) {
				group->add_widget(new null::gui::c_button{ std::format("group button {}", id) });
				});
		} window->add_widget(group);

		std::ranges::for_each(std::views::iota(6, 21), [&](int id) {
			window->add_widget(new null::gui::c_button{ std::format("window button {}", id) });
			});
	} null::gui::c_window::add_window(window);
}

void destroy_ui() {
	null::gui::i_widget::widgets.clear();
	null::gui::c_window::window_stack.clear();
}

#define debug_print_widget(widget) \
	widgets.data.push_back({#widget, { 255, 100, 100 }}); \
	widgets.data.push_back({std::format(" = "), { }});  \
	widgets.data.push_back({std::format("{}\n", null::gui::i_widget::widgets[null::gui::widget] ? null::gui::i_widget::widgets[null::gui::widget]->name : "empty"), { 100, 255, 100 }});

void main_loop() {
	null::render::begin_frame(window);
	null::gui::begin_frame();
	null::input::begin_frame(window); {
		null::render::multicolor_text_t<std::string> widgets{ };

		debug_print_widget(e_widget_state::active);
		debug_print_widget(e_widget_state::hovered);
		debug_print_widget(e_widget_state::focused);

		null::render::background.draw_text(widgets, { });
		null::render::background.draw_text(std::format("fps: {:3.0f}", 1.f / window.time_data.delta_time), { window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });
	}
	null::gui::end_frame();
	null::render::end_frame();
	null::renderer::setup_default_draw_data();
}

int main(HINSTANCE instance) {
	window = null::renderer::c_window{ instance };

	try {
		null::render::atlas.add_font_from_file_ttf("C:\\Windows\\fonts\\Tahoma.ttf", 13.f, nullptr, null::render::c_font::glyph_t::ranges_cyrillic());

		window.callbacks.add<void()>(utils::win::e_window_callbacks::on_main_loop, main_loop);

		window.callbacks.add<int(HWND, UINT, WPARAM, LPARAM)>(utils::win::e_window_callbacks::on_wnd_proc, null::input::wnd_proc);
		window.callbacks.add<int(HWND, UINT, WPARAM, LPARAM)>(utils::win::e_window_callbacks::on_wnd_proc, null::gui::wnd_proc);

		null::input::keys[null::input::e_key_id::num_0].callbacks.add<void()>(null::input::e_key_state::up, create_ui);
		null::input::keys[null::input::e_key_id::num_1].callbacks.add<void()>(null::input::e_key_state::up, destroy_ui);

		window.create();
		null::gui::initialize();
		window.main_loop();
		window.destroy();
	} catch(const std::exception& exp) {
		std::cout << exp.what() << std::endl;
	}
}