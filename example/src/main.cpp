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


	//auto layout_window{ new null::gui::c_window{ "layout testing", { 100, 100 }, 400 } }; {
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi1" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi2" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi3" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi4" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi5" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi6" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi7" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi8" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi9" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi9" });
	//	auto btn = new null::gui::c_button{ "gui" }; {
	//		btn->callbacks.at<null::gui::e_widget_callbacks::on_mouse_key_up>().add([layout_window](auto) {
	//			layout_window->body_layout->add_widget(new null::gui::c_button{ "fi9" });
	//			});
	//	} layout_window->body_layout->add_widget(btn);
	//	/*layout_window->body_layout->add_widget(new null::gui::c_button{ "fi10" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi11" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi12" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi13" });*/
	//	/*layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi12" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi13" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi14" });
	//	layout_window->body_layout->add_widget(new null::gui::c_button{ "fi337" });*/
	//	/*{
	//		auto layout{ new null::gui::c_vertical_layout{ } }; {
	//			layout->flags |= null::gui::e_container_flags::auto_size;

	//			layout->add_widget(new null::gui::c_button{ "test btn" });
	//			layout->add_widget(new null::gui::c_button{ "test btn" });
	//			layout->add_widget(new null::gui::c_button{ "test btn" });
	//			layout->add_widget(new null::gui::c_button{ "test btn" });
	//			layout->add_widget(new null::gui::c_button{ "test btn" });
	//			{
	//				auto layout2{ new null::gui::c_horizontal_layout{ } }; {
	//					layout2->flags |= null::gui::e_container_flags::auto_size;

	//					layout2->add_widget(new null::gui::c_button{ "test btn" });
	//					layout2->add_widget(new null::gui::c_button{ "test btn" });
	//					layout2->add_widget(new null::gui::c_button{ "test btn" });
	//					layout2->add_widget(new null::gui::c_button{ "test btn" });
	//					layout2->add_widget(new null::gui::c_button{ "test btn" });

	//					{
	//						auto layout3{ new null::gui::c_vertical_layout{ } }; {
	//							layout3->flags |= null::gui::e_container_flags::auto_size;

	//							layout3->add_widget(new null::gui::c_button{ "test btn" });
	//							layout3->add_widget(new null::gui::c_button{ "test btn" });
	//							layout3->add_widget(new null::gui::c_button{ "test btn" });
	//							layout3->add_widget(new null::gui::c_button{ "test btn" });
	//							layout3->add_widget(new null::gui::c_button{ "test btn" });
	//						} layout2->add_widget(layout3);
	//					}

	//				} layout->add_widget(layout2);
	//			}
	//		} layout_window->add_widget(layout);
	//	}*/
	//	{
	//		/*auto main_layout{ new null::gui::c_horizontal_layout{ } }; {
	//			main_layout->flags |= null::gui::e_container_flags::auto_size_x;

	//			auto group_layout{ new null::gui::c_horizontal_layout{ } }; {
	//				group_layout->flags |= null::gui::e_container_flags::auto_size;
	//				auto group{ new null::gui::c_group{ "group", { 100, -1 } } }; {
	//					std::ranges::for_each(std::views::iota(0, 17), [&](int id) {
	//						group->add_widget(new null::gui::c_button{ std::format("group button {}", id) });
	//						});
	//				} group_layout->add_widget(group);
	//			} main_layout->add_widget(group_layout);

	//			auto button_layout{ new null::gui::c_horizontal_layout{ } }; {
	//				button_layout->flags |= null::gui::e_container_flags::auto_size;
	//				button_layout->add_widget(new null::gui::c_button{ "test btn 1" });
	//				button_layout->add_widget(new null::gui::c_button{ "test btn 2" });
	//				button_layout->add_widget(new null::gui::c_button{ "test btn 3" });
	//			} main_layout->add_widget(button_layout);
	//		} layout_window->add_widget(main_layout);*/

	//		//auto main_layout{ new null::gui::c_layout{ } }; {
	//		//	main_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//	//main_layout->style.spacing = 10.f;
	//		//	//main_layout->style.padding = 10.f;
	//		//	main_layout->origin = rect_t::left;
	//		//	main_layout->direction = rect_t::right;

	//		//	auto left_layout{ new null::gui::c_layout{ } }; {
	//		//		left_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//left_layout->origin = rect_t::center;
	//		//		//left_layout->direction = rect_t::e_origin(rect_t::center | rect_t::bottom);
	//		//		left_layout->style.padding = 10.f;
	//		//		left_layout->size_data = { null::gui::c_layout::e_size_type::procentage, { 0.4f, -1.f } };
	//		//		left_layout->add_widget(new null::gui::c_button{ "left btn" });
	//		//	} main_layout->add_widget(left_layout);

	//		//	auto hui{ new null::gui::c_layout{ } }; {
	//		//		hui->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		hui->origin = rect_t::top;
	//		//		hui->direction = rect_t::bottom;
	//		//		left_layout->style.padding = 10.f;
	//		//		hui->size_data = { null::gui::c_layout::e_size_type::procentage, { 0.3f, 1.f } };
	//		//		//hui->direction = rect_t::e_origin(rect_t::center | rect_t::right);
	//		//		auto button_layout{ new null::gui::c_layout{ } }; {
	//		//			button_layout->flags |= null::gui::e_container_flags::auto_size;
	//		//			//button_layout->flags |= null::gui::e_layout_flags::set_size;
	//		//			//button_layout->size_data.size = 0.8f;
	//		//			button_layout->origin = rect_t::right;
	//		//			button_layout->direction = rect_t::left;
	//		//			hui->add_widget(new null::gui::c_button{ "1" });
	//		//			hui->add_widget(new null::gui::c_button{ "2" });
	//		//			hui->add_widget(new null::gui::c_button{ "3" });
	//		//		} //hui->add_widget(button_layout);
	//		//	} main_layout->add_widget(hui);
	//		//} layout_window->add_widget(main_layout);

	//		//auto hui{ new null::gui::c_layout{ } }; {
	//		//	hui->flags |= null::gui::e_container_flags::auto_size;
	//		//	auto button_layout{ new null::gui::c_layout{ } }; {
	//		//		button_layout->flags |= null::gui::e_container_flags::auto_size;
	//		//		//button_layout->flags |= null::gui::e_layout_flags::set_size;
	//		//		//button_layout->size_data.size = 0.8f;
	//		//		button_layout->origin = rect_t::right;
	//		//		button_layout->direction = rect_t::left;
	//		//		hui->add_widget(new null::gui::c_button{ "1" });
	//		//	} //hui->add_widget(button_layout);

	//		//	auto button_layout2{ new null::gui::c_layout{ } }; {
	//		//		button_layout2->flags |= null::gui::e_container_flags::auto_size;
	//		//		//button_layout2->flags |= null::gui::e_layout_flags::set_size;
	//		//		//button_layout2->size_data.size = 0.2f;
	//		//		button_layout2->origin = rect_t::right;
	//		//		button_layout2->direction = rect_t::left;
	//		//		hui->add_widget(new null::gui::c_button{ "2" });
	//		//	} //hui->add_widget(button_layout2);
	//		//} layout_window->add_widget(hui);
	//		//{
	//		//	auto main_layout{ new null::gui::c_layout{ } }; {
	//		//		main_layout->name = "main_layout";
	//		//		main_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		main_layout->flags |= null::gui::e_layout_flags::reverse_setup;
	//		//		main_layout->origin = rect_t::left; main_layout->direction = rect_t::right;

	//		//		//auto first_layout{ new null::gui::c_layout{ } }; {
	//		//		//	//first_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	first_layout->scroll_bar = (null::gui::c_scroll_bar*)first_layout->add_widget(new null::gui::c_scroll_bar{ });
	//		//		//	first_layout->origin = rect_t::top; first_layout->direction = rect_t::bottom;
	//		//		//	first_layout->size_data = { null::gui::c_layout::e_size_type::procentage, { 0.5f, 1.f } };

	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//} main_layout->add_widget(first_layout);

	//		//		//auto second_layout{ new null::gui::c_layout{ } }; {
	//		//		//	second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//} main_layout->add_widget(second_layout);

	//		//		auto first_layout{ new null::gui::c_layout{ } }; {
	//		//			first_layout->name = "first_layout";
	//		//			first_layout->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout->add_widget(new null::gui::c_button{ "fi" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "f2" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		} main_layout->add_widget(first_layout);

	//		//		auto first_layout1{ new null::gui::c_layout{ } }; {
	//		//			first_layout1->name = "first_layout";
	//		//			first_layout1->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout1->add_widget(new null::gui::c_button{ "fi3123123" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "f2" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "first 1" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "first 2" });
	//		//		} main_layout->add_widget(first_layout1);

	//		//		auto second_layout{ new null::gui::c_layout{ } }; {
	//		//			second_layout->name = "second_layout";
	//		//			second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//			second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//			second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		} main_layout->add_widget(second_layout);
	//		//	} layout_window->add_widget(main_layout);
	//		//}

	//		//{
	//		//	auto main_layout{ new null::gui::c_layout{ } }; {
	//		//		main_layout->name = "main_layou1t";
	//		//		main_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		main_layout->origin = rect_t::right; main_layout->direction = rect_t::left;

	//		//		//auto first_layout{ new null::gui::c_layout{ } }; {
	//		//		//	//first_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	first_layout->scroll_bar = (null::gui::c_scroll_bar*)first_layout->add_widget(new null::gui::c_scroll_bar{ });
	//		//		//	first_layout->origin = rect_t::top; first_layout->direction = rect_t::bottom;
	//		//		//	first_layout->size_data = { null::gui::c_layout::e_size_type::procentage, { 0.5f, 1.f } };

	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//} main_layout->add_widget(first_layout);

	//		//		//auto second_layout{ new null::gui::c_layout{ } }; {
	//		//		//	second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//} main_layout->add_widget(second_layout);

	//		//		auto first_layout{ new null::gui::c_layout{ } }; {
	//		//			first_layout->name = "first_layout1";
	//		//			first_layout->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout->add_widget(new null::gui::c_button{ "fi" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "f2" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//			first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		} main_layout->add_widget(first_layout);

	//		//		auto first_layout1{ new null::gui::c_layout{ } }; {
	//		//			first_layout1->name = "first_layou1t";
	//		//			first_layout1->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout1->add_widget(new null::gui::c_button{ "fi3123123" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "f2" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "first 1" });
	//		//			first_layout1->add_widget(new null::gui::c_button{ "first 2" });
	//		//		} main_layout->add_widget(first_layout1);

	//		//		auto second_layout{ new null::gui::c_layout{ } }; {
	//		//			second_layout->name = "second_layout1";
	//		//			second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//			second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//			second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//			second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		} main_layout->add_widget(second_layout);
	//		//	} layout_window->body_layout->add_widget(main_layout);
	//		//}

	//		//{
	//		//	auto main_layout{ new null::gui::c_layout{ } }; {
	//		//		main_layout->name = "main_layou1t";
	//		//		main_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		main_layout->flags |= null::gui::e_layout_flags::reverse_setup;
	//		//		main_layout->origin = rect_t::right; main_layout->direction = rect_t::left;

	//		//		//auto first_layout{ new null::gui::c_layout{ } }; {
	//		//		//	//first_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	first_layout->scroll_bar = (null::gui::c_scroll_bar*)first_layout->add_widget(new null::gui::c_scroll_bar{ });
	//		//		//	first_layout->origin = rect_t::top; first_layout->direction = rect_t::bottom;
	//		//		//	first_layout->size_data = { null::gui::c_layout::e_size_type::procentage, { 0.5f, 1.f } };

	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 1" });
	//		//		//	first_layout->add_widget(new null::gui::c_button{ "first 2" });
	//		//		//} main_layout->add_widget(first_layout);

	//		//		//auto second_layout{ new null::gui::c_layout{ } }; {
	//		//		//	second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//		//	second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		//	second_layout->add_widget(new null::gui::c_button{ "second 2" });
	//		//		//} main_layout->add_widget(second_layout);

	//		//		auto first_layout{ new null::gui::c_layout{ } }; {
	//		//			first_layout->name = "first_layout1";
	//		//			first_layout->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout->add_widget(new null::gui::c_button{ "fi" });
	//		//		} main_layout->add_widget(first_layout);

	//		//		auto first_layout1{ new null::gui::c_layout{ } }; {
	//		//			first_layout1->name = "first_layou1t";
	//		//			first_layout1->flags |= null::gui::e_container_flags::auto_size;
	//		//			first_layout1->add_widget(new null::gui::c_button{ "fi3123123" });
	//		//		} main_layout->add_widget(first_layout1);

	//		//		auto second_layout{ new null::gui::c_layout{ } }; {
	//		//			second_layout->name = "second_layout1";
	//		//			second_layout->flags |= null::gui::e_container_flags::auto_size_y;
	//		//			second_layout->origin = rect_t::top; second_layout->direction = rect_t::bottom;

	//		//			second_layout->add_widget(new null::gui::c_button{ "second 1" });
	//		//		} main_layout->add_widget(second_layout);
	//		//	} layout_window->add_widget(main_layout);
	//		//}
	//	}
	//} //null::gui::c_window::add_window(layout_window);

	//auto autosize_window = new null::gui::c_window{ "window", { 0, 100 }, { 100, 200 } }; {
	//	auto btn = new null::gui::c_button{ "gui" }; {
	//		btn->callbacks.at<null::gui::e_widget_callbacks::on_mouse_key_up>().add([autosize_window](auto) {
	//			static int h;
	//			std::string hui{ "f1" };
	//			for(int i = 0; i <= h; i++) hui.append("1");
	//			autosize_window->add_child(new null::gui::c_button{ hui });
	//			h++;
	//		});
	//	} autosize_window->add_child(btn);
	//} //null::gui::c_window::add_window(autosize_window);

	//auto test_window{ new null::gui::c_window{ "window 2", { 300, 100 }, { 200, 200 } } }; {
	//	auto group{ new null::gui::c_group{ "group", null::gui::e_layout_flags{ } } }; {
	//		group->size.y = 200;
	//		group->add_child(new null::gui::c_button{ "button" });
	//		group->add_child(new null::gui::c_color_picker{ "color picker", &clr });
	//	} test_window->add_child(group);
	//} //null::gui::c_window::add_window(test_window);
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