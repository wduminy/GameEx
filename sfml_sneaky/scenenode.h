// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

namespace codespear {
using std::vector;
using std::unique_ptr;

class SceneNode : public sf::Drawable, public sf::Transformable, public sf::NonCopyable {
public:
	using u_ptr = unique_ptr<SceneNode>;
private:
	vector<u_ptr> m_children;
	SceneNode * m_parent;
public:
	SceneNode();
	/**
	 * The child must not have a parent
	 * @param child
	 */
	void attach(u_ptr child);
	/**
	 * The child must be part of the node
	 * @param child
	 * @return
	 */
	u_ptr detach(SceneNode * child);
	/**
	 * Draw node and children
	 * @param target
	 * @param states
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
protected:
	virtual void draw_node(sf::RenderTarget& target, sf::RenderStates states) const {}
};

}
