#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <GdiUtils.h>

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

	protected:
		void OnMapEditorExe();
		void OnCreate();
		void OnPaint();
		void OnLButtonDown( int xMousePos, int yMousePos );
		void OnLButtonUp( int xMousePos, int yMousePos );
		void OnMouseMove( int xMousePos, int yMousePos );
	private:
		HWND handle;
		HBRUSH bkgrdBrush;

		ButtonInfo newGameButton;
		ButtonInfo mapEditorButton;
		ButtonInfo exitGameButton;
		Gdiplus::Image* defButtonImage;
		Gdiplus::Image* hoverButtonImage;
		Gdiplus::Image* pressedButtonImage;

		CUIManager manager;

		static const wchar_t* const className;
		
		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
	};
}