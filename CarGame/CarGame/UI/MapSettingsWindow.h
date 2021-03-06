﻿#pragma once

#include <Windows.h>
//#include <gdiplus.h>
#include "Core/Player.h"
#include "Core/GameMode.h"
#include <utility>
//#include "UI/ButtonUtils.h"

class CUIManager;
namespace UI {
	class ButtonInfo;
	class ButtonImages;
	class CComboBox;

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
		void OnCreate();
		void OnPaint();
		void OnLButtonDown( int xMousePos, int yMousePos );
		void OnLButtonUp( int xMousePos, int yMousePos );
		void OnMouseMove( int xMousePos, int yMousePos );
		
	private:
		HWND handle;
		HWND mapNameControl;
		std::vector<HWND> positionOwnerControls;
		std::vector<HWND> nameControls;
		std::vector<CComboBox*> comboBoxes;
		CComboBox* chooseMapComboBox;

		HBRUSH bkgrdBrush;

		ButtonInfo* startGameButton;
		ButtonInfo* settingsButton;
		ButtonInfo* backToMenuButton;
		ButtonImages* buttonImages;
		ButtonImages* b;

		CUIManager* manager;

		const int BUTTON_START_GAME = 1;
		const int BUTTON_BACK_TO_MENU = 2;
		const int BUTTON_SETTINGS = 3;
		const int FIRST_POSITION_OWNER = 4;
		//id каждого следующего position owner'а на 1 отличаются от предыдущего 

		static const wchar_t* const className;

		static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
		void changeHoveredButton( ButtonInfo* button, int xMousePos, int yMousePos );
	};
}