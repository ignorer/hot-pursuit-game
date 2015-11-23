#pragma once

#include <string>
#include <vector>
#include <Windows.h>

const std::string RESOURCE_DIRECTORY = ".\\Resources\\";

enum PlayersTypes {
	USER = 1, AI = 2
};

namespace Core {

	enum Cell {
		ROAD = 0, FOREST = 1, CAR = 2
	};

	struct CCoordinates {
		CCoordinates( int newX = 0, int newY = 0) :
			x( newX ),
			y( newY )
		{}

		bool operator == ( const CCoordinates &point ) const
		{
			return this->x == point.x && this->y == point.y;
		}
		bool operator<( const CCoordinates& point ) const
		{
			return x < point.x ? true : y < point.y;
		}

		CCoordinates operator + ( const CCoordinates &point ) const
		{
			return CCoordinates( this->x + point.x, this->y + point.y );
		}

		CCoordinates operator - ( const CCoordinates &point ) const
		{
			return CCoordinates( this->x - point.x, this->y - point.y );
		}
		
		CCoordinates& operator = ( CCoordinates right ) 
		{
			this->x = right.x;
			this->y = right.y;
			return *this;
		}

		int x;
		int y;
	};

	typedef std::pair<CCoordinates, CCoordinates> CLine;
	typedef std::vector<std::vector<Cell>> CField;
}

typedef std::pair<size_t, size_t> CSize;

enum PowerupType {
	WALL, SAND, OIL, MINE, MINE_ACTIVE, LAZER, SHIELD, NONE
};