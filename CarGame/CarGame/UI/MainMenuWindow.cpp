#include <string>
#include <thread>

#include "resource.h"

#include "UIManager.h"
#include "UI/MainMenuWindow.h"
#include "UI/Drawing.h"

const wchar_t* const UI::CMainMenuWindow::className = L"CMainWindow";

bool UI::CMainMenuWindow::RegisterClass( HINSTANCE hInst )
{
	WNDCLASSEX tag;
	ZeroMemory( &tag, sizeof( WNDCLASSEX ) );
	tag.cbSize = sizeof( WNDCLASSEX );
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hCursor = LoadCursor( nullptr, IDC_ARROW );
	tag.hbrBackground = HBRUSH( GetStockObject( BLACK_BRUSH ) );
	tag.lpszMenuName = nullptr;
	tag.lpszClassName = className;
	tag.hInstance = ::GetModuleHandle( nullptr );
	tag.hIcon = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR ));
	tag.hIconSm = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ));

	return !RegisterClassEx( &tag );
}

UI::CMainMenuWindow::CMainMenuWindow( HINSTANCE hInst ) :
	handle( nullptr ),
	newGameButton( nullptr ),
	exitGameButton( nullptr ),
	manager( this, hInst )
{}

bool UI::CMainMenuWindow::Create()
{
	handle = CreateWindow( className, L"Main menu - Rock'n'Roll race", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		100, 100, 300, 300, nullptr, nullptr, ::GetModuleHandle( nullptr ), this );
	newGameButton = CreateWindow( L"BUTTON", L"New game", WS_VISIBLE | WS_CHILD, 75, 100, 150, 30,
		handle, HMENU(BUTTON_NEW_GAME), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	exitGameButton = CreateWindow( L"BUTTON", L"Exit game", WS_VISIBLE | WS_CHILD, 75, 150, 150, 30,
		handle, HMENU(BUTTON_EXIT), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );

	return handle != nullptr;
}

void UI::CMainMenuWindow::Destroy()
{
	manager.FinishUIThread();
	glutDestroyWindow( UI::CDrawing::GetWindow() );
	manager.GetUIThread()->join();
	::PostQuitMessage( 0 );
}

void UI::CMainMenuWindow::Show( int cmdShow )
{
	::ShowWindow( handle, cmdShow );
	::SetForegroundWindow( handle );
}

void UI::CMainMenuWindow::Play()
{
	manager.SwitchToSettings();
}

void UI::CMainMenuWindow::MakeVisible() const
{
	::ShowWindow( handle, SW_SHOW );
	//::SetForegroundWindow( handle );
}

void UI::CMainMenuWindow::MakeInvisible() const
{
	::ShowWindow( handle, SW_HIDE );
}

LRESULT UI::CMainMenuWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	CMainMenuWindow* wnd;
	if( message == WM_NCCREATE ) {
		wnd = static_cast<CMainMenuWindow*>(LPCREATESTRUCT( lParam )->lpCreateParams);
		::SetWindowLongPtr( handle, GWLP_USERDATA, LONG( LPCREATESTRUCT( lParam )->lpCreateParams ) );
		wnd->handle = handle;
	}
	wnd = reinterpret_cast<CMainMenuWindow*>(::GetWindowLong( handle, GWL_USERDATA ));

	switch( message ) {
		case WM_DESTROY:
			wnd->Destroy();
			return 0;
		case WM_COMMAND:
			if( LOWORD( wParam ) == wnd->BUTTON_NEW_GAME ) {
				wnd->Play();
			} else if( LOWORD( wParam ) == wnd->BUTTON_EXIT ) {
				::SendMessage( wnd->handle, WM_DESTROY, wParam, lParam );
			}
			return 0;
	}

	return ::DefWindowProc( handle, message, wParam, lParam );
}