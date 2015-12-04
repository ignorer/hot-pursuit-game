#ifdef RACE_AI_EXPORTS
#define RACE_AI_API __declspec(dllexport) 
#else
#define RACE_AI_API __declspec(dllimport) 
#endif

#include <vector>
#include <memory>

#include "IPlayerState.h"
#include "IStrategy.h"

extern "C" RACE_AI_API IStrategy* GetNewStrategy( 
	const std::vector< std::vector < int > > &inputCells,
	const std::pair< int, int > &_leftFinishPoint,
	const std::pair< int, int > &_rightFinishPoint,
	int lapsCount,
	std::shared_ptr<IPlayerState> playerState );

// Factories to export PlayerState and Map classes
extern "C" RACE_AI_API IPlayerState* GetPlayerState( int x, int y, int xVelocity, int yVelocity );