#pragma once
#include <utility>
#include <stack>
#include <unordered_map>

#include "../CMap.hpp"
#include "../CPlayerState.hpp"
#include "../IStrategy.h"

class CAStarStrategy : public IStrategy {
public:
	CAStarStrategy( const Map& map, const PlayerState& initState, int lapsCount, bool isSequential );

	// Возвращает, в каком направлении нужно сдвинуться по оптимальному маршруту
	int GetNextStep( std::vector<std::shared_ptr<IPlayerState>>& players, int playerNumber, std::vector<Powerup>& powerups );
private:
	// Предподсчет расстояний до финиша по чебышевской метрике
	void calculateDistancesToFinish();
	// Ищет пусть A*
	bool findPath( const PlayerState& initState );
	// Подсчет эвристики
	int calculateHeuristic( const PlayerState& state, const PlayerState& nextState );

	void reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom, 
		const PlayerState& goal );

	// Перевод пары координат в цифры на клавиатуре, соответствующую этому направлению
	int getMovementDirection( int dx, int dy );
	
	// Не стоит ли на клетке плохой поверап или другой игрок
	bool isCellOccupied( const PlayerState& curState, const std::vector<std::shared_ptr<IPlayerState>>& players, int curPlayerNumber, 
		const std::vector<Powerup>& powerups );

	// Расстояние до финиша из клетки
	std::vector<std::vector<int>> distancesToFinish;
	std::vector<std::pair<int, int>> optimalPath;
	int pathPosition = 0;

	Map map;
	int lapsCount;
	int lapMaxLenght = 0;

	PlayerState curState;

	bool isSequential;

	static const int maxPathLenght;
};