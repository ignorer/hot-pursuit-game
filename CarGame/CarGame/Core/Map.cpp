#include <iostream>

#include "Core/Map.h"

namespace Core {
	CMap::CMap( const CField& newMap, CLine finish, const std::vector<CCoordinates>& _startPoints ) :
		map( newMap ),
		size( newMap[0].size(), newMap.size() ),
		finishLine( finish ),
		startPoints( _startPoints )
	{}

	bool CMap::IsEmpty( int x, int y ) const
	{
		return map[y][x] != FOREST && map[y][x] != WALL_CELL;
	}

	CSize CMap::GetSize() const
	{
		return size;
	}

	CField CMap::GetField() const
	{
		return map;
	}

	CLine CMap::GetFinishLine() const
	{
		return finishLine;
	}

	const std::vector<CCoordinates>& CMap::GetStartPoints() const
	{
		return startPoints;
	}
}