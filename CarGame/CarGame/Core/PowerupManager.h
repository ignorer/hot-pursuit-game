#pragma once

#include <map>

#include "GlobalDefinitions.h"
#include "Core/Player.h"

namespace Core {
	class CMap;

	class CPowerupManager {
	public:
		CPowerupManager();
		PowerupType GetPowerup( const Core::CCoordinates& coordinates ) const;
		PowerupType GetPowerup( int x, int y ) const;
		const std::map<CCoordinates, PowerupType>& GetPowerups() const;
		// ���� ��������� ������ ������ � � ������������ 1/4 ������� ��� ��������� �������
		void GeneratePowerup( const CMap& map );
		void HandleStep( std::vector<CPlayer>& players );

	private:
		std::map<CCoordinates, PowerupType> powerups;
	};
}