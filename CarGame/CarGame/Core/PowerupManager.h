#pragma once

#include <map>
#include <set>

#include "Utils.h"
#include "Core/Player.h"

namespace Core {
	class CMap;

	class CPowerupManager {
	public:

		CPowerupManager();
		PowerupType GetPowerup( const Core::CCoordinates& coordinates ) const;
		PowerupType GetPowerup( int x, int y ) const;
		const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& GetShots( ) const;
		const std::map<CCoordinates, PowerupType>& GetPowerups() const;

		void DropShots();

		void HandleStep( std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		void HandleStepForPlayer( CPlayer& activePlayer, std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		// создаёт по всей карте поверапы или пересоздаёт на новом круге. в случае, если никто не зашёл на новый круг, не делает ничего
		void GeneratePowerups( const CMap& map );
		void UpdatePowerups( const CMap& map, const std::vector<CPlayer>& players );

	private:
		// ищет свободную клетку дороги и генерит там рандомный поверап
		void generatePowerup( const CMap& map );
		void handleWalls( CPlayer& player, std::set<CPlayer*>& crashedPlayers );
		
		std::map<CCoordinates, PowerupType> powerups;
		// это поле нужно для режима генерации поверапов STABLE
		std::vector<CCoordinates> powerupCoordinates;
		int lastLap;
		bool initialized;
		std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> shots;
	};
}