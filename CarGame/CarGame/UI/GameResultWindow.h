#pragma once

#include <Windows.h>

class CUIManager;


namespace UI {
	//class ButtonInfo;
	//class ButtonImages;

	class CGameResultWindow {
	public:
		static bool RegisterClass( HINSTANCE hInst );

		CGameResultWindow( CUIManager* manager );

		bool Create();
		void Destroy() const;

		void MakeVisible() const;
		void MakeInvisible() const;
		void SetWinners( const std::vector<Core::CPlayer>& winners ) const;
	protected:
		void OnCreate();
		void OnPaint();
		void OnLButtonDown( int xMousePos, int yMousePos );
		void OnLButtonUp( int xMousePos, int yMousePos );
		void OnMouseMove( int xMousePos, int yMousePos );
	private:
		HWND handle;
		/*HWND toMainMenuButton;
		HWND toSettingsButton;
		HWND exitButton;*/
		HWND resultMessage;

		HBRUSH bkgrdBrush;

		CUIManager* manager;

		static const wchar_t* const className;

		const int BUTTON_MAIN_MENU = 1;
		const int BUTTON_SETTINGS = 2;
		const int BUTTON_EXIT = 3;

		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	};
}