// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "flaremap.h"
#include "exception.h"
#include <fstream>
#include <sstream>

namespace codespear {
FlareMap::FlareMap(const std::string& file_name) {
	std::ifstream file(file_name.c_str());
	if (!file)
		THROW_STREAM("file does not exist:" << file_name);
	string word;
	while (file.good()) {
		while (file.good() && word != "[layer]")
			file >> word;
		if (!file.good())
			break;
		file >> word;
		auto layer_name = word.substr(5);
		file >> word;
		while (file.good() && word != "data=") {
			layer_name += " " + word;
			file >> word;
		}
		auto &layer = m_layers.emplace(layer_name,FlareMapLayer{}).first->second;
		int v;
		file >> word;
		while (file.good() && word != "[layer]") {
			for (char &c : word) if (c == ',') c = ' ';
			std::istringstream ss(word + " -1");
			do {
				ss >> v;
				if (v != -1)
					layer.push_back(v);
			} while (v != -1);
			file >> word;
		}
	}
}
}
