#pragma once

#include <map>
#include <set>

#include "GlobalDefinitions.h"
#include "Core/Player.h"
#include "GameMode.h"

namespace Core {
	class CMap;

	class CPowerupManager {
	public:
		CPowerupManager();
		PowerupType GetPowerup( const Core::CCoordinates& coordinates ) const;
		PowerupType GetPowerup( int x, int y ) const;
		const std::map<CCoordinates, PowerupType>& GetPowerups() const;
		void HandleStep( std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		void HandleStepForPlayer( CPlayer& activePlayer, std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		// создаёт по всей карте поверапы или пересоздаёт на новом круге. в случае, если никто не зашёл на новый круг, не делает ничего
		void UpdatePowerups( const CMap& map, const std::vector<CPlayer>& players );

	private:
		// ищет свободную клетку дороги и генерит там рандомный поверап
		void generatePowerup( const CMap& map );

		std::map<CCoordinates, PowerupType> powerups;
		int lastLap;
	};
}