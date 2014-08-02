// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <functional>
namespace codespear {

using WidgetCallBack = std::function<void()>;

class Widget :  public sf::Drawable, public sf::Transformable {
private:
	bool m_selected{false};
	bool m_active{false};
	WidgetCallBack m_on_activate;
public:
	using s_ptr = std::shared_ptr<Widget>;
	virtual void handle_event(const sf::Event& e) {};
	virtual bool is_selectable() const = 0;
	bool is_selected() const {return m_selected;}
	bool is_active() const {return m_active;}
	void set_on_activate(WidgetCallBack v) {m_on_activate = v;}
	virtual void set_active(bool v);
	virtual void set_selected(bool v) {m_selected = v;}
	using sf::Drawable::draw;
};

class Panel : public Widget {
private:
	std::vector<Widget::s_ptr> m_children;
	size_t m_selected_child = -1U;
public:
	bool is_selectable() const final {return false;}
	void handle_event(const sf::Event& e);
	void add(Widget::s_ptr w);
	bool has_selection() const {return m_selected_child != -1U;}
	virtual void select(size_t child_index);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	Widget::s_ptr operator+=(Widget * c) {
		auto r = s_ptr(c);
		add(r);
		return r;
	}
};

struct WidgetSkin {
	sf::Font  * font = nullptr;
	sf::Texture * texture = nullptr;
	sf::IntRect button_normal, button_selected, button_pressed;
	float button_scale{1.f};
	static WidgetSkin instance;
};

class Label : public Widget {
private:
	sf::Text m_text;
public:
	Label(const sf::Font& font,const std::string& txt) : m_text(txt,font) {}
	Label(const std::string& txt);
	void set_text(const std::string& txt);
	bool is_selectable() const final {return false;}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
};

class Button : public Widget {
private:
	sf::Text m_text;
	sf::Sprite m_sprite;
	sf::IntRect m_norm_r;
	sf::IntRect m_sel_r;
	sf::IntRect m_pressed_r;
public:
	Button(const sf::Font& font,const std::string& txt,
			const sf::Texture& tex, const sf::IntRect& norm_rect,
			const sf::IntRect& sel_rect, const sf::IntRect& press_rect, const float scale)
			: m_text(txt,font),
			  m_sprite(tex,norm_rect),
			  m_norm_r(norm_rect),
			  m_sel_r(sel_rect),
			  m_pressed_r(press_rect)	{
		m_sprite.setScale({scale,scale});
		const auto offsetx = (norm_rect.width*scale - m_text.getCharacterSize()*txt.length()/2.5f)/2.f;
		const auto offsety = (norm_rect.height*scale - 1.5f*m_text.getCharacterSize())/2.f;
		m_text.setPosition(offsetx,offsety);
	}
	Button(const std::string& txt, const sf::Vector2f& p, WidgetCallBack on_activate);

	bool is_selectable() const final {return true;}

	void set_selected(bool v) override {
		const auto &r = v?m_sel_r:m_norm_r;
		m_sprite.setTextureRect(r);
	}

	void set_active(bool v) override {
		const auto&r = v?(is_selected()?m_sel_r:m_norm_r):m_pressed_r;
		m_sprite.setTextureRect(r);
		Widget::set_active(v);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
};
}
