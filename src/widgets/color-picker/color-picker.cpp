#include <null-gui.h>

namespace null::gui {
	void c_color_picker::c_sv_box::setup() {
		size = style.size;
		i_widget::setup();
	}

	void c_color_picker::c_sv_box::draw() {
		color_t<int> color{ hsv_color_t{ hsv->h() } };
		gui_layer.draw_rect_filled_multicolor(pos, pos + size, { color_t<int>::palette_t::white, color, color_t<int>{ 255, 0 }, color_t<int>{ color, 0 } });
		gui_layer.draw_rect_filled_multicolor(pos, pos + size, { 0, 0, color_t<int>::palette_t::black, color_t<int>::palette_t::black });

		vec2_t dot_pos{ math::round(pos + vec2_t{ hsv->s(), 1.f - hsv->v() } * size) };
		gui_layer.draw_circle_filled(dot_pos, { 45, 200 }, 5.f);
		gui_layer.draw_circle_filled(dot_pos, { color_t<float>{ *hsv }, 255 }, 3.f);

		i_widget::draw();
	}

	void c_color_picker::c_sv_box::on_mouse_move() {
		if(state & e_widget_state::active) {
			vec2_t sv{ (input::mouse.pos - pos) / style.size };
			hsv->s() = std::clamp(sv.x, 0.f, 1.f);
			hsv->v() = std::clamp(1.f - sv.y, 0.f, 1.f);
		}

		i_widget::on_mouse_move();
	}

	void c_color_picker::c_h_bar::setup() {
		size = { style.thickness, node.parent->working_region.size().y };
		i_widget::setup();
	}

	void c_color_picker::c_h_bar::draw() {
		vec2_t segment_size{ size.x, size.y / 6.f };
		for(const int& segment : std::views::iota(0, 6)) {
			color_t<int> hsv{ hsv_color_t{ segment * 60.f } }, next_hsv{ hsv_color_t{ (segment + 1) * 60.f } };
			rect_t segment_rect{ pos + vec2_t{ 0.f, segment_size.y * segment }, segment_size, rect_t::top | rect_t::left };
			gui_layer.draw_rect_filled_multicolor(segment == 0 || segment == 5 ? math::round(segment_rect) : segment_rect, { hsv, hsv, next_hsv, next_hsv });
		}

		float indicator_pos{ std::round(pos.y + (hsv->h() / 360.f) * size.y) };
		gui_layer.draw_rect_filled(vec2_t{ pos.x, indicator_pos } - vec2_t{ 2, 1 }, vec2_t{ pos.x + size.x, indicator_pos } + vec2_t{ 2, 1 }, { });

		i_widget::draw();
	}

	void c_color_picker::c_h_bar::on_mouse_move() {
		if(state & e_widget_state::active) hsv->h() = std::clamp((input::mouse.pos.y - pos.y) / size.y, 0.f, 1.f) * 360.f;
		i_widget::on_mouse_move();
	}

	void c_color_picker::c_alpha_bar::setup() {
		size = { node.parent->working_region.size().x, style.thickness };
		i_widget::setup();
	}

	void c_color_picker::c_alpha_bar::draw() {
		float indicator_pos{ std::round(pos.x + hsv->a() * size.x) };
		color_t<int> rgba{ color_t<float>{ *hsv }, 255 };
		gui_layer.draw_rect_filled_multicolor(pos, pos + size, { color_t<int>::palette_t::white, rgba, color_t<int>::palette_t::white, rgba });
		gui_layer.draw_rect_filled(vec2_t{ indicator_pos, pos.y } - vec2_t{ 1.f, 2.f }, vec2_t{ indicator_pos, pos.y + size.y } + vec2_t{ 1.f, 2.f }, { });

		i_widget::draw();
	}

	void c_color_picker::c_alpha_bar::on_mouse_move() {
		if(state & e_widget_state::active) hsv->a() = std::clamp((input::mouse.pos.x - pos.x) / size.x, 0.f, 1.f);
		i_widget::on_mouse_move();
	}

	bool c_color_picker::can_hovered() {
		if(!i_widget::can_hovered()) return false;
		return box_region.contains(input::mouse.pos);
	}

	void c_color_picker::setup() {
		size = render::c_font::get_current_font()->calc_text_size(name) + vec2_t{ style.text_offset + style.box_size.x, 0.f };
		size = math::max(size, vec2_t{ node.parent->working_region.size().x, style.box_size.y });

		working_region = rect_t{ vec2_t{ 0.f }, style.box_size };
		box_region = working_region + pos;
		i_widget::setup();
	}

	void c_color_picker::draw() {
		color_t field_color{ style.default_color };
		if(state & e_widget_state::active) field_color = style.active_color;
		else if(state & e_widget_state::hovered) field_color = style.hovered_color;

		gui_layer.draw_text(name, box_region.origin(rect_t::center) + vec2_t{ box_region.size().x / 2.f + style.text_offset, 0.f }, style.text_color, render::e_text_flags::aligin_center_y);
		gui_layer.draw_rect_filled(box_region, *color);

		i_widget::draw();
	}

	void c_color_picker::on_child_in_node_event_handled(i_widget* child) {
		*color = color_t<int>{ hsv };
		i_widget::on_child_in_node_event_handled(child);
	}

	void c_color_picker::on_mouse_key_up(const input::e_key_id& key) {
		if(!(popup->flags & e_widget_flags::visible)) {
			hsv = hsv_color_t{ *color };

			on_lost_focus(popup);
			popup->pos = vec2_t{ working_region.max.x, working_region.min.y } + style.picker_popup_offset + pos;
			popup->open();
			popup->on_focused();
		} else {
			popup->close();
		}
		i_widget::on_mouse_key_up(key);
	}
}