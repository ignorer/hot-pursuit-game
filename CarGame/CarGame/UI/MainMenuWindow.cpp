#include <string>
#include <thread>
#include <Windowsx.h>

#include "Resources/resource.h"

#include "UIManager.h"
#include "UI/MainMenuWindow.h"
#include "UI/Drawing.h"
#include "Utils.h"

//#pragma comment(lib, "Gdiplus.lib")

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
	tag.hCursor = LoadCursor( hInst, MAKEINTRESOURCE( IDC_CURSOR1 ) );
	tag.hbrBackground = CreatePatternBrush( LoadBitmap( hInst, MAKEINTRESOURCE( IDB_BITMAP1 ) ) );
	tag.lpszMenuName = nullptr;
	tag.lpszClassName = className;
	tag.hInstance = ::GetModuleHandle( nullptr );
	tag.hIcon = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR ));
	tag.hIconSm = reinterpret_cast<HICON> (::LoadImage( hInst, MAKEINTRESOURCE( IDI_MAIN_ICON ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ));

	return !RegisterClassEx( &tag );
}

UI::CMainMenuWindow::CMainMenuWindow( HINSTANCE hInst ) :
	handle( nullptr ),
	manager( this, hInst )
{}

bool UI::CMainMenuWindow::Create()
{
	handle = CreateWindow( className, L"Main menu - Rock'n'Roll racing", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		100, 100, 535, 515, nullptr, nullptr, ::GetModuleHandle( nullptr ), this );
	return handle != nullptr;
}

void UI::CMainMenuWindow::Destroy()
{
	glutDestroyWindow( UI::CDrawing::GetWindow() );
	manager.GetUIThread()->join();
	::PostQuitMessage( 0 );
}

void UI::CMainMenuWindow::Show( int cmdShow )
{
	::ShowWindow( handle, cmdShow );
	::SetForegroundWindow( handle );
	::UpdateWindow( handle );
}

void UI::CMainMenuWindow::Play()
{
	manager.SwitchToSettings();
}

void UI::CMainMenuWindow::MakeVisible() const
{
	::ShowWindow( handle, SW_SHOW );
}

void UI::CMainMenuWindow::MakeInvisible() const
{
	::ShowWindow( handle, SW_HIDE );
}

void UI::CMainMenuWindow::OnMapEditorExe() 
{
	ShellExecute( handle, nullptr, L".\\Race.exe", nullptr, nullptr, SW_SHOW );
	Destroy();
}

void UI::CMainMenuWindow::OnCreate()
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong( handle, GWL_HINSTANCE );
	bkgrdBrush = CreatePatternBrush( LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_BITMAP1 ) ) );
	newGameButton.curButtonImage = defButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\default_button.png").c_str() );
	hoverButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\hover.png").c_str() );
	pressedButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\pressed.png").c_str() );
	newGameButton.buttonRect = { 160, 240, 204, 61 };
	newGameButton.buttonName = L"NEW GAME";
	newGameButton.buttonNameRect = { 165, 245, 200, 60 };
	mapEditorButton.buttonName = L"MAP EDITOR";
	mapEditorButton.curButtonImage = defButtonImage;
	mapEditorButton.buttonRect = { 160, 310, 204, 61 };
	exitGameButton.buttonName = L"EXIT GAME";
	exitGameButton.curButtonImage = defButtonImage;
	exitGameButton.buttonRect = { 160, 380, 204, 61 };
}

void UI::CMainMenuWindow::OnPaint()
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

	graphics.DrawImage( newGameButton.curButtonImage, newGameButton.buttonRect );
	graphics.DrawImage( mapEditorButton.curButtonImage, mapEditorButton.buttonRect );
	graphics.DrawImage( exitGameButton.curButtonImage, exitGameButton.buttonRect );

	SetBkMode( newHdc, TRANSPARENT );
	SetTextColor( newHdc, RGB( 255, 255, 255 ) );
	SelectObject( newHdc, openSans );
	TextOut( newHdc, 222, 263, newGameButton.buttonName, 8 );
	TextOut( newHdc, 215, 333, mapEditorButton.buttonName, 10 );
	TextOut( newHdc, 220, 403, exitGameButton.buttonName, 9 );
	::BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );

	::SelectObject( newHdc, oldbitmap );
	::DeleteObject( bitmap );
	::DeleteObject( openSans );

	::DeleteDC( hdc );
	::DeleteDC( newHdc );

	::EndPaint( handle, &ps );
}

void UI::CMainMenuWindow::OnLButtonDown( int xMousePos, int yMousePos ) 
{
	if ( newGameButton.buttonRect.Contains( xMousePos, yMousePos ) ) {
		newGameButton.curButtonImage = pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
	else if ( mapEditorButton.buttonRect.Contains( xMousePos, yMousePos ) ) {
		mapEditorButton.curButtonImage = pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
	else if ( exitGameButton.buttonRect.Contains( xMousePos, yMousePos ) ) {
		exitGameButton.curButtonImage = pressedButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
	}
}

void UI::CMainMenuWindow::OnLButtonUp( int xMousePos, int yMousePos ) 
{
	if ( newGameButton.buttonRect.Contains( xMousePos, yMousePos ) ) {
		newGameButton.curButtonImage = defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		Play();
	}
	else if (mapEditorButton.buttonRect.Contains( xMousePos, yMousePos )) {
		mapEditorButton.curButtonImage = defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		OnMapEditorExe();
	}
	else if (exitGameButton.buttonRect.Contains( xMousePos, yMousePos )) {
		exitGameButton.curButtonImage = defButtonImage;
		::InvalidateRect( handle, NULL, FALSE );
		::UpdateWindow( handle );
		Destroy();
	}
}

void UI::CMainMenuWindow::OnMouseMove( int xMousePos, int yMousePos )
{
	if (newGameButton.buttonRect.Contains( xMousePos, yMousePos )) {
		newGameButton.curButtonImage = hoverButtonImage;
	} else {
		newGameButton.curButtonImage = defButtonImage;
	}
	if (mapEditorButton.buttonRect.Contains( xMousePos, yMousePos )) {
		mapEditorButton.curButtonImage = hoverButtonImage;
	}
	else {
		mapEditorButton.curButtonImage = defButtonImage;
	}
	if (exitGameButton.buttonRect.Contains( xMousePos, yMousePos )) {
		exitGameButton.curButtonImage = hoverButtonImage;
	}
	else {
		exitGameButton.curButtonImage = defButtonImage;
	}
	::InvalidateRect( handle, NULL, FALSE );
	::UpdateWindow( handle );
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
		case WM_CREATE:
			wnd->OnCreate();
			return 0;
		case WM_PAINT:
			wnd->OnPaint();
		case WM_COMMAND:
			return 0;
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