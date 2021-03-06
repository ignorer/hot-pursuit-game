﻿#pragma once

#include <Windows.h>

#include "Core\GameMode.h"
#include <utility>


struct colorStruct {
	COLORREF color;
	HBRUSH brush;
};

namespace UI {
	class ButtonImages;
	class ButtonInfo;

	class CSettingsDialog {
	public:
		static void Init( HWND hwndDlg );
		static void OnDialogOk( HWND hwndDlg, WPARAM wParam );
		static void OnDialogCancel( HWND hwndDlg, WPARAM wParam );
		static void OnDialogLapsNumberEdit( HWND hwndDlg );
		static void OnDialogVscroll( HWND hwndDlg );
		static void OnPaint( HWND hwndDlg );
		static void OnLButtonDown( HWND hwndDlg, int xMousePos, int yMousePos );
		static void OnLButtonUp( HWND hwndDlg, int xMousePos, int yMousePos, WPARAM wParam );
		static void OnMouseMove( HWND hwndDlg, int xMousePos, int yMousePos );
		static void changeHoveredButton( HWND hwndDlg, ButtonInfo* button, int xMousePos, int yMousePos );
		static int GetLapsNumber( HWND hwndDlg, int editId );
		static BOOL CALLBACK DialogSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
		//специальный флаг, определяющий, посылается ли EN_CHANGE из UI-окна
		static bool sent;
		static std::pair<COLORREF, HBRUSH> staticBrush;
		static std::pair<COLORREF, HBRUSH> editBrush;
		static HBRUSH bkgrdBrush;
		static ButtonImages* buttonImages;
		static ButtonInfo* okButton;
		static ButtonInfo* cancelButton;

	};

	class CModeToItemIdConverter {
	public:
		static int getMoveItemId( Core::CGameMode::MovementMode mode );
		static int getCrashItemId( Core::CGameMode::DeathPenalty mode );
		static int getBarrierItemId( Core::CGameMode::ObjectRate mode );
		static int getObjectChangeModelItemId( Core::CGameMode::ObjectChangeModel mode );
	};
}