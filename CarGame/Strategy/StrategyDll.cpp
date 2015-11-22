#pragma once
#include "StrategyDll.h"
#include "IPlayerState.h"
#include "IMap.h"
#include "CPlayerState.hpp"
#include "CMap.hpp"
#include "AStar/AStarStrategy.h"

#include <time.h>

enum class EMovementDirection : int {
	UP_LEFT = 7, UP = 8, UP_RIGHT = 9,
	LEFT = 4, NOT_CHANGED = 5, RIGHT = 6,
	DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3,
	NONE = 10
};

Map PutPlayersOnMap(const Map &_map, const std::vector< std::shared_ptr<IPlayerState> > &_playerStateList, int CurrentPlayerIndex) {
	int xSize = _map.size().first;
	int ySize = _map.size().second;
	Map res(xSize, ySize);
	for (int x = 0; x < xSize; ++x)
		for (int y = 0; y < ySize; ++y)
		{
			int cellValue = _map[x][y];
			if (cellValue == 0)
			{
				for (int p = 0; p < _playerStateList.size(); ++p)
				{
					if (p == CurrentPlayerIndex)
						continue;
					if (_playerStateList[p]->GetX() == x && _playerStateList[p]->GetX() == y)
					{
						cellValue = 1;
						break;
					};
				};
			};
			res[x][y] = cellValue;
		};
	return res;
}

EMovementDirection GetMovementDirection(int dx, int dy) {
	if (dx == 0 && dy == 0) {
		return EMovementDirection::NOT_CHANGED;
	}
	else if (dx == -1 && dy == 0) {
		return EMovementDirection::UP;
	}
	else if (dx == -1 && dy == 1) {
		return EMovementDirection::UP_RIGHT;
	}
	else if (dx == 0 && dy == 1) {
		return EMovementDirection::RIGHT;
	}
	else if (dx == 1 && dy == 1) {
		return EMovementDirection::DOWN_RIGHT;
	}
	else if (dx == 1 && dy == 0) {
		return EMovementDirection::DOWN;
	}
	else if (dx == 1 && dy == -1) {
		return EMovementDirection::DOWN_LEFT;
	}
	else if (dx == 0 && dy == -1) {
		return EMovementDirection::LEFT;
	}
	else if (dx == -1 && dy == -1) {
		return EMovementDirection::UP_LEFT;
	}
}

IPlayerState* GetPlayerState(int x, int y, int xVelocity, int yVelocity) {
	return new PlayerState(x, y, xVelocity, yVelocity);
}

IMap* GetDefaultMap() {
	return new Map();
}

int AStarStrategyFunc( const Map &map, const PlayerState &currentPlayer )
{
	static CAStarStrategy strategy( map, currentPlayer );
	auto step = strategy.GetNextStep();
	return (int) GetMovementDirection( step.first, step.second );
}

int StrategyFunc( const std::vector< std::vector < int > > &inputCells,
	const std::pair< int, int > &_leftFinishPoint,
	const std::pair< int, int > &_rightFinishPoint, 
	std::shared_ptr<IPlayerState> _playerState )
{
	Map map( inputCells, _leftFinishPoint, _rightFinishPoint );
	const PlayerState currentPlayer = *( std::dynamic_pointer_cast<PlayerState>(_playerState) );
	return AStarStrategyFunc( map, currentPlayer );
};
