#pragma once

#include <map>
#include <set>

#include "Utils.h"
#include "Core/Player.h"
#include "GameMode.h"

namespace Core {
	class CMap;

	class CPowerupManager {
	public:

		CPowerupManager();
		PowerupType GetPowerup( const Core::CCoordinates& coordinates ) const;
		PowerupType GetPowerup( int x, int y ) const;
		const std::vector<std::pair<CCoordinates, CCoordinates>>& GetShots() const;
		const std::map<CCoordinates, PowerupType>& GetPowerups() const;

		void DropShots();

		void HandleStep( std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		void HandleStepForPlayer( CPlayer& activePlayer, std::vector<CPlayer>& players, std::set<CPlayer*>& crashedPlayers );
		// ������ �� ���� ����� �������� ��� ���������� �� ����� �����. � ������, ���� ����� �� ����� �� ����� ����, �� ������ ������
		void GeneratePowerups( const CMap& map );
		void UpdatePowerups( const CMap& map, const std::vector<CPlayer>& players );

	private:
		// ���� ��������� ������ ������ � ������� ��� ��������� �������
		void generatePowerup( const CMap& map );
		void handleWalls( CPlayer& player, std::set<CPlayer*>& crashedPlayers );
		
		std::map<CCoordinates, PowerupType> powerups;
		// ��� ���� ����� ��� ������ ��������� ��������� STABLE
		std::vector<CCoordinates> powerupCoordinates;
		int lastLap;
		bool initialized;
		std::vector<std::pair<CCoordinates, CCoordinates>> shots;
	};
}