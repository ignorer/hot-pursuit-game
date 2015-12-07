#pragma once
#include "StrategyDll.h"
#include "IPlayerState.h"
#include "IMap.h"
#include "CPlayerState.hpp"
#include "CMap.hpp"
#include "AStar/AStarStrategy.h"

#include <time.h>

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

IPlayerState* GetPlayerState( int x, int y, int xVelocity, int yVelocity, int curLap ) {
	return new PlayerState( x, y, xVelocity, yVelocity, curLap );
}

IStrategy* GetNewStrategy(
	const std::vector< std::vector < int > > &inputCells,
	const std::pair< int, int > &_leftFinishPoint,
	const std::pair< int, int > &_rightFinishPoint,
	int lapsCount,
	std::shared_ptr<IPlayerState> playerState,
	bool isSequential )
{
	Map map( inputCells, _leftFinishPoint, _rightFinishPoint );
	const PlayerState currentPlayer = *(std::dynamic_pointer_cast<PlayerState>(playerState));

	return new CAStarStrategy( map, currentPlayer, lapsCount, isSequential );
}