#pragma once

#include <set>
#include <memory>
#include <map>

#include "Core/Map.h"
#include "Core/Player.h"
#include "Core/PowerupManager.h"
#include "UIManager.h"
#include "AI/IPlayerState.h"
#include "AI/IStrategy.h"

namespace Core {
	typedef IStrategy*(__cdecl *STRATEGY_PROC)(
		const std::vector< std::vector < int > > &inputCells,
		const std::pair< int, int > &_leftFinishPoint,
		const std::pair< int, int > &_rightFinishPoint,
		int lapsCount,
		std::shared_ptr<IPlayerState> playerState );

	typedef IPlayerState*(__cdecl *PLAYER_STATE_FACTORY_PROC)(int x, int y, int xVelocity, int yVelocity);

	class CGame {
	public:
		CGame( const CMap& map, const std::vector<CPlayer>& playersInfo, const CUIManager* _manager );

		void Start();

	private:
		CMap map;
		std::vector<CPlayer> players;
		const CUIManager* manager;
		CPowerupManager powerupManager;
		std::map<int, IStrategy*> AIStrategies;

		HINSTANCE hInstanceDLLLibrary;
		STRATEGY_PROC StrategyBuilderFunc;
		PLAYER_STATE_FACTORY_PROC GetPlayerStateFunc;

		void handleFinishLineIntersections();
		// требование к возвращаемому значению:
		//	-1 - если пересекли финишную черту в неправильную сторону
		//	0 - не пересекли
		//	1 - пересекли черту в правильную сторону
		int finishLineIntersectsWithPlayer( const CPlayer& player ) const;
		
		bool playerOutOfTrack( const CPlayer& player ) const;
		void handleCrashes( const std::set<CPlayer*>& crashedPlayers, int& deadPlayersCount ) const;
		void findCollisions( std::set<CPlayer*>& crashedPlayers );
		void findCollisionsForPlayer( int playerId, std::set<CPlayer*>& crashedPlayers );
		void findCrashes( std::set<CPlayer*>& crashedPlayers );
		void findCrashesForPlayer( CPlayer& player, std::set<CPlayer*>& crashedPlayers ) const;
		void findWinners( std::vector<CPlayer>& winners ) const;
		void finish( const std::vector<CPlayer>& winners ) const;

		void turnOfPlayer( CPlayer& player, std::set<CPlayer*>& crashedPlayers );
		int turnOfUser(CPlayer& player);
		void initAI( CPlayer* player );
	};
}