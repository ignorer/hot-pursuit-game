#pragma once

#include <string>
#include <vector>

const std::string RESOURCE_DIRECTORY = ".\\Resources\\";

typedef std::pair<size_t, size_t> CSize;

enum PlayersTypes {
	USER = 1, AI = 2
};

enum PowerupType {
	WALL, SAND, OIL, MINE, MINE_ACTIVE, LAZER, SHIELD, NONE
};

namespace Core {
	enum Cell {
		ROAD = 0, FOREST = 1, CAR = 2
	};

	struct CCoordinates {
		CCoordinates( int newX = 0, int newY = 0 );

		bool operator== (const CCoordinates &point) const;
		bool operator< (const CCoordinates& point) const;

		CCoordinates operator+ (const CCoordinates &point) const;

		CCoordinates operator- (const CCoordinates &point) const;

		CCoordinates& operator= (CCoordinates right);

		int x;
		int y;
	};

	typedef std::pair<CCoordinates, CCoordinates> CLine;
	typedef std::vector<std::vector<Cell>> CField;
}

namespace UI {
	// координаты в игре
	struct CCoordinates {
		float x;
		float y;
		float angle;

		CCoordinates( float _x, float _y, float _angle = 0 );

		bool operator== (const CCoordinates &point) const;
		bool operator!= (const CCoordinates &point) const;
		bool operator< (const CCoordinates& point) const;
	};

	// координаты в окне
	struct CWindowCoordinates {
		float x;
		float y;

		CWindowCoordinates( float _x = 0, float _y = 0 );
	};

	CWindowCoordinates transateToWcoord( float x, float y, float cellSize, CWindowCoordinates indent, CSize mapSize );
}

bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint );

int area( const Core::CCoordinates& firstPoint, const Core::CCoordinates& secondPoint, const Core::CCoordinates& thirdPoint );

bool isIntersects(
	const Core::CCoordinates& firstPoint,
	const Core::CCoordinates& secondPoint,
	const Core::CCoordinates& thirdPoint,
	const Core::CCoordinates& fourthPoint );