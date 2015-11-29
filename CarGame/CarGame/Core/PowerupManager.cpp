#include "Core/PowerupManager.h"
#include "Core/Map.h"

namespace Core {
	PowerupType CPowerupManager::GetPowerup( const Core::CCoordinates& coordinates ) const
	{
		return GetPowerup( coordinates.x, coordinates.y );
	}

	PowerupType CPowerupManager::GetPowerup( int x, int y ) const
	{
		if( powerups.find( CCoordinates( x, y ) ) == powerups.end() ) {
			return NONE;
		}
		return powerups.at(CCoordinates( x, y ));
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
				if( type != MINE_ACTIVE && type != NONE ) {
					powerups[Core::CCoordinates( x, y )] = PowerupType( type );
					return;
				}
			}
		}
	}

	void CPowerupManager::HandleStep( std::vector<Core::CPlayer>& players )
	{
		// тут будет много кода
	}
}