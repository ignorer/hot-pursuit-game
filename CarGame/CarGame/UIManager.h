#pragma once

#include <set>
#include <vector>
#include <thread>
#include <map>
#include <Windows.h>

#include "UI/MapSettingsWindow.h"
#include "UI/GameResultWindow.h"

namespace Core {
	struct CCoordinates;
	class CPlayer;
	class CMap;
}

namespace UI {
	class CMainMenuWindow;
}

class CUIManager {
public:
	CUIManager( UI::CMainMenuWindow* mainMenuWindow, HINSTANCE hInst );

	int GetDirection( const std::vector<Core::CCoordinates>& possibleMoves, Core::CCoordinates inertia, Core::CCoordinates position ) const;
	void InitMap( const Core::CMap& map, const std::vector<Core::CPlayer>& players, const Core::CLine& finish ) const;
	void UpdatePlayersInfo( const std::vector<Core::CPlayer>& movedPlayers ) const;
	void ShowCrashesAndRespawn( const std::set<Core::CPlayer*>& collidedPlayers ) const;
	void ShowCrashes( const std::set<Core::CPlayer*>& crashedPlayers ) const;
	void ShowPowerups( const std::map<Core::CCoordinates, PowerupType>& powerups ) const;
	void ShowShots( const std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>>& shots, bool needDelay ) const;
	void ShowGameResult( const std::vector<Core::CPlayer>& winners ) const;
	void SwitchToMainMenu() const;
	void SwitchToSettings() const;
	void SwitchToGame() const;
	void SwitchToResults() const;
	std::thread* GetUIThread();
	void FinishGame() const;
	void FinishUIThread() const;
	void MarkPossibleMoves(const std::vector<Core::CCoordinates>& possibleMoves) const;
	void UnMarkPossibleMoves(const std::vector<Core::CCoordinates>& possibleMoves) const;

private:
	UI::CMainMenuWindow* mainMenuWindow;
	UI::CMapSettingsWindow mapSettingsWindow;
	UI::CGameResultWindow gameResultWindow;

	std::thread uiThread;
};