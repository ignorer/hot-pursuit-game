#include "AStarStrategy.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>
#include <queue>
#include <cmath>

const int CAStarStrategy::maxPathLenght = std::numeric_limits<int>::max();

enum EMovementDirection {
	UP_LEFT = 7, UP = 8, UP_RIGHT = 9,
	LEFT = 4, NOT_CHANGED = 5, RIGHT = 6,
	DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3,
	NONE = 10
};

CAStarStrategy::CAStarStrategy( const Map& _map, const PlayerState& currentPlayer ) :
	map( _map )
{
	calculateDistancesToFinish();
	findPath( currentPlayer );
}

int CAStarStrategy::getMovementDirection( int dx, int dy )
{
	if( dx == 0 && dy == 0 ) {
		return EMovementDirection::NOT_CHANGED;
	} else if( dx == -1 && dy == 0 ) {
		return EMovementDirection::UP;
	} else if( dx == -1 && dy == 1 ) {
		return EMovementDirection::UP_RIGHT;
	} else if( dx == 0 && dy == 1 ) {
		return EMovementDirection::RIGHT;
	} else if( dx == 1 && dy == 1 ) {
		return EMovementDirection::DOWN_RIGHT;
	} else if( dx == 1 && dy == 0 ) {
		return EMovementDirection::DOWN;
	} else if( dx == 1 && dy == -1 ) {
		return EMovementDirection::DOWN_LEFT;
	} else if( dx == 0 && dy == -1 ) {
		return EMovementDirection::LEFT;
	} else if( dx == -1 && dy == -1 ) {
		return EMovementDirection::UP_LEFT;
	}
}

int CAStarStrategy::GetNextStep()
{
	std::pair<int, int> top = optimalPath.top();
	optimalPath.pop();
	return getMovementDirection( top.first, top.second );
}

void CAStarStrategy::calculateDistancesToFinish()
{
	// Ставим размеры сетки
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
			int minDistance = maxPathLenght; // Ищем минимальное значение расстояния до финиша среди соседей
			for( int i = -1; i <= 1; ++i ) {
				for( int j = -1; j <= 1; ++j ) {
					// Если на клетку можно встать, её еще не посетили и она не находится с другой стороны от финиша - идем в неё
					if( map.canPlayerStayOnCell( curNode.first + i, curNode.second + j )
						&& distancesToFinish[curNode.second + j][curNode.first + i] == -1
						&& !map.IsOnTheFinishLine( curNode.first, curNode.second, curNode.first + i, curNode.second + j ) )
					{
						bfsQueue.push( std::make_pair( curNode.first + i, curNode.second + j ) );
					} else if( map.canPlayerStayOnCell( curNode.first + i, curNode.second + j ) 
						&& distancesToFinish[curNode.second + j][curNode.first + i] != -1
						&& !map.IsOnTheFinishLine( curNode.first + i, curNode.second + j, curNode.first, curNode.second ) )
					{ 
						// Уже посещенная вершина
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
	std::unordered_set<PlayerState, PlayerState::CPlayerStateHasher> closedSet; // Уже просмотренные вершины
	std::set< std::pair<int, PlayerState> > openSetVal;	// Вершины, которые собираемся просмотреть, отсортированные по возрастанию fScore
	std::unordered_set<PlayerState, PlayerState::CPlayerStateHasher> openSet; // Вершины, которые собираемся просмотреть, со стоимостями
	std::unordered_map<PlayerState, PlayerState, PlayerState::CPlayerStateHasher> cameFrom; // Пройденые вершины
	std::unordered_map<PlayerState, int, PlayerState::CPlayerStateHasher> gScore; // Функция текущей стоимости + эвристика
	std::unordered_map<PlayerState, int, PlayerState::CPlayerStateHasher> fScore; // Функция текущей стоимости

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
					bool tentIsBetter; // Предполагаем ли, что путь лучше

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
							fScore[neighborState] = gScore[neighborState] + heuristic;
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
	return distancesToFinish[nextState.GetY()][nextState.GetX()] / 4;
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
