#include <iostream>
#include <null-gui.h>
#include <null-renderer-directx9.h>

null::renderer::c_window window{ };

void main_loop() {
	null::input::begin_frame(window);
	null::render::begin_frame(window); {
		null::gui::begin_frame(); {
			if(null::gui::begin_window("style window", { 60, 20 }, { 600, 0 }, null::gui::e_window_flags{ null::gui::e_window_flags::set_size | null::gui::e_window_flags::auto_size })) {
				null::gui::begin_columns(3); {
					null::gui::begin_group("global", vec2_t{ 0.f, 100.f }); {
						null::gui::colorpicker("main_color##global", &null::gui::style::main_color);
						null::gui::colorpicker("text##global", &null::gui::style::text);
						null::gui::colorpicker("text_hovered##global", &null::gui::style::text_hovered);
						null::gui::slider_float("text_spacing##global", &null::gui::style::text_spacing, 1, 20, "{:.0f}");
						null::gui::slider_float("new_line_size##global", &null::gui::style::new_line_size, 1, 50, "{:.0f}");
						null::gui::checkbox("items_size_full_window##global", &null::gui::style::items_size_full_window);
					} null::gui::end_group();
					
					null::gui::begin_group("button", vec2_t{ 0.f, 100.f }); {
						null::gui::colorpicker("box##button", &null::gui::style::button::box);
						null::gui::colorpicker("box_hovered##button", &null::gui::style::button::box_hovered);
						null::gui::colorpicker("box_active##button", &null::gui::style::button::box_active);
						null::gui::slider_float("rounding##button", &null::gui::style::button::rounding, 0.f, 50.f);

						null::gui::text("padding##button"); {
							null::gui::begin_columns(2); {
								null::gui::slider_float("x##button padding", &null::gui::style::button::padding.x, 0.f, 50.f);
								null::gui::next_column();
								null::gui::slider_float("y##button padding", &null::gui::style::button::padding.y, 0.f, 50.f);
							} null::gui::end_columns();
						}
					} null::gui::end_group();
					
					null::gui::begin_group("checkbox", vec2_t{ 0.f, 100.f }); {
						null::gui::slider_float("box_size##checkbox", &null::gui::style::checkbox::box_size, 0.f, 50.f);
						null::gui::slider_float("check_mark_size##checkbox", &null::gui::style::checkbox::check_mark_size, 0.f, 50.f);
						null::gui::slider_float("body_offset##checkbox", &null::gui::style::checkbox::body_offset, 0.f, 50.f);
						null::gui::slider_float("rounding##checkbox", &null::gui::style::checkbox::rounding, 0.f, 50.f);
						null::gui::checkbox("hovered_with_text##checkbox", &null::gui::style::checkbox::hovered_with_text);
					} null::gui::end_group();
					
					null::gui::begin_group("colorpicker", vec2_t{ 0.f, 100.f }); {
						null::gui::slider_float("sv_box_size##colorpicker", &null::gui::style::colorpicker::sv_box_size, 0.f, 50.f);
						null::gui::slider_float("sliders_thickness##colorpicker", &null::gui::style::colorpicker::sliders_thickness, 0.f, 50.f);
						null::gui::slider_float("rounding##colorpicker", &null::gui::style::colorpicker::rounding, 0.f, 50.f);
					} null::gui::end_group();

					null::gui::next_column();

					null::gui::begin_group("combo", vec2_t{ 0.f, 100.f }); {
						null::gui::slider_float("box_size##combo", &null::gui::style::combo::box_size, 0.f, 50.f);
						null::gui::slider_float("arrow_size##combo", &null::gui::style::combo::arrow_size, 0.f, 50.f);
						null::gui::slider_float("popup_padding##combo", &null::gui::style::combo::popup_padding, 0.f, 50.f);
						null::gui::slider_int("autosize_items##combo", &null::gui::style::combo::autosize_items, 0, 50);
						null::gui::slider_float("rounding##combo", &null::gui::style::combo::rounding, 0.f, 50.f);
					} null::gui::end_group();
					

					null::gui::begin_group("scrollbar", vec2_t{ 0.f, 100.f }); {
						null::gui::slider_float("thickness##scrollbar", &null::gui::style::scrollbar::thickness, 0.f, 50.f);
						null::gui::slider_float("rounding##scrollbar", &null::gui::style::scrollbar::rounding, 0.f, 50.f);

						null::gui::text("padding##scrollbar"); {
							null::gui::begin_columns(2); {
								null::gui::slider_float("x##scrollbar padding", &null::gui::style::scrollbar::padding.x, 0.f, 50.f);
								null::gui::next_column();
								null::gui::slider_float("y##scrollbar padding", &null::gui::style::scrollbar::padding.y, 0.f, 50.f);
							} null::gui::end_columns();
						}

						null::gui::checkbox("show_background##scrollbar", &null::gui::style::scrollbar::show_background);
					} null::gui::end_group();

					null::gui::begin_group("selectable", vec2_t{ 0.f, 86.f }); {
						null::gui::slider_float("offset##selectable", &null::gui::style::selectable::offset, 0.f, 50.f);
						null::gui::slider_float("active_offset##selectable", &null::gui::style::selectable::active_offset, 0.f, 50.f);
					} null::gui::end_group();

					null::gui::begin_group("slider", vec2_t{ 0.f, 86.f }); {
						null::gui::slider_float("thickness##slider", &null::gui::style::slider::thickness, 0.f, 50.f);
						null::gui::slider_float("rounding##slider", &null::gui::style::slider::rounding, 0.f, 50.f);
					} null::gui::end_group();

					null::gui::next_column();

					null::gui::begin_group("text_input", vec2_t{ 0.f, 100.f }); {
						null::gui::colorpicker("select_text##text_input", &null::gui::style::text_input::select_text);
						null::gui::slider_float("line_size##text_input", &null::gui::style::text_input::line_size, 0.f, 50.f);
						null::gui::slider_float("rounding##text_input", &null::gui::style::text_input::rounding, 0.f, 50.f);

						null::gui::text("padding##text_input"); {
							null::gui::begin_columns(2); {
								null::gui::slider_float("x##text_input padding", &null::gui::style::text_input::padding.x, 0, 50);
								null::gui::next_column();
								null::gui::slider_float("y##text_input padding", &null::gui::style::text_input::padding.y, 0, 50);
							} null::gui::end_columns();
						}
					} null::gui::end_group();

					null::gui::begin_group("window", vec2_t{ 0.f, 100.f }); {
						null::gui::colorpicker("background##window", &null::gui::style::window::background);
						null::gui::slider_float("item_spacing##window", &null::gui::style::window::item_spacing, 0.f, 50.f);
						null::gui::slider_float("rounding##window", &null::gui::style::window::rounding, 0.f, 50.f);

						null::gui::text("padding##window"); {
							null::gui::begin_columns(2); {
								null::gui::slider_float("x##window padding", &null::gui::style::window::padding.x, 0.f, 50.f);
								null::gui::next_column();
								null::gui::slider_float("y##window padding", &null::gui::style::window::padding.y, 0.f, 50.f);
							} null::gui::end_columns();
						}

						null::gui::checkbox("clamp_on_screen##window", &null::gui::style::window::clamp_on_screen);
						null::gui::checkbox("move_on_titlebar##window", &null::gui::style::window::move_on_titlebar);
					} null::gui::end_group();

					null::gui::begin_group("titlebar", vec2_t{ 0.f, 100.f }); {
						null::gui::colorpicker("background##titlebar", &null::gui::style::window::titlebar::background);
						null::gui::slider_float("size##titlebar", &null::gui::style::window::titlebar::size, 0.f, 50.f);
						null::gui::slider_float("line_size##titlebar", &null::gui::style::window::titlebar::line_size, 0.f, 50.f);
						null::gui::slider_float("rounding##titlebar", &null::gui::style::window::titlebar::rounding, 0.f, 50.f);
					} null::gui::end_group();

					null::gui::begin_group("group", vec2_t{ 0.f, 53.f }); {
						null::gui::colorpicker("background##group", &null::gui::style::group::background);
					} null::gui::end_group();

					null::gui::begin_group("popup", vec2_t{ 0.f, 53.f }); {
						null::gui::colorpicker("background##popup", &null::gui::style::popup::background);
					} null::gui::end_group();
				} null::gui::end_columns();
				null::gui::end_window();
			}

			null::render::background.draw_text(std::format("[ directx9 ] fps: {:3.0f}", 1.f / window.time_data.delta_time), { window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });
		} null::gui::end_frame();
	} null::render::end_frame();
}

int main(HINSTANCE instance) {
	window = null::renderer::c_window{ instance };

	window.callbacks.add<void()>(utils::win::e_window_callbacks::on_main_loop, main_loop);
	window.callbacks.add<int(HWND, UINT, WPARAM, LPARAM)>(utils::win::e_window_callbacks::on_wnd_proc, null::input::wnd_proc);
	window.callbacks.add<int(HWND, UINT, WPARAM, LPARAM)>(utils::win::e_window_callbacks::on_wnd_proc, null::gui::wnd_proc);

	try {
		null::render::atlas.add_font_from_file_ttf("C:\\Windows\\fonts\\Tahoma.ttf", 13.f, nullptr, null::render::c_font::glyph_t::ranges_cyrillic());

		window.create();

		window.main_loop();
		window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}