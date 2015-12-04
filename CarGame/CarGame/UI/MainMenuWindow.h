#pragma once

#include <Windows.h>

namespace UI {
	class CMapSettingsWindow;
	class CGameResultWindow;

	class CMainMenuWindow {
	public:
		static bool RegisterClass( HINSTANCE hInst );

		CMainMenuWindow( HINSTANCE hInst );

		bool Create();
		void Destroy();

		void Show( int cmdShow );
		void Play();

		void MakeVisible() const;
		void MakeInvisible() const;

	private:
		HWND handle;
		HWND newGameButton;
		HWND exitGameButton;

		CUIManager manager;

		const int BUTTON_NEW_GAME = 1;
		const int BUTTON_EXIT = 2;

		static const wchar_t* const className;
		
		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	};
}