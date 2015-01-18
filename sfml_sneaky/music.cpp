// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "music.h"
#include <SFML/Audio.hpp>
#include "exception.h"


namespace codespear {
namespace {
sf::Music music;
}

MusicPlayer MusicPlayer::instance;

void MusicPlayer::play(const string& filename) {
	check_that(music.openFromFile(filename));
	music.setLoop(true);
	music.setVolume(50);
	music.play();
}

void MusicPlayer::toggle() {
	if (music.getStatus() == sf::Music::Status::Playing)
		music.pause();
	else
		music.play();
}
}
