//
//  CPlayerState.hpp
//  AI-ABBYY
//
//  Created by Alexander Danilyak on 21/09/15.
//  Copyright © 2015 Alexander Danilyak. All rights reserved.
//

#ifndef CPlayerState_hpp
#define CPlayerState_hpp

#include <iostream>
#include "IPlayerState.h"

class PlayerState : public IPlayerState {
private:
    std::pair< int, int > position;
    std::pair< int, int > velocityVector;

	int curLap = 0;
public:    
    PlayerState();
    PlayerState(std::pair< int, int > startPosition);
	PlayerState( int x, int y, int xVelocity, int yVelocity, int _curLap );
    
    ~PlayerState();

	bool operator== (const PlayerState &other) const;
	bool operator!= (const PlayerState &other) const;
	bool operator< (const PlayerState &other) const;
    
    void changePosition(std::pair< int, int > inputPosition);
    void changeVelocityVector(std::pair< int, int > inputVelocityVector);
    void changePositionUsingVelocityVector();
    void dropVelocityVector();

	void Move( int dx, int dy );

    int GetXVelocity() const;
    int GetYVelocity() const;
    int GetX() const;
    int GetY() const;
    void SetXVelocity(int xVelocity);
    void SetYVelocity(int yVelocity);
    void SetX(int x);
    void SetY(int y);
    
    std::pair< int, int > getPosition() const;
    std::pair< int, int > getVelocityVector() const;

	// Возвращает новое состояния после применения вектора скорости
	PlayerState GetNextState( int dx, int dy ) const;

	void IncreaseLapsCount();

	int GetCurLap() const;

	struct CPlayerStateHasher {
		std::size_t operator()( const PlayerState& state ) const
		{
			return std::hash<int>()(state.getPosition().first) + std::hash<int>()(state.getPosition().second) +
				std::hash<int>()(state.getVelocityVector().first) + std::hash<int>()(state.getVelocityVector().second) +
				std::hash<int>()(state.GetCurLap());
		}
	};
};

#endif /* CPlayerState_hpp */
