#pragma once

#include <Windows.h>
#include <Core/Player.h>
#include "Core/GameMode.h"

class CUIManager;

namespace UI {
	class CMapSettingsWindow {
	public:
		static bool RegisterClass( HINSTANCE hInst );

		CMapSettingsWindow( CUIManager* manager );

		bool Create();
		void Destroy() const;

		void StartGame();
		void BackToMenu() const;
		void ChangeSettings();

		std::string GetMapName() const;
		std::vector<Core::CPlayer> GetPlayersInfo( const std::vector<Core::CCoordinates>& coordinates );
		
		void MakeVisible() const;
		void MakeInvisible() const;
	protected:
		bool CreateMapNameControl();
	private:
		HWND handle;
		HWND startGameButton;
		HWND backToMenuButton;
		HWND settingsButton;
		HWND mapNameControl;
		std::vector<HWND> positionOwnerControls;

		CUIManager* manager;

		const int BUTTON_START_GAME = 1;
		const int BUTTON_BACK_TO_MENU = 2;
		const int BUTTON_SETTINGS = 3;

		static const wchar_t* const className;

		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	};

	class CSettingsDialog {
	public:
		static void Init( HWND hwndDlg );
		static void OnDialogOk( HWND hwndDlg, WPARAM wParam );
		static void OnDialogCancel( HWND hwndDlg, WPARAM wParam );
		static int GetLapsNumber( HWND hwndDlg, int editId );
		static BOOL CALLBACK DialogSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
	};

	class CModeToItemIdConverter {
	public:
		static int getMoveItemId( Core::CGameMode::MovementMode mode );
		static int getCrashItemId( Core::CGameMode::DeathPenalty mode );
		static int getBarrierItemId( Core::CGameMode::ObjectRate mode );
	};
}