#pragma once

#include <utility>

class IPlayerState {
public:
	virtual bool operator== (const IPlayerState &other) const = 0;
	virtual bool operator!= (const IPlayerState &other) const = 0;

	virtual void changePosition(std::pair< int, int > inputPosition) = 0;
	virtual void changeVelocityVector(std::pair< int, int > inputVelocityVector) = 0;
	virtual void changePositionUsingVelocityVector() = 0;
	virtual void dropVelocityVector() = 0;

	virtual int GetXVelocity() const = 0;
	virtual int GetYVelocity() const = 0;
	virtual int GetX() const = 0;
	virtual int GetY() const = 0;
	virtual void SetXVelocity(int xVelocity) = 0;
	virtual void SetYVelocity(int yVelocity) = 0;
	virtual void SetX(int x) = 0;
	virtual void SetY(int y) = 0;

	virtual std::pair< int, int > getPosition() const = 0;
	virtual std::pair< int, int > getVelocityVector() const = 0;
};