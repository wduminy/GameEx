// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "sneakyworld.h"

namespace sneaky {

class Arena : public SceneNode {
private:
	sf::RectangleShape m_shape{{120,50}};
public:
	Arena() {
		m_shape.setSize({10,10});
		m_shape.setFillColor(sf::Color(150,150,150));
	}
protected:
	void draw_node(sf::RenderTarget& target, sf::RenderStates state) const override {
		target.draw(m_shape);
	}
};

SneakyWorld::SneakyWorld() : m_scene_graph(), m_arena(new Arena()) {
	m_scene_graph.attach(SceneNode::u_ptr(m_arena));
}

SneakyGame::SneakyGame() : Game(800,600,"Sneaky") {}

void SneakyGame::run() {
	Game::run();
}
void SneakyGame::update(codespear::FrameTime step) {

}
void SneakyGame::draw(sf::RenderTarget &window) {
	m_world.scene().draw(window,m_rstate);
}
}
