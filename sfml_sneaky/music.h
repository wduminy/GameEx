// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include "types.h"

namespace codespear {

// TOD 060 Implement music player
class MusicPlayer {
public:
	static MusicPlayer instance;
	void play(const string& filename);
	void toggle();
private:
	MusicPlayer(){}
};

}
