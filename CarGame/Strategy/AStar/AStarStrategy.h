#pragma once
#include <utility>
#include <stack>
#include <unordered_map>

#include "../CMap.hpp"
#include "../CPlayerState.hpp"

class CAStarStrategy {
public:
	CAStarStrategy( const Map& map, const PlayerState& initState );

	// Возвращает, в каком направлении нужно сдвинуться по оптимальному маршруту
	std::pair<int, int> GetNextStep();

private:
	// Предподсчет расстояний до финиша по чебышевской метрике
	void calculateDistancesToFinish();
	// Ищет пусть A*
	void findPath( const PlayerState& initState );
	// Подсчет эвристики
	int calculateHeuristic( const PlayerState& state, const PlayerState& nextState );

	void reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom, 
		const PlayerState& goal );
	
	// Расстояние до финиша из клетки
	std::vector<std::vector<int>> distancesToFinish;
	std::stack<std::pair<int, int>> optimalPath;

	Map map;

	static const int maxPathLenght;
};