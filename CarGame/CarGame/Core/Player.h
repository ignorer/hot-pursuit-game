#pragma once

#include "GlobalDefinitions.h"
#include <vector>

namespace Core {
	enum Direction {
		SW = 1, S, SE,
		W, C, E, // `C` for no direction
		NW, N, NE
	};

	class CPlayer {
	public:
		CPlayer( const CCoordinates& coordinates, size_t playerNumber, PlayersTypes playerType, std::wstring playerName );

		void Die();
		void GoToStart();
		void DecreaseLaps( int count = 1 );
		void IncreaseLaps( int count = 1 );

		void Move( Direction direction );
		bool DirectionIsValid( Direction direction, const CSize& size ) const;
		bool DirectionIsValid( CCoordinates direction, const CSize& size ) const;

		CCoordinates GetPosition() const;
		CCoordinates GetInertia() const;
		CCoordinates GetPreviousPosition() const;
		CCoordinates GetInitialPosition() const;
		size_t GetNumber() const;
		std::wstring GetName() const;
		PlayersTypes GetType() const;
		bool IsAlive() const;
		int GetLaps() const;
		int GetPenalty() const;
		int GetShield() const;

		bool operator< ( const CPlayer& player ) const;

		std::vector<CCoordinates> PossibleMoves( const CSize& size );
		void SetInertia( CCoordinates newInertia );
		void SetPenalty( int newPenalty );
		void ActivateShield();
		void DecreaseShield();
		void DropShield();

	private:
		CCoordinates position;
		CCoordinates inertia;
		CCoordinates initialPosition; // Для возвращения на старт, после столкновения с машиной
		CCoordinates previousPosition;
		bool isAlive;
		int lapCount;
		size_t number;
		std::wstring name;
		PlayersTypes type;
		int shieldLeft;
		int penalty;

		CCoordinates convertFromDirectionCode( Direction directionCode ) const;
	};
}