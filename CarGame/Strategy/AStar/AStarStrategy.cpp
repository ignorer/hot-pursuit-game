#include "AStarStrategy.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>
#include <queue>
#include <cmath>

const int CAStarStrategy::maxPathLenght = std::numeric_limits<int>::max();

CAStarStrategy::CAStarStrategy( const Map& _map, const PlayerState& currentPlayer ) :
	map( _map )
{
	calculateDistancesToFinish();
	findPath( currentPlayer );
}

std::pair<int, int> CAStarStrategy::GetNextStep()
{
	std::pair<int, int> top = optimalPath.top();
	optimalPath.pop();
	return top;
}

void CAStarStrategy::calculateDistancesToFinish()
{
	// ������ ������� �����
	distancesToFinish.resize( map.sizeOnYaxis() );
	for( int i = 0; i < distancesToFinish.size(); ++i ) {
		distancesToFinish[i].resize( map.sizeOnXaxis(), -1 );
	}

	std::queue< std::pair<int, int> > bfsQueue;
	auto leftFinishPoint = map.GetLeftFinishPoint();
	auto rightFinishPoint = map.GetRightFinishPoint();

	if( leftFinishPoint.second == rightFinishPoint.second ) {
		for( int i = leftFinishPoint.first; i <= rightFinishPoint.first; ++i ) {
			if( map.canPlayerStayOnCell( i, leftFinishPoint.second + 1 ) ) {
				bfsQueue.push( std::make_pair( i, leftFinishPoint.second + 1 ) );
			}
			if( map.canPlayerStayOnCell( i - 1, leftFinishPoint.second + 1 ) ) {
				bfsQueue.push( std::make_pair( i - 1, leftFinishPoint.second + 1 ) );
			}
			if( map.canPlayerStayOnCell( i + 1, leftFinishPoint.second + 1 ) ) {
				bfsQueue.push( std::make_pair( i + 1, leftFinishPoint.second + 1 ) );
			}
			distancesToFinish[leftFinishPoint.second][i] = 0;
		}
	}
	while( !bfsQueue.empty() ) {
		auto curNode = bfsQueue.front();
		bfsQueue.pop();

		if( distancesToFinish[curNode.second][curNode.first] == -1 ) {
			int minDistance = maxPathLenght; // ���� ����������� �������� ���������� �� ������ ����� �������
			for( int i = -1; i <= 1; ++i ) {
				for( int j = -1; j <= 1; ++j ) {
					// ���� �� ������ ����� ������, � ��� �� �������� � ��� �� ��������� � ������ ������� �� ������ - ���� � ��
					if( map.canPlayerStayOnCell( curNode.first + i, curNode.second + j )
						&& distancesToFinish[curNode.second + j][curNode.first + i] == -1
						&& !map.IsOnTheFinishLine( curNode.first, curNode.second, curNode.first + i, curNode.second + j ) )
					{
						bfsQueue.push( std::make_pair( curNode.first + i, curNode.second + j ) );
					} else if( map.canPlayerStayOnCell( curNode.first + i, curNode.second + j ) 
						&& distancesToFinish[curNode.second + j][curNode.first + i] != -1
						&& !map.IsOnTheFinishLine( curNode.first + i, curNode.second + j, curNode.first, curNode.second ) )
					{ 
						// ��� ���������� �������
						minDistance = std::min( distancesToFinish[curNode.second + j][curNode.first + i], minDistance );
					}
				}
			}
			distancesToFinish[curNode.second][curNode.first] = ++minDistance;
		}
	}
}

void CAStarStrategy::findPath( const PlayerState& initState )
{
	std::unordered_set<PlayerState, PlayerState::CPlayerStateHasher> closedSet; // ��� ������������� �������
	std::set< std::pair<int, PlayerState> > openSetVal;	// �������, ������� ���������� �����������, ��������������� �� ����������� fScore
	std::unordered_set<PlayerState, PlayerState::CPlayerStateHasher> openSet; // �������, ������� ���������� �����������, �� �����������
	std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher> cameFrom; // ��������� �������
	std::unordered_map<PlayerState, int, PlayerState::CPlayerStateHasher> gScore; // ������� ������� ��������� + ���������
	std::unordered_map<PlayerState, int, PlayerState::CPlayerStateHasher> fScore; // ������� ������� ���������

	gScore[initState] = 0;
	fScore[initState] = gScore[initState] + calculateHeuristic( initState, initState );
	openSetVal.insert( std::make_pair( fScore[initState], initState ) );
	openSet.insert( initState );
	cameFrom[initState] = initState;

	while( !openSet.empty() ) {
		PlayerState currentState = openSetVal.begin()->second;
		if( map.intersectFinishLine( cameFrom[currentState].GetX(), cameFrom[currentState].GetY(),
			currentState.GetX(), currentState.GetY())
			&& !map.wrongFinishLineIntersection( cameFrom[currentState].GetX(), cameFrom[currentState].GetY(),
			currentState.GetX(), currentState.GetY() ) )
		{
			reconstructPath( cameFrom, currentState );
			return;
		}
		openSet.erase( currentState );
		openSetVal.erase( openSetVal.begin() );
		closedSet.insert( currentState );

		for( int i = -1; i <= 1; ++i ) {
			for( int j = -1; j <= 1; ++j ) {
				if( map.canPlayerStayOnCell( currentState.GetX() + currentState.GetXVelocity() + i,
					currentState.GetY() + currentState.GetYVelocity() + j ) ) 
				{
					PlayerState neighborState = currentState.GetNextState( i, j );

					if( closedSet.find( neighborState ) != closedSet.end() ) {
						continue;
					}

					int tentGScore = gScore[currentState] + 1;
					bool tentIsBetter; // ������������ ��, ��� ���� �����

					if( openSet.find( neighborState ) == openSet.end( ) ) {
						tentIsBetter = true;
					} else {
						if( tentGScore < gScore[neighborState] ) {
							tentIsBetter = true;
						} else {
							tentIsBetter = false;
						}
					}

					if( tentIsBetter ) {
						cameFrom[neighborState] = currentState;
						gScore[neighborState] = tentGScore;
						int heuristic = calculateHeuristic( currentState, neighborState );
						if( heuristic != std::numeric_limits<int>::max() ) {
							fScore[neighborState] = gScore[neighborState] + heuristic / 2;
							openSet.insert( neighborState );
							openSetVal.insert( std::make_pair( fScore[neighborState], neighborState ) );
						}
					}
				}
			}
		}
	}
}

int CAStarStrategy::calculateHeuristic( const PlayerState& state, const PlayerState& nextState )
{
	if( map.hasBarrierOnPath( state.GetX(), state.GetY(), nextState.GetX(), nextState.GetY() ) ) {
		return std::numeric_limits<int>::max();
	}
	if( distancesToFinish[nextState.GetY()][nextState.GetX()] == -1 ) {
		return std::numeric_limits<int>::max();
	}
	int velocity = (int) std::sqrt( nextState.GetXVelocity() * nextState.GetXVelocity() + 
		nextState.GetYVelocity() * nextState.GetYVelocity() );
	return distancesToFinish[nextState.GetY()][nextState.GetX()];
}

void CAStarStrategy::reconstructPath( std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher>& cameFrom,
	const PlayerState& goal )
{
	PlayerState curr = goal;
	for( ; cameFrom[curr] != curr; ) {
		auto next = cameFrom[curr];

		auto prevVelocity = next.getVelocityVector();
		auto prevPosition = next.getPosition();
		prevPosition.first += prevVelocity.first;
		prevPosition.second += prevVelocity.second;
		optimalPath.push( std::make_pair( (curr.getPosition().second - prevPosition.second),
			(curr.getPosition().first - prevPosition.first) ) );

		curr = next;
	}
}
