#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <string>
#include <vector>

struct OrientationConfig {
	std::string type;
	std::vector<std::uint8_t> value;
};

#endif
