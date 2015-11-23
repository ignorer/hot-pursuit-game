#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "IPlayerState.h"

class IMap {
private:
	virtual void fillMapWithDefaultData() = 0;

	virtual bool intersect(int a, int b, int c, int d) const = 0;
	virtual int orientedArea(int ax, int ay, int bx, int by, int cx, int cy) const = 0;
	virtual bool wrongFinishLineIntersection(int xFirst, int yFirst, int xSecond, int ySecond) const = 0;

public:
	virtual const std::pair<size_t, size_t> size() const = 0;
	virtual const size_t sizeOnXaxis() const = 0;
	virtual const size_t sizeOnYaxis() const = 0;

	virtual std::vector< int > &operator[](int i) = 0;
	virtual const std::vector< int > &operator[](int i) const = 0;

	virtual bool canPlayerStayOnCell(int x, int y) const = 0;
	virtual bool hasBarrierOnPath(int xFirst, int yFirst, int xSecond, int ySecond) const = 0;
	virtual bool canPlayerStayOnCellLookOnOtherPlayers(int x, int y, int playerID, const std::vector< IPlayerState > &players) const = 0;
	virtual bool intersectFinishLine(int xFirst, int yFirst, int xSecond, int ySecond) const = 0;

	virtual const std::pair< int, int >& GetLeftFinishPoint() const = 0;
	virtual const std::pair< int, int >& GetRightFinishPoint() const = 0;

	virtual void print(const std::vector< IPlayerState > &players) const = 0;
};
