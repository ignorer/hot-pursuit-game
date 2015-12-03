#include <ctime>

#include "Core/PowerupManager.h"
#include "Core/Map.h"

namespace Core {
	CPowerupManager::CPowerupManager() :
		lastLap( -1 )
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

	void CPowerupManager::HandleStep( std::vector<Core::CPlayer>& players, std::set<CPlayer*>& crashedPlayers )
	{
		for( auto& player : players ) {
			HandleStepForPlayer( player, players, crashedPlayers );
		}
	}

	void CPowerupManager::HandleStepForPlayer( CPlayer& activePlayer, std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers )
	{
		if( GetPowerup( activePlayer.GetPosition( ).x, activePlayer.GetPosition( ).y ) == NONE ) {
			return;
		}
		switch( powerups[activePlayer.GetPosition( )] ) {
			case WALL:
				// to do
				break;
			case SAND:
				powerups.erase( activePlayer.GetPosition( ) );
				activePlayer.SetInertia( { 0, 0 } );
				break;
			case OIL:
				powerups.erase( activePlayer.GetPosition( ) );
				activePlayer.SetInertia( { 0, 0 } );
				while( true ) {
					int direction = std::rand( ) % 9 + 1;
					if( direction != 5 ) {
						activePlayer.Move( Direction( direction ) );
						break;
					}
				}
				activePlayer.SetInertia( { 0, 0 } );
				break;
			case MINE:
				powerups[activePlayer.GetPosition( )] = MINE_ACTIVE;
				break;
			case MINE_ACTIVE:
				powerups.erase( activePlayer.GetPosition( ) );
				if( activePlayer.GetShield( ) == 0 ) {
					crashedPlayers.insert( &activePlayer );
				}
				break;
			case LAZER:
				// TODO
				powerups.erase( activePlayer.GetPosition( ) );
				break;
			case SHIELD:
				powerups.erase( activePlayer.GetPosition( ) );
				activePlayer.ActivateShield( );
				break;
			default: break;
		}
	}

	void CPowerupManager::UpdatePowerups( const CMap& map, const std::vector<CPlayer>& players )
	{
		int max = 0;
		for( int i = 0; i < players.size(); ++i ) {
			if( players[i].GetLaps() > players[max].GetLaps() ) {
				max = i;
			}
		}
		if( players[max].GetLaps() <= lastLap ) {
			return;
		}
		if( lastLap > -1 && CGameMode::GetObjectChangeModel() == CGameMode::NO_CHANGE ) {
			return;
		}
		powerups.clear();
		int n = 0;
		auto size = map.GetSize();
		switch( CGameMode::GetObjectRate() ) {
			case CGameMode::NO: n = 0;  break;
			case CGameMode::FEW: n = size.first * size.second / 100; break;
			case CGameMode::NORMAL: n = size.first * size.second / 75; break;
			case CGameMode::MANY: n = size.first * size.second / 50; break;
			default: n = 0;  break;
		}
		if( CGameMode::GetObjectChangeModel() == CGameMode::CIRCLE_RANDOM || CGameMode::GetObjectChangeModel() == CGameMode::STABLE && powerups.size() < n ) {
			for( int i = 0; i < n; ++i ) {
				generatePowerup( map );
			}
		} else if( CGameMode::GetObjectChangeModel() == CGameMode::STABLE ) {
			for( auto& powerup : powerups ) {
				while( true ) {
					int type = std::rand() % 8;
					if( type != NONE ) {
						powerup.second = PowerupType( type );
						break;
					}
				}
			}
		}
		lastLap = players[max].GetLaps();
	}
}