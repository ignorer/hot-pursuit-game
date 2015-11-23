#pragma once

#include <map>

#include "GlobalDefinitions.h"
#include "Core/Player.h"

namespace Core {
	class CMap;

	class CPowerupManager {
	public:
		PowerupType GetPowerup( const Core::CCoordinates& coordinates ) const;
		PowerupType GetPowerup( int x, int y ) const;
		// ���� ��������� ������ ������ � � ������������ 1/4 ������� ��� ��������� �������
		void GeneratePowerup( const Core::CMap& map );
		void HandleStep( std::vector<Core::CPlayer>& players );

	private:
		std::map<Core::CCoordinates, PowerupType> powerups;
	};
}