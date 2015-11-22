#include <vector>
#include <algorithm>

#include "Core/Game.h"
#include "Core/GameMode.h"
#include "AI/IPlayerState.h"
#include "AI/IMap.h"

HINSTANCE hInstanceDLLLibrary = nullptr;

typedef int( __cdecl *STRATEGY_PROC )(const std::vector< std::vector < int > > &inputCells,
	const std::pair< int, int > &_leftFinishPoint,
	const std::pair< int, int > &_rightFinishPoint, 
	const std::vector<std::shared_ptr<IPlayerState>> &_playerStates, int curPlayerPosition);

typedef IPlayerState*(__cdecl *PLAYER_STATE_FACTORY_PROC)(int x, int y, int xVelocity, int yVelocity);

typedef IMap*(__cdecl *MAP_DEFAULT_FACTORY_PROC)();

namespace Core {
	namespace {
		bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint )
		{
			if( firstPoint > secondPoint ) {
				std::swap( firstPoint, secondPoint );
			}
			if( thirdPoint > forthPoint ) {
				std::swap( thirdPoint, forthPoint );
			}
			return max( firstPoint, thirdPoint ) <= min( secondPoint, forthPoint );
		}

		int area( const CCoordinates& firstPoint, const CCoordinates& secondPoint, const CCoordinates& thirdPoint )
		{
			return (secondPoint.x - firstPoint.x) * (thirdPoint.y - firstPoint.y)
				- (secondPoint.y - firstPoint.y) * (thirdPoint.x - firstPoint.x);
		}

		bool isIntersects(
			const CCoordinates& firstPoint,
			const CCoordinates& secondPoint,
			const CCoordinates& thirdPoint,
			const CCoordinates& fourthPoint )
		{
			return inBoxOnAxis( firstPoint.x, secondPoint.x, thirdPoint.x, fourthPoint.x )
				&& inBoxOnAxis( firstPoint.y, secondPoint.y, thirdPoint.y, fourthPoint.y )
				&& (area( firstPoint, secondPoint, thirdPoint )
					* area( firstPoint, secondPoint, fourthPoint )) <= 0
				&& (area( thirdPoint, fourthPoint, firstPoint )
					* area( thirdPoint, fourthPoint, secondPoint )) <= 0;
		}
	}

	CGame::CGame( const CMap& newMap, const std::vector<CPlayer>& playersInfo, const CUIManager* _manager ) :
		map( newMap ),
		players( playersInfo ),
		manager( _manager )
	{}

	int CGame::finishLineIntersectsWithPlayer( const CPlayer& player ) const
	{
		// Происходит проверка:
		// 1. Проекции отрезков на оси пересекаются
		// 2. Считается ориентированная площадь треугольников. Нужно, чтобы эти площади были разных знаков.
		CCoordinates playersPreviousCoordinates = player.GetPreviousPosition();
		CCoordinates playersCoordinates = player.GetPosition();

		if (playersCoordinates == playersPreviousCoordinates)
			return 0;

		if ( isIntersects( playersPreviousCoordinates, playersCoordinates, map.GetFinishLine().first, map.GetFinishLine().second ) )
		{
			CCoordinates normalVectorCoordinates( map.GetFinishLine().second.x - map.GetFinishLine().first.x,
				map.GetFinishLine().second.y - map.GetFinishLine().first.y );
			if (area(map.GetFinishLine().first, playersCoordinates, map.GetFinishLine().second) *
				area(map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second) >= 0 &&
				area( map.GetFinishLine().first, playersPreviousCoordinates, map.GetFinishLine().second ) *
				area( map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second ) < 0 )
				return 1;
			return -1;
		}
		return 0;
	}

	void CGame::handleFinishLineIntersections()
	{
		for( int i = 0; i < players.size(); ++i ) {
			players[i].IncreaseLaps( finishLineIntersectsWithPlayer( players[i] ) );
		}
	}

	bool CGame::playerOutOfTrack( const CPlayer& player ) const
	{
		CCoordinates playersPreviousCoordinates = player.GetPreviousPosition();
		CCoordinates playersCoordinates = player.GetPosition();

		int minX = min( playersPreviousCoordinates.x, playersCoordinates.x ),
			maxX = max( playersPreviousCoordinates.x, playersCoordinates.x ),
			minY = min( playersPreviousCoordinates.y, playersCoordinates.y ),
			maxY = max( playersPreviousCoordinates.y, playersCoordinates.y );

		CCoordinates realCoordinates( playersCoordinates.x * 10 + 5, playersCoordinates.y * 10 + 5 ),
			realPreviousCoordinates( playersPreviousCoordinates.x * 10 + 5, playersPreviousCoordinates.y * 10 + 5 );

		for( int i = minX; i <= maxX; ++i ) {
			for( int j = minY; j <= maxY; ++j ) {
				if( map.IsEmpty( i, j ) ) {
					continue;
				}
				CCoordinates firstPoint( i * 10, j * 10 ),
					secondPoint( (i + 1) * 10, j * 10 ),
					thirdPoint( (i + 1) * 10, (j + 1) * 10 ),
					fourthPoint( i * 10, (j + 1) * 10 );

				if( isIntersects( realPreviousCoordinates, realCoordinates, firstPoint, secondPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, secondPoint, thirdPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, thirdPoint, fourthPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, fourthPoint, firstPoint ) ) {
					return true;
				}
			}
		}
		return false;
	}

	void CGame::findCollisions( std::set<CPlayer*>& crashedPlayers )
	{
		for( size_t i = 0; i < players.size(); ++i ) {
			findCollisionsForPlayer( i, crashedPlayers );
		}
	}

	void CGame::findCollisionsForPlayer( int playerId, std::set<CPlayer*>& crashedPlayers )
	{
		for( size_t j = playerId + 1; j < players.size(); ++j ) {
			if( players[playerId].GetPosition() == players[j].GetPosition() && players[playerId].IsAlive() && players[j].IsAlive() ) {
				crashedPlayers.insert( &players[playerId] );
				crashedPlayers.insert( &players[j] );
			}
		}
	}

	void CGame::findCrashes( std::set<CPlayer*>& crashedPlayers )
	{
		for( int i = 0; i < players.size(); ++i ) {
			findCrashesForPlayer( players[i], crashedPlayers );
		}
	}

	void CGame::findCrashesForPlayer( CPlayer& player, std::set<CPlayer*>& crashedPlayers ) const
	{
		if( player.IsAlive() && playerOutOfTrack( player ) ) {
			crashedPlayers.insert( &player );
		}
	}

	void CGame::findWinners( std::vector<CPlayer>& winners ) const
	{
		int lapCount = CGameMode::GetLapCount();
		for( auto player : players ) {
			if( player.GetLaps() >= lapCount ) {
				winners.push_back( player );
			}
		}
	}

	void CGame::turnOfPlayer( CPlayer& player, std::set<CPlayer*>& crashedPlayers )
	{
		int direction;
		std::vector<CCoordinates> possibleMoves;
		switch ( player.GetType() )
		{
			case USER: 
				//вывести возможные ходы
				possibleMoves = player.PossibleMoves(map.GetSize());
				manager->MarkPossibleMoves(possibleMoves);
				//manager-->possibleMoves
				direction = manager->GetDirection(possibleMoves, player.GetInertia(), player.GetPosition());
				manager->UnMarkPossibleMoves(possibleMoves);
				break;
			case AI:
			{
				if( !hInstanceDLLLibrary ) hInstanceDLLLibrary = LoadLibrary( TEXT( "Strategy.dll" ) );
				STRATEGY_PROC StrategyFunc = (STRATEGY_PROC)GetProcAddress( hInstanceDLLLibrary, "StrategyFunc" );
				PLAYER_STATE_FACTORY_PROC GetPlayerState = (PLAYER_STATE_FACTORY_PROC)GetProcAddress( hInstanceDLLLibrary, "GetPlayerState" );
				// Map map;

				CField field = map.GetField();
				CSize sizemap = map.GetSize();
				std::vector< std::vector< int > > mapForAI( sizemap.second );

				for( int i = 0; i < sizemap.second; ++i ) {
					mapForAI[i].resize( sizemap.first );
					for( int j = 0; j < sizemap.first; ++j ) {
						mapForAI[i][j] = field[i][j];
					}
				}
				CCoordinates firstFinishPoint = map.GetFinishLine().first;
				CCoordinates secondFinishPoint = map.GetFinishLine().second;

				std::vector< std::shared_ptr< IPlayerState > > playerStates;

				for ( int i = 0; i < players.size(); ++i )
				{
					if ( !players[i].IsAlive() ) continue;

					CCoordinates currentPosition = players[i].GetPosition();
					CCoordinates previuosPosition = players[i].GetPreviousPosition();

					int xVelocity = currentPosition.x - previuosPosition.x;
					int yVelocity = currentPosition.y - previuosPosition.y;

					std::shared_ptr<IPlayerState> playerStatePtr( GetPlayerState( currentPosition.x, currentPosition.y, xVelocity, yVelocity ) );
					playerStates.push_back( playerStatePtr );
				}

				direction = StrategyFunc( mapForAI, std::make_pair( firstFinishPoint.x, firstFinishPoint.y ), 
					std::make_pair( secondFinishPoint.x, secondFinishPoint.y ), playerStates, player.GetNumber() );
				break;
			}
			default:
				throw std::runtime_error( "Invalid type of player. Please, don't cheat." );
		}
		if( player.DirectionIsValid( Direction( direction ), map.GetSize() ) ) {
			player.Move( Direction( direction ) );
			return;
		}
		player.Move( Direction( direction ) );
		player.Die();
		crashedPlayers.insert( &player );
	}

	void CGame::handleCrashes( const std::set<CPlayer*>& crashedPlayers, int& deadPlayersCount ) const
	{
		auto penalty = CGameMode::GetDeathPenalty();
		if( penalty == CGameMode::DESTROY ) {
			for( auto player : crashedPlayers ) {
				player->Die();
				++deadPlayersCount;
			}
			manager->ShowCrashes( crashedPlayers );
		} else if( penalty == CGameMode::TO_START ) {
			for( auto player : crashedPlayers ) {
				player->GoToStart();
			}
			manager->ShowCrashesAndRespawn( crashedPlayers );
		} else if( penalty == CGameMode::STOP ) {
			for( auto player : crashedPlayers ) {
				player->SetInertia( CCoordinates() );
				// здесь вкрутить пропуск хода
			}
		}
	}

	void CGame::Start()
	{
		std::vector<CPlayer> winners;
		int deadPlayersCount = 0;
		manager->InitMap( map, players, map.GetFinishLine() );
		std::set<CPlayer*> crashedPlayers;

		do {
			if( CGameMode::GetMovementMode() == CGameMode::CONCURRENT ) {
				for( size_t i = 0; i < players.size(); ++i ) {
					if( players[i].IsAlive() ) {
						turnOfPlayer( players[i], crashedPlayers );
					}
				}
				manager->Move( players );

				findCollisions( crashedPlayers );
				findCrashes( crashedPlayers );
				handleCrashes( crashedPlayers, deadPlayersCount );
				crashedPlayers.clear();
			} else if( CGameMode::GetMovementMode() == CGameMode::SEQUENTIAL ) {
				for( size_t i = 0; i < players.size(); ++i ) {
					if( players[i].IsAlive() ) {
						turnOfPlayer( players[i], crashedPlayers );
						manager->Move( {players[i]} );
					}

					findCollisionsForPlayer( i, crashedPlayers );
					findCrashesForPlayer( players[i], crashedPlayers );
					handleCrashes( crashedPlayers, deadPlayersCount );
					crashedPlayers.clear();
				}
			}

			handleFinishLineIntersections();
			findWinners( winners );
		} while( winners.size() == 0 && deadPlayersCount < players.size() );

		finish( winners );
	}

	void CGame::finish(  const std::vector<CPlayer>& winners ) const
	{
		manager->FinishGame();
		manager->ShowGameResult( winners );
	}
}