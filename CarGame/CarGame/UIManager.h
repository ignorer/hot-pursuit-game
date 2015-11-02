﻿#pragma once

#include <set>

#include "Core/Map.h"
#include "Core/Player.h"

class CUIManager {
public:
	int GetDirection() const;
	void InitMap( const Core::CMap& map, const std::vector<Core::CPlayer>& players ) const;
	void Move( const std::vector<Core::CPlayer>& movedPlayers ) const;
	void ShowCollisions( const std::set<Core::CPlayer>& collidedPlayers ) const;
	void ShowCrashes( const std::set<Core::CPlayer>& crashedPlayers ) const;
	void ShowWinner( const Core::CPlayer* winner ) const;
	std::vector<Core::CPlayer> GetPlayersInfo( const std::vector<Core::CCoordinates>& startPoints ) const;
	size_t GetMapName() const;
};