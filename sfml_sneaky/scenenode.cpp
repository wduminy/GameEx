// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "scenenode.h"
#include "exception.h"
#include <algorithm>
namespace codespear {
using std::find_if;

SceneNode::SceneNode() : m_parent(nullptr) {}

void SceneNode::attach(u_ptr child) {
	ASSERT(child);
	ASSERT(child->m_parent == nullptr);
	child->m_parent = this;
	m_children.emplace_back(std::move(child));
}

SceneNode::u_ptr SceneNode::detach(SceneNode * child) {
	auto fnd = find_if(m_children.begin(), m_children.end(),
			[&child](SceneNode::u_ptr &e){return e.get() == child;});
	ASSERT(fnd != m_children.end());
	child->m_parent = nullptr;
	SceneNode::u_ptr result = std::move(*fnd);
	m_children.erase(fnd);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	state.transform *= getTransform();
	draw_node(target,state);
	for(auto &e : m_children) e->draw(target,state);
}

void SpriteNode::draw_node(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(m_sprite,state);
}

sf::Transform SceneNode::world_transform() const {
	sf::Transform result;
	for (const SceneNode* n = this; n != nullptr; n = n->m_parent)
		result = n->getTransform() * result;
	return result;
}


SpriteNode::SpriteNode(const sf::Texture& tex, const sf::IntRect& rect, const float scale) {
	m_sprite.setTexture(tex,false);
	m_sprite.setTextureRect(rect);
	m_sprite.setScale({scale,scale});
}

}
