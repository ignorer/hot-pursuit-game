#include <vector>

#include "Core/Game.h"
#include "Core/GameMode.h"

namespace Core {
	CGame::CGame( const CMap& newMap, const std::vector<CPlayer>& playersInfo, const CUIManager* _manager ) :
		map( newMap ),
		players( playersInfo ),
		manager( _manager ),
		hInstanceDLLLibrary( nullptr ),
		StrategyBuilderFunc( nullptr ),
		GetPlayerStateFunc( nullptr )
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
			handleFinishLineIntersectionsForPlayer( i );
		}
	}

	void CGame::handleFinishLineIntersectionsForPlayer( int i )
	{
		players[i].IncreaseLaps( finishLineIntersectsWithPlayer( players[i] ) );
	}

	bool CGame::playerOutOfTrack( const CPlayer& player ) const
	{
		if( player.GetPosition().x < 0 ||
			player.GetPosition().y < 0 ||
			player.GetPosition().x >= map.GetSize().first ||
			player.GetPosition().y >= map.GetSize().second ) {
			return true;
		}
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
		for( size_t j = 0; j < players.size(); ++j ) {
			if( j != playerId && players[playerId].GetPosition() == players[j].GetPosition() && players[playerId].IsAlive() && players[j].IsAlive() ) {
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
		for( auto& player : players ) {
			if( player.GetLaps() >= CGameMode::GetLapCount() ) {
				winners.push_back( player );
			}
		}
	}

	int CGame::turnOfUser( CPlayer& player ) const
	{
		std::vector<CCoordinates> possibleMoves = player.PossibleMoves( map.GetSize( ) );
		manager->MarkPossibleMoves( possibleMoves );
		int direction = manager->GetDirection( possibleMoves, player.GetInertia(), player.GetPosition() );
		manager->UnMarkPossibleMoves( possibleMoves );
		return direction;
	}

	void CGame::initAI( CPlayer* player )
	{
		if( hInstanceDLLLibrary == nullptr ) {
			hInstanceDLLLibrary = ::LoadLibrary( TEXT( "Strategy.dll" ) );
			StrategyBuilderFunc = STRATEGY_PROC(::GetProcAddress( hInstanceDLLLibrary, "GetNewStrategy" ));
			GetPlayerStateFunc = PLAYER_STATE_FACTORY_PROC(::GetProcAddress( hInstanceDLLLibrary, "GetPlayerState" ));
		}

		CField field = map.GetField();
		CSize sizemap = map.GetSize();
		std::vector<std::vector<int>> mapForAI( sizemap.second );

		for( int i = 0; i < sizemap.second; ++i ) {
			mapForAI[i].resize( sizemap.first );
			for( int j = 0; j < sizemap.first; ++j ) {
				mapForAI[i][j] = field[i][j];
			}
		}
		CCoordinates firstFinishPoint = map.GetFinishLine().first;
		CCoordinates secondFinishPoint = map.GetFinishLine().second;

		CCoordinates currentPosition = player->GetPosition();
		CCoordinates previuosPosition = player->GetPreviousPosition();

		int xVelocity = currentPosition.x - previuosPosition.x;
		int yVelocity = currentPosition.y - previuosPosition.y;

		std::shared_ptr<IPlayerState> playerStatePtr( GetPlayerStateFunc( currentPosition.x, currentPosition.y, xVelocity, yVelocity, 0 ) );
		AIStrategies[player->GetNumber()] = std::shared_ptr<IStrategy>( StrategyBuilderFunc( 
			mapForAI, 
			std::make_pair( firstFinishPoint.x, firstFinishPoint.y ),
			std::make_pair( secondFinishPoint.x, secondFinishPoint.y ), 
			CGameMode::GetLapCount(), 
			playerStatePtr,
			CGameMode::GetMovementMode() == CGameMode::SEQUENTIAL ) 
			);
	}

	int CGame::turnOfAI( CPlayer& player )
	{
		std::vector<std::shared_ptr<IPlayerState>> playersForAI;
		for( int i = 0; i < players.size(); ++i ) {
			CCoordinates currentPosition = players[i].GetPosition();
			CCoordinates previuosPosition = players[i].GetPreviousPosition();

			int xVelocity = currentPosition.x - previuosPosition.x;
			int yVelocity = currentPosition.y - previuosPosition.y;
			playersForAI.push_back( std::shared_ptr<IPlayerState>(
				GetPlayerStateFunc( currentPosition.x, currentPosition.y, xVelocity, yVelocity, players[i].GetLaps() ) ) );
		}
		std::vector<Powerup> powerups;
		for( auto powerup : powerupManager.GetPowerups() ) {
			powerups.push_back( Powerup( powerup.second, std::make_pair( powerup.first.x, powerup.first.y ) ) );
		}
		return AIStrategies[player.GetNumber()]->GetNextStep( playersForAI, player.GetNumber(), powerups );
	}

	void CGame::turnOfPlayer( CPlayer& player, std::set<CPlayer*>& crashedPlayers )
	{
		int direction;
		switch ( player.GetType() )
		{
			case USER: 
				direction = turnOfUser( player );
				break;
			case AI:
				direction = turnOfAI( player );
				break;
			default:
				throw std::runtime_error( "Invalid type of player. Please, don't cheat." );
		}
		if( player.DirectionIsValid( Direction( direction ), map.GetSize() ) ) {
			player.Move( Direction( direction ) );
			return;
		}
		player.Move( Direction( direction ) );
		if( CGameMode::GetDeathPenalty() == CGameMode::DESTROY ) {
			player.Die();
		}
		crashedPlayers.insert( &player );
	}

	void CGame::handleCrashes( const std::set<CPlayer*>& crashedPlayers, int& deadPlayersCount ) const
	{
		auto penalty = CGameMode::GetDeathPenalty();
		for( auto player : crashedPlayers ) {
			if( player->GetShield() ) {
				player->DropShield();
				player->SetInertia( CCoordinates( 0, 0 ) );
				continue;
			}
			if( penalty == CGameMode::DESTROY ) {
				player->Die();
				++deadPlayersCount;
				manager->ShowCrashes( crashedPlayers );
			} else if( penalty == CGameMode::TO_START ) {
				player->GoToStart();
				manager->ShowCrashesAndRespawn( crashedPlayers );
			} else if( penalty == CGameMode::STOP ) {
				if( player->GetPenalty() == 1 ) {
					player->SetPenalty( 0 );
				} else {
					player->SetInertia( CCoordinates() );
					player->SetPenalty( 1 );
				}
			}
		}
	}
	
	void CGame::Start()
	{
		std::vector<CPlayer> winners;
		powerupManager.GeneratePowerups( map );
		manager->InitMap( map, players, map.GetFinishLine() );
		int deadPlayersCount = 0;
		std::set<CPlayer*> crashedPlayers;

		for( CPlayer player : players ) {
			if( player.GetType() == AI ) {
				initAI( &player );
			}
		}

		do {
			if( CGameMode::GetMovementMode() == CGameMode::CONCURRENT ) {
				powerupManager.UpdatePowerups( map, players );
				manager->ShowPowerups( powerupManager.GetPowerups() );
				for( size_t i = 0; i < players.size(); ++i ) {
					if( players[i].IsAlive() ) {
						if( players[i].GetPenalty() == 0 ) {
							turnOfPlayer( players[i], crashedPlayers );
						}
					}
				}
				manager->UpdatePlayersInfo( players );
				powerupManager.HandleStep( players, crashedPlayers );
				manager->ShowPowerups( powerupManager.GetPowerups() );
				manager->ShowShots( powerupManager.GetShots(), true );
				manager->UpdatePlayersInfo( players );
				powerupManager.DropShots();
				manager->ShowShots( {}, false );

				findCollisions( crashedPlayers );
				findCrashes( crashedPlayers );
				handleCrashes( crashedPlayers, deadPlayersCount );
				crashedPlayers.clear();
				for( auto& player : players ) {
					player.DecreaseShield();
				}
				manager->UpdatePlayersInfo( players );
				handleFinishLineIntersections();
				findWinners( winners );
			} else if( CGameMode::GetMovementMode() == CGameMode::SEQUENTIAL ) {
				for( size_t i = 0; i < players.size(); ++i ) {
					powerupManager.UpdatePowerups( map, players );
					manager->ShowPowerups( powerupManager.GetPowerups() );
					if( players[i].IsAlive() ) {
						if( players[i].GetPenalty() == 0 ) {
							turnOfPlayer( players[i], crashedPlayers );
						}
						manager->UpdatePlayersInfo( players );
						powerupManager.HandleStepForPlayer( players[i], players, crashedPlayers );
						manager->ShowPowerups( powerupManager.GetPowerups() );
						manager->ShowShots( powerupManager.GetShots(), !powerupManager.GetShots().empty() );
						manager->UpdatePlayersInfo( players );
						powerupManager.DropShots();
						manager->ShowShots( {}, false );
					}

					findCollisionsForPlayer( i, crashedPlayers );
					findCrashesForPlayer( players[i], crashedPlayers );
					handleCrashes( crashedPlayers, deadPlayersCount );
					crashedPlayers.clear();
					players[i].DecreaseShield();
					manager->UpdatePlayersInfo( players );
					handleFinishLineIntersectionsForPlayer( i );
				}
				findWinners( winners );
			}
		} while( winners.size() == 0 && deadPlayersCount < players.size() );

		finish( winners );
	}

	void CGame::finish(  const std::vector<CPlayer>& winners ) const
	{
		manager->FinishGame();
		manager->ShowGameResult( winners );
	}
}