#include <ctime>

#include "Core/PowerupManager.h"
#include "Core/Map.h"

namespace Core {
	CPowerupManager::CPowerupManager()
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

	void CPowerupManager::GeneratePowerup( const Core::CMap& map )
	{
		if( std::rand() % 4 != 0 ) {
			return;
		}
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

	void CPowerupManager::HandleStep( std::vector<Core::CPlayer>& players )
	{
		for( auto& player : players ) {
			if( GetPowerup( player.GetPosition().x, player.GetPosition().y ) == NONE ) {
				continue;
			}
			switch( powerups[player.GetPosition()] ) {
				case WALL:
					// to do
					break;
				case SAND:
					player.SetInertia( { 0, 0 } );
					powerups.erase( player.GetPosition() );
					break;
				case OIL:
					player.SetInertia( { 0, 0 } );
					while( true ) {
						int direction = std::rand() % 9 + 1;
						if( direction != 5 ) {
							player.Move( Direction( direction ) );
							break;
						}
					}
					player.SetInertia( { 0, 0 } );
					powerups.erase( player.GetPosition() );
					break;
				case MINE:
					powerups[player.GetPosition()] = MINE_ACTIVE;
					break;
				case MINE_ACTIVE:
					player.Die( );
					powerups.erase( player.GetPosition() );
					break;
				case LAZER:
					// TODO
					powerups.erase( player.GetPosition() );
					break;
				case SHIELD:
					player.ActivateShield();
					powerups.erase( player.GetPosition() );
					break;
				default: break;
			}
		}
	}
}