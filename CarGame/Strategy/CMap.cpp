#pragma once

#define DEFAULT_X_SIZE 14
#define DEFAULT_Y_SIZE 14

#include "CMap.hpp"
#include <stdexcept>
#include <algorithm>
#include <cmath>

Map::Map() : 
	xSize(DEFAULT_X_SIZE),
	ySize(DEFAULT_Y_SIZE),
	cells(DEFAULT_Y_SIZE, std::vector< int >(DEFAULT_X_SIZE, EMPTY_CELL))
{
	fillMapWithDefaultData();
}

Map::Map(const size_t xSize, const size_t ySize) :
	xSize(xSize),
	ySize(ySize),
	cells(ySize, std::vector< int >(xSize, EMPTY_CELL))
{
}

Map::Map(const std::vector< std::vector < int > > &inputCells,
	const std::pair< int, int > &_leftFinishPoint,
	const std::pair< int, int > &_rightFinishPoint) :
	cells(inputCells),
	leftFinishPoint(_leftFinishPoint),
	rightFinishPoint(_rightFinishPoint),
	ySize(inputCells.size()),
	xSize(inputCells[0].size())
{
}

Map::~Map() {
}

const std::pair<size_t, size_t> Map::size() const {
    return std::make_pair(this->xSize, this->ySize);
}

const size_t Map::sizeOnXaxis() const {
    return xSize;
}

const size_t Map::sizeOnYaxis() const {
    return ySize;
}

bool Map::canPlayerStayOnCell(int x, int y) const {
    if (x < 0 || y < 0 || x >= xSize || y >= ySize) {
        return false;
    }
    
    if (cells[y][x] == 0) {
        return true;
    } else {
        return false;
    }
}

bool Map::canPlayerStayOnCellLookOnOtherPlayers(int x, int y, int playerID, const std::vector< IPlayerState > &players) const {
    if (x < 0 || y < 0 || x >= xSize || y >= ySize) {
        return false;
    }
    
    for (int i = 0; i < players.size(); ++i) {
        if (i == playerID) {
            continue;
        } else {
            if (players[i].GetX() == x && players[i].GetY() == y) {
                return false;
            }
        }
    }
    
    if (cells[y][x] == 0) {
        return true;
    } else {
        return false;
    }
}

bool Map::isIntersects( const std::pair<int, int>& firstPoint, const std::pair<int, int>& secondPoint,
	const std::pair<int, int>& thirdPoint, const std::pair<int, int>& fourthPoint ) const
{
	return intersect( firstPoint.first, secondPoint.first, thirdPoint.first, fourthPoint.first )
		&& intersect( firstPoint.second, secondPoint.second, thirdPoint.second, fourthPoint.second )
		&& ( orientedArea( firstPoint.first, firstPoint.second, secondPoint.first,
		secondPoint.second, thirdPoint.first, thirdPoint.second )
		* orientedArea( firstPoint.first, firstPoint.second, secondPoint.first,
		secondPoint.second, fourthPoint.first, fourthPoint.second )) <= 0
		&& ( orientedArea( thirdPoint.first, thirdPoint.second, fourthPoint.first,
		fourthPoint.second, firstPoint.first, firstPoint.second )
		* orientedArea( thirdPoint.first, thirdPoint.second, fourthPoint.first,
		fourthPoint.second, secondPoint.first, secondPoint.second ) ) <= 0;
}

bool Map::hasBarrierOnPath(int xFirst, int yFirst, int xSecond, int ySecond) const {
	int minX = std::min( xFirst, xSecond ),
		maxX = std::max( xFirst, xSecond ),
		minY = std::min( yFirst, ySecond ),
		maxY = std::max( yFirst, ySecond );

	std::pair<int, int> prevCoordinates = std::make_pair( xFirst * 10 + 5, yFirst * 10 + 5 );
	std::pair<int, int> curCoordinates = std::make_pair( xSecond * 10 + 5, ySecond * 10 + 5 );

	for( int i = minX; i <= maxX; ++i ) {
		for( int j = minY; j <= maxY; ++j ) {
			if( cells[j][i] == 0 ) {
				continue;
			}
			std::pair<int, int> firstPoint( i * 10, j * 10 ),
				secondPoint( (i + 1) * 10, j * 10 ),
				thirdPoint( (i + 1) * 10, (j + 1) * 10 ),
				fourthPoint( i * 10, (j + 1) * 10 );

			if( isIntersects( prevCoordinates, curCoordinates, firstPoint, secondPoint ) ||
				isIntersects( prevCoordinates, curCoordinates, secondPoint, thirdPoint ) ||
				isIntersects( prevCoordinates, curCoordinates, thirdPoint, fourthPoint ) ||
				isIntersects( prevCoordinates, curCoordinates, fourthPoint, firstPoint ) ) 
			{
				return true;
			}
		}
	}
	if( wrongFinishLineIntersection( xFirst, yFirst, xSecond, ySecond ) ) {
		return true;
	}

	return false;
}

void Map::fillMapWithDefaultData() {
    for (int i = 0; i < DEFAULT_Y_SIZE; ++i) {
        for (int j = 0; j < DEFAULT_X_SIZE; ++j) {
            if (i >= 10 && j >= 0 && i < 20 && j < 20) {
                cells[i][j] = 1;
            } else if(i == 15 && j < 20) {
                cells[i][j] = 1;
            } else {
                cells[i][j] = 0;
            }
        }
    }

	leftFinishPoint = std::make_pair(DEFAULT_X_SIZE - 2, DEFAULT_Y_SIZE - 1);
	rightFinishPoint = std::make_pair(DEFAULT_X_SIZE - 1, DEFAULT_Y_SIZE - 2);
}

void Map::print(const std::vector< IPlayerState > &players) const {
    for (int i = 0; i < ySize; ++i) {
        for (int j = 0; j < xSize; ++j) {
            for (int p = 0; p < players.size(); ++p) {
                if (players[p].getPosition() == std::make_pair(i, j)) {
                    std::cout << "*";
                } else {
                    std::cout << cells[i][j];
                }
            }
        }
        std::cout << std::endl;
    }
}

std::vector< int >& Map::operator[](int i) {
    if (i >= ySize) {
        throw std::out_of_range("index is out of range");
    }
    return cells[i];
}

const std::vector< int >& Map::operator[](int i)const {
	if (i >= ySize) {
		throw std::out_of_range("index is out of range");
	}
	return cells[i];
}

void Map::AddBarrier( std::pair<int, int>& barrier )
{
	cells[barrier.second][barrier.first] = 1;
}

void Map::DeleteBarrier( std::pair<int, int>& barrier )
{
	cells[barrier.second][barrier.first] = 0;
}

int Map::orientedArea( int ax, int ay, int bx, int by, int cx, int cy ) const
{
	return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

bool Map::intersect(int a, int b, int c, int d) const {
	if (a > b) {
		std::swap(a, b);
	}

	if (c > d) {
		std::swap(c, d);
	}

	return std::max(a, c) <= std::min(b, d);
}

bool Map::intersectFinishLine(int xFirst, int yFirst, int xSecond, int ySecond) const {
	return (intersect(xFirst, xSecond, leftFinishPoint.first, rightFinishPoint.first)
		&& intersect(yFirst, ySecond, leftFinishPoint.second, rightFinishPoint.second)
		&& (orientedArea(xFirst, yFirst, xSecond, ySecond,
			leftFinishPoint.first, leftFinishPoint.second) * orientedArea(xFirst, yFirst,
				xSecond, ySecond, rightFinishPoint.first, rightFinishPoint.second) <= 0)
		&& (orientedArea(leftFinishPoint.first, leftFinishPoint.second, rightFinishPoint.first, rightFinishPoint.second,
			xFirst, yFirst) * orientedArea(leftFinishPoint.first, leftFinishPoint.second,
				rightFinishPoint.first, rightFinishPoint.second, xSecond, ySecond) <= 0));
}

bool Map::IsOnTheFinishLine( int x1, int y1, int x2, int y2 ) {
	return leftFinishPoint.second == rightFinishPoint.second
		&& y1 == leftFinishPoint.second && y2 <= y1
		&& leftFinishPoint.first <= x1 && x1 <= rightFinishPoint.first;
}

bool Map::IsOnRightSideOfFinishLine( int x, int y )
{
	return orientedArea( leftFinishPoint.first, leftFinishPoint.second,
		rightFinishPoint.first, rightFinishPoint.second, x, y ) > 0;
}

bool Map::wrongFinishLineIntersection(int xFirst, int yFirst, int xSecond, int ySecond) const {
	if( intersectFinishLine(xFirst, yFirst, xSecond, ySecond) ) {
		int normalVectorCoordinatesX = rightFinishPoint.first - leftFinishPoint.first;
		int normalVectorCoordinatesY = rightFinishPoint.second - leftFinishPoint.second;/*
		if( area( map.GetFinishLine().first, playersCoordinates, map.GetFinishLine().second ) *
			area( map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second ) >= 0 &&
			area( map.GetFinishLine().first, playersPreviousCoordinates, map.GetFinishLine().second ) *
			area( map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second ) < 0 )*/
		return !( orientedArea( leftFinishPoint.first, leftFinishPoint.second, xSecond, 
			ySecond, rightFinishPoint.first, rightFinishPoint.second )
			* orientedArea( leftFinishPoint.first, leftFinishPoint.second, normalVectorCoordinatesX,
			normalVectorCoordinatesY, rightFinishPoint.first, rightFinishPoint.second ) >= 0
			&& orientedArea( leftFinishPoint.first, leftFinishPoint.second, xFirst,
			yFirst, rightFinishPoint.first, rightFinishPoint.second )
			* orientedArea( leftFinishPoint.first, leftFinishPoint.second, normalVectorCoordinatesX,
			normalVectorCoordinatesY, rightFinishPoint.first, rightFinishPoint.second ) < 0);
	}

	return false;
	//if( intersectFinishLine( xFirst, yFirst, xSecond, ySecond ) ) {
	//	return orientedArea( xSecond, ySecond, leftFinishPoint.first, leftFinishPoint.second,
	//		rightFinishPoint.first, rightFinishPoint.second ) >= 0;
	//}

	//return false;
}

const std::pair< int, int >& Map::GetLeftFinishPoint() const {
	return leftFinishPoint;
}

const std::pair< int, int >& Map::GetRightFinishPoint() const {
	return rightFinishPoint;
}
