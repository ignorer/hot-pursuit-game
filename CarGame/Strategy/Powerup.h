#pragma once
#include <utility>

enum PowerupType {
	WALL, SAND, OIL, MINE, MINE_ACTIVE, LAZER, SHIELD
};

struct Powerup {
	PowerupType type;
	std::pair<int, int> coordinate;
};