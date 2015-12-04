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
		int GetPositionOwnersNumber();
		
	private:
		HWND handle;
		HWND startGameButton;
		HWND backToMenuButton;
		HWND settingsButton;
		HWND mapNameControl;
		std::vector<HWND> positionOwnerControls;
		std::vector<HWND> nameControls;

		CUIManager* manager;

		const int BUTTON_START_GAME = 1;
		const int BUTTON_BACK_TO_MENU = 2;
		const int BUTTON_SETTINGS = 3;
		const int FIRST_POSITION_OWNER = 4;
		//id каждого следующего position owner'а на 1 отличаются от предыдущего 

		static const wchar_t* const className;

		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	};
}