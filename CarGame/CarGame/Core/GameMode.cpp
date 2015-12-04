#include "Core/GameMode.h"

namespace Core {
	CGameMode::MovementMode CGameMode::movementMode = CONCURRENT;
	CGameMode::DeathPenalty CGameMode::deathPenalty = DESTROY;
	int CGameMode::lapCount = 1;
	CGameMode::ObjectRate CGameMode::objectRate = NO;
	CGameMode::ObjectChangeModel CGameMode::objectChangeModel = NO_CHANGE;

	void CGameMode::Init( MovementMode _movementMode, DeathPenalty _deathPenalty, int _lapCount, 
		ObjectRate _objectRate, ObjectChangeModel _objectChangeModel )
	{
		movementMode = _movementMode;
		deathPenalty = _deathPenalty;
		lapCount = _lapCount;
		objectRate = _objectRate;
		objectChangeModel = _objectChangeModel;
	}

	CGameMode::MovementMode CGameMode::GetMovementMode()
	{
		return movementMode;
	}

	CGameMode::DeathPenalty CGameMode::GetDeathPenalty()
	{
		return deathPenalty;
	}

	int CGameMode::GetLapCount()
	{
		return lapCount;
	}

	int CGameMode::GetMaxLapsCount()
	{
		return MAX_LAPS_COUNT;
	}

	CGameMode::ObjectRate CGameMode::GetObjectRate()
	{
		return objectRate;
	}

	CGameMode::ObjectChangeModel CGameMode::GetObjectChangeModel()
	{
		return objectChangeModel;
	}

}