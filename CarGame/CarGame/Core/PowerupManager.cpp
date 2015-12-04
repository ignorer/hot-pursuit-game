#include <ctime>
#include <algorithm>

#include "Core/PowerupManager.h"
#include "Core/Map.h"

namespace Core {
	CPowerupManager::CPowerupManager() :
		lastLap( -1 ),
		initialized( false )
	{
		std::srand( std::time( nullptr ) );
	}

	PowerupType CPowerupManager::GetPowerup( const Core::CCoordinates& coordinates ) const
	{
		return GetPowerup( coordinates.x, coordinates.y );
	}

	PowerupType CPowerupManager::GetPowerup( int x, int y ) const
	{
		if( powerups.find( CCoordinates( x, y ) ) == powerups.end() ) {
			return NONE;
		}
		return powerups.at( CCoordinates( x, y ) );
	}

	const std::map<CCoordinates, PowerupType>& CPowerupManager::GetPowerups() const
	{
		return powerups;
	}

	void CPowerupManager::generatePowerup( const Core::CMap& map )
	{
		while( true ) {
			int y = std::rand() % map.GetSize().first;
			int x = std::rand() % map.GetSize().second;
			if( map.GetField()[y][x] == ROAD && powerups.find( CCoordinates( x, y ) ) == powerups.end() ) {
				int type = std::rand() % 8;
				if( type != NONE ) {
					powerups[Core::CCoordinates( x, y )] = PowerupType( type );
					return;
				}
			}
		}
	}

	void CPowerupManager::handleWalls( CPlayer& player, std::set<CPlayer*>& crashedPlayers )
	{
		CCoordinates playersPreviousCoordinates = player.GetPreviousPosition( );
		CCoordinates playersCoordinates = player.GetPosition();
		
		for( auto powerup : powerups ) {
			CCoordinates realCoordinates( playersCoordinates.y * 10 + 5, playersCoordinates.x * 10 + 5 ),
				realPreviousCoordinates( playersPreviousCoordinates.y * 10 + 5, playersPreviousCoordinates.x * 10 + 5 );

			if( powerup.second != WALL ) {
				continue;
			}
			CCoordinates firstPoint( powerup.first.y * 10, powerup.first.x * 10 ),
				secondPoint( (powerup.first.y + 1) * 10, powerup.first.x * 10 ),
				thirdPoint( (powerup.first.y + 1) * 10, (powerup.first.x + 1) * 10 ),
				fourthPoint( powerup.first.y * 10, (powerup.first.x + 1) * 10 );

			if( isIntersects( realPreviousCoordinates, realCoordinates, firstPoint, secondPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, secondPoint, thirdPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, thirdPoint, fourthPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, fourthPoint, firstPoint ) ) {
				powerups.erase( powerup.first );
				crashedPlayers.insert( &player );
				return;
			}
		}
	}

	void CPowerupManager::HandleStep( std::vector<Core::CPlayer>& players, std::set<CPlayer*>& crashedPlayers )
	{
		for( auto& player : players ) {
			HandleStepForPlayer( player, players, crashedPlayers );
		}
	}

	void CPowerupManager::HandleStepForPlayer( CPlayer& activePlayer, std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers )
	{
		if( GetPowerup( activePlayer.GetPosition().x, activePlayer.GetPosition().y ) != NONE ) {
			switch( powerups[activePlayer.GetPosition()] ) {
				case WALL:
					powerups.erase( activePlayer.GetPosition() );
					crashedPlayers.insert( &activePlayer );
					break;
				case SAND:
					powerups.erase( activePlayer.GetPosition() );
					activePlayer.SetInertia( { 0, 0 } );
					break;
				case OIL:
					powerups.erase( activePlayer.GetPosition() );
					activePlayer.SetInertia( { 0, 0 } );
					while( true ) {
						int direction = std::rand() % 9 + 1;
						if( direction != 5 ) {
							activePlayer.Move( Direction( direction ) );
							break;
						}
					}
					activePlayer.SetInertia( { 0, 0 } );
					break;
				case MINE:
					powerups[activePlayer.GetPosition()] = MINE_ACTIVE;
					break;
				case MINE_ACTIVE:
					powerups.erase( activePlayer.GetPosition() );
					crashedPlayers.insert( &activePlayer );
					break;
				case LAZER:
					// TODO
					powerups.erase( activePlayer.GetPosition() );
					break;
				case SHIELD:
					powerups.erase( activePlayer.GetPosition() );
					activePlayer.ActivateShield();
					break;
				default: break;
			}
		}
		for( auto& player : players ) {
			handleWalls( player, crashedPlayers );
		}
	}

	void CPowerupManager::GeneratePowerups( const CMap& map )
	{
		int n = 0;
		auto size = map.GetSize();
		switch( CGameMode::GetObjectRate() ) {
			case CGameMode::NO: n = 0;  break;
			case CGameMode::FEW: n = size.first * size.second / 100; break;
			case CGameMode::NORMAL: n = size.first * size.second / 75; break;
			case CGameMode::MANY: n = size.first * size.second / 50; break;
			default: n = 0;  break;
		}
		if( initialized && CGameMode::GetObjectChangeModel() == CGameMode::NO_CHANGE ) {
			return;
		}
		if( !initialized || CGameMode::GetObjectChangeModel() == CGameMode::RANDOM ) {
			if( CGameMode::GetObjectChangeModel() == CGameMode::RANDOM ) {
				powerupCoordinates.clear();
			}
			initialized = true;
			for( int i = 0; i < n; ++i ) {
				int x, y;
				do {
					y = std::rand() % map.GetSize().first;
					x = std::rand() % map.GetSize().second;
				} while( map.GetField()[y][x] != ROAD || powerups.find( CCoordinates( x, y ) ) != powerups.end() );
				powerupCoordinates.push_back( CCoordinates( x, y ) );
			}
		}
		powerups.clear();
		for( auto coordinates : powerupCoordinates ) {
			int type;
			do {
				type = std::rand() % 8;
			} while( type == NONE );
			powerups[Core::CCoordinates( coordinates.x, coordinates.y )] = PowerupType( type );
		}
	}

	void CPowerupManager::UpdatePowerups( const CMap& map, const std::vector<CPlayer>& players )
	{
		if( players.empty() ) {
			return;
		}
		int max = 0;
		for( int i = 0; i < players.size(); ++i ) {
			if( players[i].GetLaps() > players[max].GetLaps() ) {
				max = i;
			}
		}
		if( players[max].GetLaps() <= lastLap ) {
			return;
		}
		lastLap = players[max].GetLaps();
		GeneratePowerups( map );
	}
}