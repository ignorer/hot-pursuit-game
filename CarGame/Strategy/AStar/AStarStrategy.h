#pragma once
#include <utility>
#include <stack>
#include <unordered_map>

#include "../CMap.hpp"
#include "../CPlayerState.hpp"

class CAStarStrategy {
public:
	CAStarStrategy( const Map& map, const PlayerState& initState );

	// ����������, � ����� ����������� ����� ���������� �� ������������ ��������
	std::pair<int, int> GetNextStep();

private:
	// ����������� ���������� �� ������ �� ����������� �������
	void calculateDistancesToFinish();
	// ���� ����� A*
	void findPath( const PlayerState& initState );
	// ������� ���������
	int calculateHeuristic( const PlayerState& state, const PlayerState& nextState );

	void reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom, 
		const PlayerState& goal );
	
	// ���������� �� ������ �� ������
	std::vector<std::vector<int>> distancesToFinish;
	std::stack<std::pair<int, int>> optimalPath;

	Map map;

	static const int maxPathLenght;
};