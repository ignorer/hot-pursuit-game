#pragma once
#include <utility>
#include <stack>
#include <unordered_map>

#include "../CMap.hpp"
#include "../CPlayerState.hpp"
#include "../IStrategy.h"

class CAStarStrategy : public IStrategy {
public:
	CAStarStrategy( const Map& map, const PlayerState& initState );

	// Возвращает, в каком направлении нужно сдвинуться по оптимальному маршруту
	int GetNextStep();

private:
	// Предподсчет расстояний до финиша по чебышевской метрике
	void calculateDistancesToFinish();
	// Ищет пусть A*
	void findPath( const PlayerState& initState );
	// Подсчет эвристики
	int calculateHeuristic( const PlayerState& state, const PlayerState& nextState );

	void reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom, 
		const PlayerState& goal );

	// Перевод пары координат в цифры на клавиатуре, соответствующую этому направлению
	int getMovementDirection( int dx, int dy );
	
	// Расстояние до финиша из клетки
	std::vector<std::vector<int>> distancesToFinish;
	std::vector<std::pair<int, int>> optimalPath;
	int pathPosition = 0;

	PlayerState finalState;
	std::unordered_map<PlayerState, int, PlayerState::CPlayerStateHasher> fScore; // Функция текущей стоимости + эвристика

	Map map;

	static const int maxPathLenght;
};