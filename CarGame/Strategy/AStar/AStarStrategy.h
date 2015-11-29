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

	// ����������, � ����� ����������� ����� ���������� �� ������������ ��������
	int GetNextStep();

private:
	// ����������� ���������� �� ������ �� ����������� �������
	void calculateDistancesToFinish();
	// ���� ����� A*
	void findPath( const PlayerState& initState );
	// ������� ���������
	int calculateHeuristic( const PlayerState& state, const PlayerState& nextState );

	void reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom, 
		const PlayerState& goal );

	// ������� ���� ��������� � ����� �� ����������, ��������������� ����� �����������
	int getMovementDirection( int dx, int dy );
	
	// ���������� �� ������ �� ������
	std::vector<std::vector<int>> distancesToFinish;
	std::stack<std::pair<int, int>> optimalPath;

	Map map;

	static const int maxPathLenght;
};