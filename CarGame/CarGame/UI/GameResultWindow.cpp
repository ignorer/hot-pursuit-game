#include <GL/glut.h>

#include "Resources/resource.h"

#include "UIManager.h"
#include "UI/GameResultWindow.h"
#include "UI/Drawing.h"

const wchar_t* const UI::CGameResultWindow::className = L"CGameResultWindow";

bool UI::CGameResultWindow::RegisterClass( HINSTANCE hInst )
{
	WNDCLASSEX tag;
	tag.cbSize = sizeof( WNDCLASSEX );
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hCursor = LoadCursor( hInst, MAKEINTRESOURCE( IDC_CURSOR1 ) );
	tag.hbrBackground = HBRUSH( CreatePatternBrush( LoadBitmap( hInst, MAKEINTRESOURCE( IDB_BITMAP2 ) ) ) );
	tag.lpszMenuName = nullptr;
	tag.lpszClassName = className;
	tag.hInstance = ::GetModuleHandle( nullptr );
	tag.hIcon = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR ));
	tag.hIconSm = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ));

	return !RegisterClassEx( &tag );
}

UI::CGameResultWindow::CGameResultWindow( CUIManager* _manager ) :
	handle( nullptr ),
	toMainMenuButton( nullptr ),
	toSettingsButton( nullptr ),
	exitButton( nullptr ),
	resultMessage( nullptr ),
	manager( _manager )
{}

bool UI::CGameResultWindow::Create()
{
	handle = CreateWindow( className, L"Game result - Rock'n'Roll racing", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		200, 200, 300, 300, nullptr, nullptr, ::GetModuleHandle( nullptr ), this );

	HCURSOR cursor = LoadCursor( HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), MAKEINTRESOURCE( IDC_CURSOR1 ) );
	resultMessage = CreateWindow( L"Static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 50, 200, 50,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	SetClassLong( resultMessage, GCL_HCURSOR, (LONG)cursor );

	toSettingsButton = CreateWindow( L"BUTTON", L"Play again", WS_VISIBLE | WS_CHILD, 75, 130, 150, 30,
		handle, HMENU( BUTTON_SETTINGS ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	SetClassLong( toSettingsButton, GCL_HCURSOR, (LONG)cursor );

	toMainMenuButton = CreateWindow( L"BUTTON", L"To main menu", WS_VISIBLE | WS_CHILD, 75, 180, 150, 30,
		handle, HMENU( BUTTON_MAIN_MENU ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	SetClassLong( toMainMenuButton, GCL_HCURSOR, (LONG)cursor );

	exitButton = CreateWindow( L"BUTTON", L"Exit", WS_VISIBLE | WS_CHILD, 75, 230, 150, 30,
		handle, HMENU( BUTTON_EXIT ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	SetClassLong( exitButton, GCL_HCURSOR, (LONG)cursor );

	return handle != nullptr;
}

void UI::CGameResultWindow::Destroy() const
{
	glutDestroyWindow( UI::CDrawing::GetWindow() );
	manager->GetUIThread()->join();
	::PostQuitMessage( 0 );
}

void UI::CGameResultWindow::MakeVisible() const
{
	::ShowWindow( handle, SW_SHOW );
	//::SetForegroundWindow( handle );
}

void UI::CGameResultWindow::MakeInvisible() const
{
	::ShowWindow( handle, SW_HIDE );
}

void UI::CGameResultWindow::SetWinners( const std::vector<Core::CPlayer>& winners ) const
{
	if( winners.size() == 0 ) {
		::SetWindowText( resultMessage, L"All players are dead." );
	} else if( winners.size() == 1 ) {
		::SetWindowText( resultMessage, (winners[0].GetName() + L" is winner! Congratulations!!!").c_str() );
	} else {
		std::wstring text( L"Players " );
		for( int i = 0; i < winners.size() - 1; ++i ) {
			text += winners[i].GetName() + L", ";
		}
		text += winners.back().GetName();
		text += L" are winners! Congratulations!!!";
		::SetWindowText( resultMessage, text.c_str() );
	}
}

LRESULT UI::CGameResultWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	CGameResultWindow* wnd;
	if( message == WM_NCCREATE ) {
		wnd = static_cast<CGameResultWindow*>(LPCREATESTRUCT( lParam )->lpCreateParams);
		::SetWindowLong( handle, GWL_USERDATA, LONG( LPCREATESTRUCT( lParam )->lpCreateParams ) );
		wnd->handle = handle;
	}
	wnd = reinterpret_cast<CGameResultWindow*>(::GetWindowLong( handle, GWL_USERDATA ));

	switch( message ) {
		case WM_DESTROY:
			wnd->Destroy();
			return 0;

		case WM_COMMAND:
			if( LOWORD( wParam ) == wnd->BUTTON_MAIN_MENU ) {
				wnd->manager->SwitchToMainMenu();
			} else if( LOWORD( wParam ) == wnd->BUTTON_SETTINGS ) {
				wnd->manager->SwitchToSettings();
			} else if( LOWORD( wParam ) == wnd->BUTTON_EXIT ) {
				::SendMessage( wnd->handle, WM_DESTROY, wParam, lParam );
			}
			return 0;
	}

	return ::DefWindowProc( handle, message, wParam, lParam );
}