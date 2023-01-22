#include <iostream>
#include <null-gui.h>
#include <null-renderer-directx9.h>

null::renderer::c_window window{ };

color_t<int> clr{ };

void create_ui() {
	auto window{ new null::gui::c_window{ "test window", { 50, 100 }, { 400, 300 } } };
	//window->container_flags |= null::e_container_flags::auto_size;
	{
		static color_t<int> color{ 255, 255, 255 };
		window->add_child(new null::gui::c_color_picker{ "color picker", &color });

		static bool checl_box_value{ };
		window->add_child(new null::gui::c_check_box{ "check box", &checl_box_value });
		window->add_child(new null::gui::c_check_box{ "check box2", &checl_box_value });
		window->add_child(new null::gui::c_check_box{ "check box2", &checl_box_value });
		window->add_child(new null::gui::c_check_box{ "check box2", &checl_box_value });
		window->add_child(new null::gui::c_check_box{ "check box2", &checl_box_value });

		static float slider_bar_value{ 55 };
		window->add_child(new null::gui::c_slider_bar{ "slider bar", &slider_bar_value, 0, 100 });

		static std::string buffer{ "null-gui" };
		window->add_child(new null::gui::c_text_input{ "text input", &buffer });

		static int selected_id{ };
		std::vector<std::string> single_items(19);
		std::iota(single_items.begin(), single_items.end(), '0');
		window->add_child(new null::gui::c_list_box{ "list box single selectable", &selected_id, single_items, { -1, 100 } });

		static std::vector<std::pair<std::string, bool>> multi_items{ };
		if(multi_items.empty()) std::ranges::transform(single_items, std::back_inserter(multi_items), [](const std::string& a) { return std::make_pair(a, false); });
		window->add_child(new null::gui::c_list_box{ "list box multi selectable", &multi_items });

		window->add_child(new null::gui::c_combo_box{ "combo", &selected_id, single_items });
		window->add_child(new null::gui::c_combo_box{ "multi selectable combo", &multi_items });

		rect_t reasct{ };
		reasct.origin(rect_t::right | rect_t::left);

		std::ranges::for_each(std::views::iota(0, 6), [&](int id) {
			window->add_child(new null::gui::c_button{ std::format("window button {}", id) });
			});

		auto group{ new null::gui::c_group{ "group" } }; {
			std::ranges::for_each(std::views::iota(0, 17), [&](int id) {
				group->add_child(new null::gui::c_button{ std::format("group button {}", id) });
				});
		} window->add_child(group);

		std::ranges::for_each(std::views::iota(6, 21), [&](int id) {
			window->add_child(new null::gui::c_button{ std::format("window button {}", id) });
			});
	} null::gui::c_window::add_window(window);
}

void destroy_ui() {
	null::gui::interfaces::i_widget::widgets.clear();
	null::gui::c_window::window_stack.clear();
}

#define debug_print_widget(widget) \
	widgets.data.push_back({#widget, { 255, 100, 100 }}); \
	widgets.data.push_back({std::format(" = "), { }});  \
	widgets.data.push_back({std::format("{}\n", null::gui::interfaces::i_widget::widgets[null::gui::widget] ? null::gui::interfaces::i_widget::widgets[null::gui::widget]->name : "empty"), { 100, 255, 100 }});

void main_loop() {
	null::render::begin_frame(window);
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

		window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

		window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add(null::input::wnd_proc);
		window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add(null::gui::wnd_proc);

		null::input::keys[null::input::e_key_id::num_0].callbacks.at<null::input::e_key_callbacks::on_up>().add(create_ui);
		null::input::keys[null::input::e_key_id::num_1].callbacks.at<null::input::e_key_callbacks::on_up>().add(destroy_ui);

		/*const auto pring{ [](auto a) {
			vec2_t g{ rect_t::scale_from_origin(rect_t::e_origin(a)) };
			else std::cout << std::endl;
		} };
		std::cout << "rect_t::top\t| rect_t::left\t"; pring(rect_t::top | rect_t::left);
		std::cout << "rect_t::top\t| rect_t::right\t"; pring(rect_t::top | rect_t::right);
		std::cout << "rect_t::bottom	| rect_t::left\t"; pring(rect_t::bottom | rect_t::left);
		std::cout << "rect_t::bottom	| rect_t::right\t"; pring(rect_t::bottom | rect_t::right);
		std::cout << "rect_t::center\t\t\t"; pring(rect_t::center);
		std::cout << "rect_t::center	| rect_t::left\t"; pring(rect_t::center | rect_t::left);
		std::cout << "rect_t::center	| rect_t::right\t"; pring(rect_t::center | rect_t::right);
		std::cout << "rect_t::center	| rect_t::top\t"; pring(rect_t::center | rect_t::top);
		std::cout << "rect_t::center	| rect_t::bottom"; pring(rect_t::center | rect_t::bottom);
		std::cout << std::midpoint(-1, 1);*/
		window.create();
		null::gui::initialize();
		window.main_loop();
		window.destroy();
	} catch(const std::exception& exp) {
		std::cout << exp.what() << std::endl;
	}
}