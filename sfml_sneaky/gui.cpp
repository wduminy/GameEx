// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "gui.h"
#include "exception.h"

namespace codespear {
WidgetSkin WidgetSkin::instance;
void Widget::set_active(bool v) {
	if (v != m_active) {
		m_active = v;
		if (m_active && m_on_activate)
			m_on_activate();
	}
}
void Panel::handle_event(const sf::Event& e) {
	if (has_selection() && m_children[m_selected_child]->is_active())
		m_children[m_selected_child]->handle_event(e);
	else if (e.type == sf::Event::KeyPressed) {
		if (e.key.code == sf::Keyboard::Up)
			select(m_selected_child-1);
		else if (e.key.code == sf::Keyboard::Down)
			select(m_selected_child+1);
		else if (e.key.code == sf::Keyboard::Return)
			if (has_selection())
				m_children[m_selected_child]->set_active(true);
	}
}

void Panel::add(Widget::s_ptr w) {
	m_children.push_back(w);
	if (!has_selection() && w->is_selectable())
		select(m_children.size()-1);
}

void Panel::select(size_t ix) {
	if (has_selection())
		m_children.at(m_selected_child)->set_selected(false);
	if (ix == -1U)
		ix = m_children.size()-1;
	else if (ix >= m_children.size())
		ix = 0;
	m_selected_child = ix;
	m_children.at(ix)->set_selected(true);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	for (auto w : m_children) w->draw(target,states);
}

WidgetSkin& skin() {
	ASSERT(WidgetSkin::instance.font != nullptr);
	ASSERT(WidgetSkin::instance.texture != nullptr);
	return WidgetSkin::instance;
}

Label::Label(const std::string& t) : Label(*skin().font,t) {}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_text,states);
}

Button::Button(const std::string& t, const sf::Vector2f& p, WidgetCallBack on_activate) : Button(
		*skin().font,t,
		*skin().texture,
		skin().button_normal, skin().button_selected, skin().button_pressed,
		skin().button_scale) {
	setPosition(p);
	set_on_activate(on_activate);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_sprite,states);
	target.draw(m_text,states);
}
}
