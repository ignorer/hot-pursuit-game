#include "UI/Coordinates.h"

UI::CWindowCoordinates UI::transateToWcoord( float x, float y, float cellSize, CWindowCoordinates indent, CSize mapSize )
{
	return CWindowCoordinates( x * cellSize + indent.x, (mapSize.second - y) * cellSize + indent.y );
}