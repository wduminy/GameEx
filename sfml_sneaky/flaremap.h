// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <string>
#include <map>
#include <vector>

namespace codespear {
using FlareMapLayer = std::vector<int>;
using LayerMap = std::map<std::string,FlareMapLayer>;

/**
 * Encapsulates the flare map text file produced by Tiled.
 */
class FlareMap {
public:
private:
	LayerMap m_layers;
public:
	FlareMap(const std::string& file_name);
	const LayerMap& layer() const {return m_layers;}
};

}
