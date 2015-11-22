#include <string>
#include <locale>
#include <codecvt>

#include "resource.h"

#include "UIManager.h"
#include "GlobalDefinitions.h"
#include "Core/Reader.h"
#include "Core/Game.h"
#include "UI/MapSettingsWindow.h"
#include "UI/Drawing.h"
#include "Core/GameMode.h"

const wchar_t* const UI::CMapSettingsWindow::className = L"CMapSettingsWindow";

bool UI::CMapSettingsWindow::RegisterClass( HINSTANCE hInst )
{
	WNDCLASSEX tag;
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

UI::CMapSettingsWindow::CMapSettingsWindow( CUIManager* _manager ) :
	handle( nullptr ),
	startGameButton( nullptr ),
	backToMenuButton( nullptr ),
	settingsButton( nullptr ), 
	mapNameControl( nullptr ),
	positionOwnerControls( std::vector<HWND>( 12, nullptr ) ),
	manager( _manager )
{}

bool UI::CMapSettingsWindow::Create()
{
	handle = CreateWindow( className, L"Map settings - Rock'n'Roll race", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		200, 200, 400, 500, nullptr, nullptr, ::GetModuleHandle( nullptr ), this );
	
	CreateMapNameControl();

	for( int i = 0; i < positionOwnerControls.size(); ++i ) {
		positionOwnerControls[i] = CreateWindow( L"COMBOBOX", (std::wstring( L"Position " ) + std::to_wstring( i + 1 )).c_str(),
			CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL, 40, 100 + 30 * i, 125, 80,
			handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"None" ) );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"Player" ) );
		::SendMessage( positionOwnerControls[i], CB_ADDSTRING, 0, LPARAM( L"AI" ) );
		::SendMessage( positionOwnerControls[i], CB_SELECTSTRING, 0, LPARAM( L"None" ) );
	}
	
	startGameButton = CreateWindow( L"BUTTON", L"Start game", WS_VISIBLE | WS_CHILD, 225, 330, 150, 30,
		handle, HMENU( BUTTON_START_GAME ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	settingsButton = CreateWindow( L"BUTTON", L"Settings", WS_VISIBLE | WS_CHILD, 225, 380, 150, 30,
		handle, HMENU( BUTTON_SETTINGS ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	backToMenuButton = CreateWindow( L"BUTTON", L"Back to main menu", WS_VISIBLE | WS_CHILD, 225, 430, 150, 30,
		handle, HMENU( BUTTON_BACK_TO_MENU ), HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );
	
	return handle != nullptr;
}


bool UI::CMapSettingsWindow::CreateMapNameControl( ) {
	mapNameControl = CreateWindow( L"COMBOBOX", L"M", CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_VSCROLL, 40, 50, 125, 80,
		handle, nullptr, HINSTANCE( GetWindowLong( handle, GWL_HINSTANCE ) ), this );

	WIN32_FIND_DATA findFileData;
	HANDLE mapFile = ::FindFirstFile( L".\\Resources\\Maps\\*.txt", &findFileData );
	if (mapFile != INVALID_HANDLE_VALUE) {
		std::wstring mapFileName;
		std::wstring mapName;
		do {
			mapFileName = std::wstring( findFileData.cFileName );
			mapName = mapFileName.substr( 0, mapFileName.size( ) - 4 );
			::SendMessage( mapNameControl, CB_ADDSTRING, 0, LPARAM( mapName.c_str( ) ) );
		} while (::FindNextFile( mapFile, &findFileData ) != 0);
		::SendMessage( mapNameControl, CB_SELECTSTRING, 0, LPARAM( mapName.c_str( ) ) );
		return ::FindClose( mapFile );
	}
	return mapNameControl != nullptr;
}

void UI::CMapSettingsWindow::Destroy() const
{
	manager->FinishUIThread();
	glutDestroyWindow( UI::CDrawing::GetWindow() );
	manager->GetUIThread()->join();
	::PostQuitMessage( 0 );
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
		} else {
			::MessageBeep( SOUND_SYSTEM_BEEP );
			::PostQuitMessage( 1 );
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
		if( textString == L"Player" ) {
			result.push_back( Core::CPlayer( coordinates[i], playerNumber++, USER ) );
		} else if( textString == L"AI" ) {
			result.push_back( Core::CPlayer( coordinates[i], playerNumber++, AI ) );
		}
	}
	return result;
}

void UI::CMapSettingsWindow::MakeVisible() const
{
	::ShowWindow( handle, SW_SHOW );
	//::SetForegroundWindow( handle );

}

void UI::CMapSettingsWindow::MakeInvisible() const
{
	::ShowWindow( handle, SW_HIDE );

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
	}

	return ::DefWindowProc( handle, message, wParam, lParam );
}


void UI::CSettingsDialog::Init( HWND hwndDlg ) {
	Core::CGameMode::MovementMode initMovState = Core::CGameMode::GetMovementMode();
	Core::CGameMode::DeathPenalty initDeathPenalty = Core::CGameMode::GetDeathPenalty();
	Core::CGameMode::ObjectRate initBarrierRate = Core::CGameMode::GetObjectRate();

	::CheckRadioButton( hwndDlg, IDC_SEQUENTIAL_MOV, IDC_CONCURRENT_MOV, CModeToItemIdConverter::getMoveItemId( initMovState ) );
	::CheckRadioButton( hwndDlg, IDC_DESTROY_CRASH, IDC_STOP_CRASH, CModeToItemIdConverter::getCrashItemId( initDeathPenalty ) );
	::CheckRadioButton( hwndDlg, IDC_NO_BARRIERS, IDC_MANY_BARRIERS, CModeToItemIdConverter::getBarrierItemId( initBarrierRate ) );
	
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT1 );
	::SetWindowText( dialogEditCtrl, std::to_wstring( Core::CGameMode::GetLapCount() ).c_str() );
}


void UI::CSettingsDialog::OnDialogOk( HWND hwndDlg, WPARAM wParam )
{
	Core::CGameMode::MovementMode moveStatus = ( IsDlgButtonChecked ( hwndDlg, IDC_SEQUENTIAL_MOV ) == BST_CHECKED ) ?
												Core::CGameMode::SEQUENTIAL : Core::CGameMode::CONCURRENT;
	Core::CGameMode::DeathPenalty crashStatus;
	if ( IsDlgButtonChecked ( hwndDlg, IDC_DESTROY_CRASH ) == BST_CHECKED ) {
		crashStatus = Core::CGameMode::DESTROY;
	}
	else if ( IsDlgButtonChecked ( hwndDlg, IDC_START_CRASH ) == BST_CHECKED ) {
		crashStatus = Core::CGameMode::TO_START;
	}
	else {
		crashStatus = Core::CGameMode::STOP;
	}
	
	Core::CGameMode::ObjectRate barriersRate;
	if ( IsDlgButtonChecked ( hwndDlg, IDC_NO_BARRIERS ) == BST_CHECKED ) {
		barriersRate = Core::CGameMode::NO;
	}
	else if ( IsDlgButtonChecked ( hwndDlg, IDC_NORMAL_BARRIERS ) == BST_CHECKED ) {
		barriersRate = Core::CGameMode::NORMAL;
	}
	else if ( IsDlgButtonChecked ( hwndDlg, IDC_FEW_BARRIERS ) == BST_CHECKED ) {
		barriersRate = Core::CGameMode::FEW;
	}
	else {
		barriersRate = Core::CGameMode::MANY;
	}

	int lapCount = GetLapsNumber( hwndDlg, IDC_EDIT1 );
	Core::CGameMode::Init ( moveStatus, crashStatus, lapCount, barriersRate );
	::EndDialog( hwndDlg, wParam );
}

void UI::CSettingsDialog::OnDialogCancel( HWND hwndDlg, WPARAM wParam )
{
	::EndDialog( hwndDlg, wParam );
}

int UI::CSettingsDialog::GetLapsNumber( HWND hwndDlg, int editId )
{
	const size_t MAX_LENGTH = 1024;
	std::shared_ptr<wchar_t> text = std::shared_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	::GetDlgItemText( hwndDlg, editId, text.get(), MAX_LENGTH );
	return std::stoi( text.get() );
}

BOOL CALLBACK UI::CSettingsDialog::DialogSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_INITDIALOG:
		CSettingsDialog::Init( hwndDlg );
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD( wParam ))
		{
			case IDOK:
				CSettingsDialog::OnDialogOk( hwndDlg, wParam );
				return TRUE;
			case IDCANCEL:
				CSettingsDialog::OnDialogCancel( hwndDlg, wParam );
				return TRUE;
			default:
				return FALSE;
		}
	}
	return FALSE;
}

int UI::CModeToItemIdConverter::getMoveItemId( Core::CGameMode::MovementMode mode ) {
	return ( mode == Core::CGameMode::SEQUENTIAL )? IDC_SEQUENTIAL_MOV : IDC_CONCURRENT_MOV;
}

int UI::CModeToItemIdConverter::getCrashItemId( Core::CGameMode::DeathPenalty mode ) {
	int crashItemId = -1;
	switch (mode)
	{
	case Core::CGameMode::DESTROY:
		crashItemId = IDC_DESTROY_CRASH;
		break;
	case Core::CGameMode::TO_START:
		crashItemId = IDC_START_CRASH;
		break;
	case Core::CGameMode::STOP:
		crashItemId = IDC_STOP_CRASH;
		break;
	default:
		break;
	}
	return crashItemId;
}

int UI::CModeToItemIdConverter::getBarrierItemId( Core::CGameMode::ObjectRate mode ) {
	int barrierRateId = -1;
	switch (mode)
	{
		case Core::CGameMode::NO:
			barrierRateId = IDC_NO_BARRIERS;
			break;
		case Core::CGameMode::FEW:
			barrierRateId = IDC_FEW_BARRIERS;
			break;
		case Core::CGameMode::NORMAL:
			barrierRateId = IDC_NORMAL_BARRIERS;
			break;
		case Core::CGameMode::MANY:
			barrierRateId = IDC_MANY_BARRIERS;
			break;
		default:
			break;
	}
	return barrierRateId;
}