#pragma once
#include <utility>

enum PowerupType {
	WALL, SAND, OIL, MINE, MINE_ACTIVE, LAZER, SHIELD, NONE
};

struct Powerup {
	PowerupType type;
	std::pair<int, int> coordinate;

	Powerup( PowerupType _type, std::pair<int, int>& coord ) :
		type( _type ),
		coordinate( coord )
	{}
};