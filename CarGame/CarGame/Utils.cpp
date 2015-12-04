#include <algorithm>

#include "Utils.h"

Core::CCoordinates::CCoordinates( int newX, int newY ) :
	x( newX ),
	y( newY )
{
}

bool Core::CCoordinates::operator== (const CCoordinates& point) const
{
	return this->x == point.x && this->y == point.y;
}

bool Core::CCoordinates::operator< (const CCoordinates& point) const
{
	return std::pair<float, float>( x, y ) < std::pair<float, float>( point.x, point.y );
}

Core::CCoordinates Core::CCoordinates::operator+ (const CCoordinates& point) const
{
	return CCoordinates( this->x + point.x, this->y + point.y );
}

Core::CCoordinates Core::CCoordinates::operator- (const CCoordinates& point) const
{
	return CCoordinates( this->x - point.x, this->y - point.y );
}

Core::CCoordinates& Core::CCoordinates::operator= (CCoordinates right)
{
	this->x = right.x;
	this->y = right.y;
	return *this;
}

UI::CCoordinates::CCoordinates( float _x, float _y, float _angle ) :
	x( _x ),
	y( _y ),
	angle( _angle )
{
}

bool UI::CCoordinates::operator== (const CCoordinates& point) const
{
	return this->x == point.x && this->y == point.y;
}

bool UI::CCoordinates::operator!= (const CCoordinates& point) const
{
	return !(*this == point);
}

bool UI::CCoordinates::operator< (const CCoordinates& point) const
{
	return std::pair<float, float>( x, y ) < std::pair<float, float>( point.x, point.y );
}

UI::CWindowCoordinates::CWindowCoordinates( float _x, float _y ) :
	x( _x ),
	y( _y )
{
}

UI::CWindowCoordinates UI::transateToWcoord( float x, float y, float cellSize, CWindowCoordinates indent, CSize mapSize )
{
	return CWindowCoordinates( x * cellSize + indent.x, (mapSize.second - y) * cellSize + indent.y );
}

bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint )
{
	if( firstPoint > secondPoint ) {
		std::swap( firstPoint, secondPoint );
	}
	if( thirdPoint > forthPoint ) {
		std::swap( thirdPoint, forthPoint );
	}
	return std::max( firstPoint, thirdPoint ) <= std::min( secondPoint, forthPoint );
}

int area( const Core::CCoordinates& firstPoint, const Core::CCoordinates& secondPoint, const Core::CCoordinates& thirdPoint )
{
	return (secondPoint.x - firstPoint.x) * (thirdPoint.y - firstPoint.y)
		- (secondPoint.y - firstPoint.y) * (thirdPoint.x - firstPoint.x);
}

bool isIntersects(
	const Core::CCoordinates& firstPoint,
	const Core::CCoordinates& secondPoint,
	const Core::CCoordinates& thirdPoint,
	const Core::CCoordinates& fourthPoint )
{
	return inBoxOnAxis( firstPoint.x, secondPoint.x, thirdPoint.x, fourthPoint.x )
		&& inBoxOnAxis( firstPoint.y, secondPoint.y, thirdPoint.y, fourthPoint.y )
		&& (area( firstPoint, secondPoint, thirdPoint )
		* area( firstPoint, secondPoint, fourthPoint )) <= 0
		&& (area( thirdPoint, fourthPoint, firstPoint )
		* area( thirdPoint, fourthPoint, secondPoint )) <= 0;
}