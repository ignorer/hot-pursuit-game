#include <string>
#include <locale>
#include <codecvt>
#include <memory>
#include <Windowsx.h>


#include "Resources/resource.h"

#include "UIManager.h"
#include "Utils.h"
#include "Core/Reader.h"
#include "Core/Game.h"
#include "UI/MapSettingsWindow.h"
#include "UI/Drawing.h"
#include "UI/SettingsDialog.h"
#include "UI/ButtonUtils.h"

const wchar_t* const UI::CMapSettingsWindow::className = L"CMapSettingsWindow";
bool UI::CSettingsDialog::sent = 0;

bool UI::CMapSettingsWindow::RegisterClass( HINSTANCE hInst )
{
	WNDCLASSEX tag;
	tag.cbSize = sizeof( WNDCLASSEX );
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hCursor = LoadCursor( hInst, MAKEINTRESOURCE( IDC_CURSOR1 ) );
	tag.hbrBackground = HBRUSH( CreatePatternBrush( LoadBitmap( hInst, MAKEINTRESOURCE( IDB_BITMAP3 ) ) ) );
	tag.lpszMenuName = nullptr;
	tag.lpszClassName = className;
	tag.hInstance = ::GetModuleHandle( nullptr );
	tag.hIcon = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR ));
	tag.hIconSm = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ));

	return !RegisterClassEx( &tag );
}

UI::CMapSettingsWindow::CMapSettingsWindow( CUIManager* _manager ) :
	handle( nullptr ),
	startGameButton( nullptr ),
	backToMenuButton( nullptr ),
	settingsButton( nullptr ), 
	mapNameControl( nullptr ),
	positionOwnerControls( std::vector<HWND>( 12, nullptr ) ),
	nameControls( std::vector<HWND> (12, nullptr) ),
	manager( _manager )
{}

bool UI::CMapSettingsWindow::Create()
{
	handle = CreateWindow( className, L"Map settings - Rock'n'Roll racing", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		200, 200, 535, 515, nullptr, nullptr, ::GetModuleHandle( nullptr ), this );

	CreateMapNameControl();
	
	HCURSOR cursor = LoadCursor( HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), MAKEINTRESOURCE( IDC_CURSOR1 ) );
	auto openSans = ::CreateFont( 18, 0, 0, 0, 1000, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Open Sans" );
	::SendMessage( mapNameControl, WM_SETFONT, WPARAM( openSans ), TRUE );

	auto mapText = CreateWindow( L"Static", L"CHOOSE MAP", WS_VISIBLE | WS_CHILD | SS_LEFT, 310, 160, 200, 20,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	auto playersText = CreateWindow( L"Static", L"CHOOSE PLAYERS", WS_VISIBLE | WS_CHILD | SS_LEFT, 23, 40, 200, 20,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	auto typeText = CreateWindow( L"Static", L"Type:", WS_VISIBLE | WS_CHILD | SS_LEFT, 23, 80, 200, 20,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	auto nameText = CreateWindow( L"Static", L"Name:", WS_VISIBLE | WS_CHILD | SS_LEFT, 135, 80, 200, 20,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );

	::SendMessage( mapText, WM_SETFONT, WPARAM( openSans ), TRUE );
	::SendMessage( playersText, WM_SETFONT, WPARAM( openSans ), TRUE );
	::SendMessage( typeText, WM_SETFONT, WPARAM( openSans ), TRUE );
	::SendMessage( nameText, WM_SETFONT, WPARAM( openSans ), TRUE );

	for( int i = 0; i < positionOwnerControls.size(); ++i ) {
		positionOwnerControls[i] = CreateWindow( L"COMBOBOX", (std::wstring( L"Position " ) + std::to_wstring( i + 1 )).c_str(),
			CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL, 23, 100 + 30 * i, 107, 80,
			handle, HMENU( FIRST_POSITION_OWNER + i ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
		SetClassLong( positionOwnerControls[i], GCL_HCURSOR, LONG(cursor) );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"None" ) );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"Player" ) );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"AI" ) );
		::SendMessage( positionOwnerControls[i], CB_SELECTSTRING, 0, i == 0? LPARAM( L"Player" ) : LPARAM( L"None" ) );
		::SendMessage( positionOwnerControls[i], WM_SETFONT, WPARAM( openSans ), TRUE );
		nameControls[i] = CreateWindow( L"EDIT", (std::wstring( L"Name " ) + std::to_wstring( i + 1 )).c_str(),
		 WS_VISIBLE | WS_CHILD, 135, 100 + 30 * i, 133, 24,
			handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
		SetClassLong( nameControls[i], GCL_HCURSOR, LONG(cursor) );
		::SendMessage( nameControls[i], WM_SETFONT, WPARAM( openSans ), TRUE );

	}

	/*startGameButton = CreateWindow( L"BUTTON", L"Start game", WS_VISIBLE | WS_CHILD, 310, 260, 200, 60,
		handle, HMENU( BUTTON_START_GAME ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	settingsButton = CreateWindow( L"BUTTON", L"Settings", WS_VISIBLE | WS_CHILD, 310, 330, 200, 60,
		handle, HMENU( BUTTON_SETTINGS ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	backToMenuButton = CreateWindow( L"BUTTON", L"Back to main menu", WS_VISIBLE | WS_CHILD, 310, 400, 200, 60,
		handle, HMENU( BUTTON_BACK_TO_MENU ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );

	::SendMessage( startGameButton, WM_SETFONT, WPARAM( openSans ), TRUE );
	::SendMessage( settingsButton, WM_SETFONT, WPARAM( openSans ), TRUE );
	::SendMessage( backToMenuButton, WM_SETFONT, WPARAM( openSans ), TRUE );*/

	return handle != nullptr;
}

bool UI::CMapSettingsWindow::CreateMapNameControl()
{
	mapNameControl = CreateWindow( L"COMBOBOX", L"Map", CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL, 305, 190, 205, 160,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );

	HCURSOR cursor = LoadCursor( HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), MAKEINTRESOURCE( IDC_CURSOR1 ) );
	SetClassLong( mapNameControl, GCL_HCURSOR, LONG(cursor) );

	WIN32_FIND_DATA findFileData;
	HANDLE mapFile = ::FindFirstFile( (RESOURCE_DIRECTORY_W + L"\\Maps\\*.txt").c_str(), &findFileData );
	if( mapFile != INVALID_HANDLE_VALUE ) {
		std::wstring mapFileName;
		std::wstring mapName;
		do {
			mapFileName = std::wstring( findFileData.cFileName );
			mapName = mapFileName.substr( 0, mapFileName.size() - 4 );
			::SendMessage( mapNameControl, CB_ADDSTRING, 0, LPARAM( mapName.c_str() ) );
		} while( ::FindNextFile( mapFile, &findFileData ) != 0 );
		::SendMessage( mapNameControl, CB_SELECTSTRING, 0, LPARAM( mapName.c_str() ) );
		return ::FindClose( mapFile );
	}
	return mapNameControl != nullptr;
}

void UI::CMapSettingsWindow::Destroy() const
{
	glutDestroyWindow( UI::CDrawing::GetWindow() );
	manager->GetUIThread()->join();
	::PostQuitMessage( 0 );
}


int UI::CMapSettingsWindow::GetPositionOwnersNumber() {
	return positionOwnerControls.size();
}

void UI::CMapSettingsWindow::StartGame()
{
	try {
		Core::CReader reader;
		std::string mapName = GetMapName();
		Core::CMap map = reader.ReadMap( RESOURCE_DIRECTORY + "Maps\\" + mapName + ".txt" );
		std::vector<Core::CPlayer> playersInfo = GetPlayersInfo( map.GetStartPoints() );
		Core::CGame game( map, playersInfo, manager );
		manager->SwitchToGame();
		game.Start();
	} catch( std::exception& e ) {
		if( std::string( "Can't open file" ) == e.what() ) {
			::MessageBox( handle, L"Map not found", L"You're doing it wrong", MB_ICONHAND );
		} else if( std::string( "Empty Player Name" ) == e.what() ) {
			::MessageBox( handle, L"Please, enter player name", L"Empty Player Name", MB_ICONHAND );
		} else if( std::string( e.what( ) ).substr( 0, 17 ) == "Invalid cell type" ) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring message = converter.from_bytes( e.what() );
			::MessageBox( handle, message.c_str(), L"Invalid cell type", MB_ICONHAND );
		} else {
			::MessageBeep( SOUND_SYSTEM_BEEP );
		}
	}
}

void UI::CMapSettingsWindow::BackToMenu() const
{
	manager->SwitchToMainMenu();
}

void UI::CMapSettingsWindow::ChangeSettings()
{
	::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_DIALOG1 ),
		handle, static_cast<DLGPROC>( CSettingsDialog::DialogSettingsProc) );
}

std::string UI::CMapSettingsWindow::GetMapName() const
{
	const size_t MAX_LENGTH = 1024;
	std::shared_ptr<wchar_t> text = std::shared_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	::GetWindowText( mapNameControl, text.get( ), MAX_LENGTH );
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	return cv.to_bytes( text.get( ) );
}

std::vector<Core::CPlayer> UI::CMapSettingsWindow::GetPlayersInfo( const std::vector<Core::CCoordinates>& coordinates )
{
	int playerNumber = 0;
	std::vector<Core::CPlayer> result;
	const size_t MAX_LENGTH = 1024;
	std::shared_ptr<wchar_t> text = std::shared_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	for( int i = 0; i < min(12, coordinates.size()); ++i ) {
		::GetWindowText( positionOwnerControls[i], text.get(), MAX_LENGTH );
		std::wstring textString( text.get() );
		std::shared_ptr<wchar_t> name = std::shared_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
		int symbCount = ::GetWindowText( nameControls[i], name.get(), MAX_LENGTH );
		if ( textString != L"None" ) {
			std::wstring nameString;
			if (symbCount == 0) {
				throw std::invalid_argument( "Empty Player Name" );
			}
			else {
				nameString = name.get();
			}
			if (textString == L"AI") {
				result.push_back( Core::CPlayer( coordinates[i], playerNumber++, AI, nameString ) );
			}
			else if (textString == L"Player") {
				result.push_back( Core::CPlayer( coordinates[i], playerNumber++, USER, nameString ) );
			}
		}
	}
	return result;
}

void UI::CMapSettingsWindow::MakeVisible() const
{
	::ShowWindow( handle, SW_SHOW );
}

void UI::CMapSettingsWindow::MakeInvisible() const
{
	::ShowWindow( handle, SW_HIDE );

}

void UI::CMapSettingsWindow::OnCreate()
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE );
	bkgrdBrush = CreatePatternBrush( LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_BITMAP3 ) ) );
	buttonImages = new ButtonImages();
	startGameButton = new ButtonInfo();
	settingsButton = new ButtonInfo();
	backToMenuButton = new ButtonInfo();
	startGameButton->curButtonImage = buttonImages->defButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\default_button.png").c_str() );
	buttonImages->hoverButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\hover.png").c_str() );
	buttonImages->pressedButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\pressed.png").c_str() );
	startGameButton->buttonRect = { 305, 255, 204, 61 };
	startGameButton->buttonName = L"START GAME";
	startGameButton->buttonNameRect = { 305, 240, 200, 60 };
	settingsButton->buttonName = L"SETTINGS";
	settingsButton->curButtonImage = buttonImages->defButtonImage;
	settingsButton->buttonRect = { 305, 325, 204, 61 };
	backToMenuButton->buttonName = L"BACK";
	backToMenuButton->curButtonImage = buttonImages->defButtonImage;
	backToMenuButton->buttonRect = { 305, 395, 204, 61 };
}

void UI::CMapSettingsWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint( handle, &ps );
	HDC newHdc = ::CreateCompatibleDC( hdc );
	RECT rect;
	::GetClientRect( handle, &rect );
	HFONT openSans = ::CreateFont( 18, 0, 0, 0, 1000, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Open Sans" );
	HBITMAP bitmap = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	HGDIOBJ oldbitmap = ::SelectObject( newHdc, bitmap );
	FillRect( newHdc, &ps.rcPaint, bkgrdBrush );

	Gdiplus::Graphics graphics( newHdc );

	graphics.DrawImage( startGameButton->curButtonImage, startGameButton->buttonRect );
	graphics.DrawImage( settingsButton->curButtonImage, settingsButton->buttonRect );
	graphics.DrawImage( backToMenuButton->curButtonImage, backToMenuButton->buttonRect );

	SetBkMode( newHdc, TRANSPARENT );
	SetTextColor( newHdc, RGB( 255, 255, 255 ) );
	SelectObject( newHdc, openSans );
	TextOut( newHdc, 357, 278, startGameButton->buttonName, 10 );
	TextOut( newHdc, 368, 348, settingsButton->buttonName, 8 );
	TextOut( newHdc, 383, 418, backToMenuButton->buttonName, 4 );
	::BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );

	::SelectObject( newHdc, oldbitmap );
	::DeleteObject( bitmap );
	::DeleteObject( openSans );

	::DeleteDC( hdc );
	::DeleteDC( newHdc );

	::EndPaint( handle, &ps );
}

void UI::CMapSettingsWindow::OnLButtonDown( int xMousePos, int yMousePos )
{
	if( startGameButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		startGameButton->curButtonImage = buttonImages->pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
	else if( settingsButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		settingsButton->curButtonImage = buttonImages->pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
	else if( backToMenuButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		backToMenuButton->curButtonImage = buttonImages->pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
}

void UI::CMapSettingsWindow::OnLButtonUp( int xMousePos, int yMousePos )
{
	if( startGameButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		startGameButton->curButtonImage = buttonImages->defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		StartGame();
	}
	else if( settingsButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		settingsButton->curButtonImage = buttonImages->defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		ChangeSettings();
	}
	else if( backToMenuButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		backToMenuButton->curButtonImage = buttonImages->defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		BackToMenu();
	}
}

void UI::CMapSettingsWindow::OnMouseMove( int xMousePos, int yMousePos )
{
	if( startGameButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		startGameButton->curButtonImage = buttonImages->hoverButtonImage;
	}
	else {
		startGameButton->curButtonImage = buttonImages->defButtonImage;
	}
	if( settingsButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		settingsButton->curButtonImage = buttonImages->hoverButtonImage;
	}
	else {
		settingsButton->curButtonImage = buttonImages->defButtonImage;
	}
	if( backToMenuButton->buttonRect.Contains( xMousePos, yMousePos ) ) {
		backToMenuButton->curButtonImage = buttonImages->hoverButtonImage;
	}
	else {
		backToMenuButton->curButtonImage = buttonImages->defButtonImage;
	}
	::InvalidateRect( handle, NULL, FALSE );
	::UpdateWindow( handle );
}

LRESULT UI::CMapSettingsWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	CMapSettingsWindow* wnd;
	if( message == WM_NCCREATE ) {
		wnd = static_cast<CMapSettingsWindow*>(LPCREATESTRUCT( lParam )->lpCreateParams);
		::SetWindowLong( handle, GWL_USERDATA, LONG( LPCREATESTRUCT( lParam )->lpCreateParams ) );
		wnd->handle = handle;
	}
	wnd = reinterpret_cast<CMapSettingsWindow*>( ::GetWindowLong( handle, GWL_USERDATA ) );
	HDC hdc;
	switch( message ) {
		case WM_DESTROY:
			wnd->Destroy();
			return 0;
		case WM_COMMAND:
			if ( wParam == wnd->BUTTON_START_GAME ) {
				wnd->StartGame();
			} else if ( wParam == wnd->BUTTON_BACK_TO_MENU ) {
				wnd->BackToMenu();
			}
			else if (wParam == wnd->BUTTON_SETTINGS) {
				wnd->ChangeSettings();
			}
			return 0;
		case WM_CTLCOLORSTATIC:
			hdc = (HDC)wParam;
			SetTextColor( hdc, RGB( 0, 0, 0 ) );
			SetBkMode( hdc, TRANSPARENT );

			return (LRESULT)GetStockObject( NULL_BRUSH );
		case WM_CREATE:
			wnd->OnCreate();
			return 0;
		case WM_PAINT:
			wnd->OnPaint();
		case WM_LBUTTONDOWN:
			wnd->OnLButtonDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		case WM_LBUTTONUP:
			wnd->OnLButtonUp( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		case WM_MOUSEMOVE:
			wnd->OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		case WM_ERASEBKGND:
			return 1;
	}

	return ::DefWindowProc( handle, message, wParam, lParam );
}