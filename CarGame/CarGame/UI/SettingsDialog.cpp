#include <Windows.h>
#include <Windowsx.h>

#include <CommCtrl.h>
#include <string>
#include <memory>

#include "SettingsDialog.h"
#include "Core\GameMode.h"
#include "Resources/resource.h"
#include "UI\ButtonUtils.h"
#include "Utils.h"

std::pair<COLORREF, HBRUSH> UI::CSettingsDialog::staticBrush = 
							std::make_pair( RGB( 25, 27, 78 ), CreateSolidBrush( RGB( 25, 27, 78 ) ) );

std::pair<COLORREF, HBRUSH> UI::CSettingsDialog::editBrush =
							std::make_pair( RGB( 255, 255, 255 ), CreateSolidBrush( RGB( 255, 255, 255 ) ) );

HBRUSH UI::CSettingsDialog::bkgrdBrush = HBRUSH( GetStockObject( WHITE_BRUSH ) );
UI::ButtonInfo* UI::CSettingsDialog::okButton = nullptr;
UI::ButtonInfo* UI::CSettingsDialog::cancelButton = nullptr;
UI::ButtonImages* UI::CSettingsDialog::buttonImages = nullptr;

void UI::CSettingsDialog::Init( HWND hwndDlg ) {
	HWND hSpin = ::GetDlgItem( hwndDlg, IDC_SPIN3 ); //Получаем дескрипторы окон
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );

	bkgrdBrush = CreatePatternBrush( LoadBitmap( HINSTANCE( GetWindowLong( hwndDlg, GWL_HINSTANCE ) ), MAKEINTRESOURCE( IDB_BITMAP4 ) ) );
	auto openSans = ::CreateFont( 18, 0, 0, 0, 1000, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Open Sans" );
	
	//Задаем вид курсора
	HCURSOR cursor = LoadCursor( HINSTANCE( GetWindowLong( hwndDlg, GWL_HINSTANCE ) ), MAKEINTRESOURCE( IDC_CURSOR1 ) );
	SetClassLong( hwndDlg, GCL_HCURSOR, LONG(cursor) );
	SetClassLong( dialogEditCtrl, GCL_HCURSOR, LONG(cursor) );
	SetClassLong( hSpin, GCL_HCURSOR, LONG(cursor) );

	//Задаём приятельское окно
	SendMessage( hSpin, UDM_SETBUDDY, WPARAM( dialogEditCtrl ), 0 );

	//Задаём диапазон
	SendMessage( hSpin, UDM_SETRANGE, 0, MAKELONG( Core::CGameMode::GetMaxLapsCount(), 1 ) );

	//Задаём позицию
	SendMessage( hSpin, UDM_SETPOS, 0, Core::CGameMode::GetLapCount() );

	::SetWindowText( dialogEditCtrl, std::to_wstring( Core::CGameMode::GetLapCount() ).c_str() );
	::SendMessage( dialogEditCtrl, WM_SETFONT, WPARAM( openSans ), TRUE );

	Core::CGameMode::MovementMode initMovState = Core::CGameMode::GetMovementMode();
	Core::CGameMode::DeathPenalty initDeathPenalty = Core::CGameMode::GetDeathPenalty();
	Core::CGameMode::ObjectRate initBarrierRate = Core::CGameMode::GetObjectRate();
	Core::CGameMode::ObjectChangeModel initObjectChangeModel = Core::CGameMode::GetObjectChangeModel();


	::CheckRadioButton( hwndDlg, IDC_SEQUENTIAL_MOV, IDC_CONCURRENT_MOV, CModeToItemIdConverter::getMoveItemId( initMovState ) );
	::CheckRadioButton( hwndDlg, IDC_DESTROY_CRASH, IDC_STOP_CRASH, CModeToItemIdConverter::getCrashItemId( initDeathPenalty ) );
	::CheckRadioButton( hwndDlg, IDC_NO_BARRIERS, IDC_MANY_BARRIERS, CModeToItemIdConverter::getBarrierItemId( initBarrierRate ) );
	::CheckRadioButton( hwndDlg, IDC_NO_CHANGE, IDC_RANDOM, CModeToItemIdConverter::getObjectChangeModelItemId(initObjectChangeModel ) );

	okButton = new ButtonInfo();
	cancelButton = new ButtonInfo();
	buttonImages = new ButtonImages();
	buttonImages->defButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\default_button.png").c_str() );
	buttonImages->hoverButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\hover.png").c_str() );
	buttonImages->pressedButtonImage = new Gdiplus::Image( (RESOURCE_DIRECTORY_W + L"\\Images\\pressed.png").c_str() );
	okButton->curButtonImage = buttonImages->defButtonImage;
	okButton->buttonRect = { 200, 200, 204, 61 };
	cancelButton->curButtonImage = buttonImages->defButtonImage;
	cancelButton->buttonRect = { 200, 300, 204, 61 };
}

void UI::CSettingsDialog::OnDialogOk( HWND hwndDlg, WPARAM wParam )
{
	Core::CGameMode::MovementMode moveStatus = (IsDlgButtonChecked( hwndDlg, IDC_SEQUENTIAL_MOV ) == BST_CHECKED) ?
		Core::CGameMode::MovementMode::SEQUENTIAL : Core::CGameMode::MovementMode::CONCURRENT;
	Core::CGameMode::DeathPenalty crashStatus;
	if (IsDlgButtonChecked( hwndDlg, IDC_DESTROY_CRASH ) == BST_CHECKED) {
		crashStatus = Core::CGameMode::DeathPenalty::DESTROY;
	}
	else if (IsDlgButtonChecked( hwndDlg, IDC_START_CRASH ) == BST_CHECKED) {
		crashStatus = Core::CGameMode::DeathPenalty::TO_START;
	}
	else {
		crashStatus = Core::CGameMode::DeathPenalty::STOP;
	}

	Core::CGameMode::ObjectRate barriersRate;
	if (IsDlgButtonChecked( hwndDlg, IDC_NO_BARRIERS ) == BST_CHECKED) {
		barriersRate = Core::CGameMode::ObjectRate::NO;
	}
	else if (IsDlgButtonChecked( hwndDlg, IDC_NORMAL_BARRIERS ) == BST_CHECKED) {
		barriersRate = Core::CGameMode::ObjectRate::NORMAL;
	}
	else if (IsDlgButtonChecked( hwndDlg, IDC_FEW_BARRIERS ) == BST_CHECKED) {
		barriersRate = Core::CGameMode::ObjectRate::FEW;
	}
	else {
		barriersRate = Core::CGameMode::ObjectRate::MANY;
	}

	Core::CGameMode::ObjectChangeModel changeModel;
	if (IsDlgButtonChecked( hwndDlg, IDC_NO_CHANGE ) == BST_CHECKED) {
		changeModel = Core::CGameMode::ObjectChangeModel::NO_CHANGE;
	}
	else if (IsDlgButtonChecked( hwndDlg, IDC_STABLE ) == BST_CHECKED) {
		changeModel = Core::CGameMode::ObjectChangeModel::STABLE;
	}
	else {
		changeModel = Core::CGameMode::ObjectChangeModel::RANDOM;
	}

	int lapCount = GetLapsNumber( hwndDlg, IDC_EDIT2 );
	Core::CGameMode::Init( moveStatus, crashStatus, lapCount, barriersRate, changeModel );
	::EndDialog( hwndDlg, wParam );
}

void UI::CSettingsDialog::OnDialogCancel( HWND hwndDlg, WPARAM wParam )
{
	//::DeleteObject( staticBrush.second );
	//::DeleteObject( editBrush.second );
	::EndDialog( hwndDlg, wParam );
}

void UI::CSettingsDialog::OnDialogLapsNumberEdit( HWND hwndDlg )
{
	if( sent ) {
		HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );
		int lapCount = GetLapsNumber( hwndDlg, IDC_EDIT2 );
		int maxLapsCount = Core::CGameMode::GetMaxLapsCount();
		if( lapCount > maxLapsCount ) {
			sent = !sent;
			::SetWindowText( dialogEditCtrl, std::to_wstring( maxLapsCount ).c_str() );
		}
	} else {
		sent = !sent;
	}
}

void UI::CSettingsDialog::OnDialogVscroll( HWND hwndDlg )
{
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );
	HWND hSpin = ::GetDlgItem( hwndDlg, IDC_SPIN3 );
	LRESULT curPos = ::SendMessage( hSpin, UDM_GETPOS, 0, 0 );
	sent = 0;
	::SetWindowText( dialogEditCtrl, std::to_wstring( LOWORD( curPos ) ).c_str() );
}

void UI::CSettingsDialog::OnPaint( HWND hwndDlg )
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint( hwndDlg, &ps );
	HDC newHdc = ::CreateCompatibleDC( hdc );
	RECT rect;
	::GetClientRect( hwndDlg, &rect );
	HFONT openSans = ::CreateFont( 18, 0, 0, 0, 1000, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Open Sans" );
	HBITMAP bitmap = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	HGDIOBJ oldbitmap = ::SelectObject( newHdc, bitmap );
	FillRect( newHdc, &ps.rcPaint, bkgrdBrush );

	Gdiplus::Graphics graphics( newHdc );

	graphics.DrawImage( okButton->curButtonImage, okButton->buttonRect );
	graphics.DrawImage( cancelButton->curButtonImage, cancelButton->buttonRect );

	SetBkMode( newHdc, TRANSPARENT );
	SetTextColor( newHdc, RGB( 255, 255, 255 ) );
	SelectObject( newHdc, openSans );
	//TextOut
	::BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );

	::SelectObject( newHdc, oldbitmap );
	::DeleteObject( bitmap );
	::DeleteObject( openSans );

	::DeleteDC( hdc );
	::DeleteDC( newHdc );

	::EndPaint( hwndDlg, &ps );
}

void  UI::CSettingsDialog::OnLButtonDown( HWND hwndDlg, int xMousePos, int yMousePos )
{
	if (okButton->buttonRect.Contains( xMousePos, yMousePos )) {
		okButton->curButtonImage = buttonImages->pressedButtonImage;
		::InvalidateRect( hwndDlg, NULL, FALSE );
		::UpdateWindow( hwndDlg );
	}
	else if ( cancelButton->buttonRect.Contains( xMousePos, yMousePos )) {
		cancelButton->curButtonImage = buttonImages->pressedButtonImage;
		::InvalidateRect( hwndDlg, NULL, FALSE );
		::UpdateWindow( hwndDlg );
	}
}

void  UI::CSettingsDialog::OnLButtonUp( HWND hwndDlg, int xMousePos, int yMousePos )
{
	if (okButton->buttonRect.Contains( xMousePos, yMousePos )) {
		okButton->curButtonImage = buttonImages->defButtonImage;
		::InvalidateRect( hwndDlg, NULL, FALSE );
		::UpdateWindow( hwndDlg );
	}
	else if (cancelButton->buttonRect.Contains( xMousePos, yMousePos )) {
		cancelButton->curButtonImage = buttonImages->defButtonImage;
		::InvalidateRect( hwndDlg, NULL, FALSE );
		::UpdateWindow( hwndDlg );
	}
}

void  UI::CSettingsDialog::OnMouseMove( HWND hwndDlg, int xMousePos, int yMousePos )
{
	UI::CSettingsDialog::changeHoveredButton( hwndDlg, okButton, xMousePos, yMousePos );
	UI::CSettingsDialog::changeHoveredButton( hwndDlg, cancelButton, xMousePos, yMousePos );
	::UpdateWindow( hwndDlg );
}
void UI::CSettingsDialog::changeHoveredButton( HWND hwndDlg, ButtonInfo* button, int xMousePos, int yMousePos ) {
	RECT rectToChange;
	if (button->buttonRect.Contains( xMousePos, yMousePos )) {
		button->curButtonImage = buttonImages->hoverButtonImage;
	}
	else {
		button->curButtonImage = buttonImages->defButtonImage;
	}
	rectToChange.bottom = button->buttonRect.GetBottom();
	rectToChange.left = button->buttonRect.GetLeft();
	rectToChange.top = button->buttonRect.GetTop();
	rectToChange.right = button->buttonRect.GetRight();
	::InvalidateRect( hwndDlg, &rectToChange, FALSE );
}

int UI::CSettingsDialog::GetLapsNumber( HWND hwndDlg, int editId )
{
	const size_t MAX_LENGTH = 1024;
	std::unique_ptr<wchar_t> text = std::unique_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	int symbCount = ::GetDlgItemText( hwndDlg, editId, text.get(), MAX_LENGTH );
	if( symbCount == 0 ) {
		return -1;
	}
	return std::stoi( text.get() );
}

int UI::CModeToItemIdConverter::getMoveItemId( Core::CGameMode::MovementMode mode ) {
	return (mode == Core::CGameMode::SEQUENTIAL) ? IDC_SEQUENTIAL_MOV : IDC_CONCURRENT_MOV;
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

int  UI::CModeToItemIdConverter::getObjectChangeModelItemId( Core::CGameMode::ObjectChangeModel mode )
{
	int changeModelItemId = -1;
	switch (mode)
	{
		case Core::CGameMode::ObjectChangeModel::NO_CHANGE:
			changeModelItemId = IDC_NO_CHANGE;
			break;
		case Core::CGameMode::ObjectChangeModel::STABLE:
			changeModelItemId = IDC_STABLE;
			break;
		case Core::CGameMode::ObjectChangeModel::RANDOM:
			changeModelItemId = IDC_RANDOM;
			break;
		default:
			break;
	}
	return changeModelItemId;
}


BOOL CALLBACK UI::CSettingsDialog::DialogSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HBRUSH brush;
	switch (message)
	{
		case WM_INITDIALOG:
			CSettingsDialog::Init( hwndDlg );
			return TRUE;
		case WM_CTLCOLOREDIT:
			::SetBkColor( reinterpret_cast<HDC>(wParam), editBrush.first );
			//SetBkMode( reinterpret_cast<HDC>( wParam ), TRANSPARENT );
			//return (LRESULT)GetStockObject( NULL_BRUSH );
			return INT_PTR( editBrush.second );
		case WM_CTLCOLORSTATIC:
			//brush = CreateSolidBrush( RGB( 255, 27, 78 ) );
			//::SetBkColor( reinterpret_cast<HDC>(wParam), staticBrush.first );
			//return INT_PTR( staticBrush.second );
			SetBkMode( reinterpret_cast<HDC>( wParam ), TRANSPARENT );
			return (LRESULT)GetStockObject( NULL_BRUSH );
		case WM_CTLCOLORDLG:
			//SetBkMode( reinterpret_cast<HDC>( wParam ), TRANSPARENT );
			//return (LRESULT)GetStockObject( NULL_BRUSH );
			return (INT_PTR)bkgrdBrush;
		case WM_PAINT:
			CSettingsDialog::OnPaint( hwndDlg );
		case WM_LBUTTONDOWN:
			CSettingsDialog::OnLButtonDown( hwndDlg, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		case WM_LBUTTONUP:
			CSettingsDialog::OnLButtonUp( hwndDlg, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;
		case WM_MOUSEMOVE:
			CSettingsDialog::OnMouseMove( hwndDlg, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
		case WM_COMMAND:
			switch (LOWORD( wParam ))
			{
				case IDOK:
					CSettingsDialog::OnDialogOk( hwndDlg, wParam );
					return TRUE;
				case IDCANCEL:
					CSettingsDialog::OnDialogCancel( hwndDlg, wParam );
					return TRUE;
				case IDC_EDIT2:
					if (HIWORD( wParam ) == EN_CHANGE) {
						CSettingsDialog::OnDialogLapsNumberEdit( hwndDlg );
					}
					return TRUE;
				default:
					return FALSE;
			}
		case WM_VSCROLL:
			CSettingsDialog::OnDialogVscroll( hwndDlg );
			return TRUE;
	}

	return FALSE;
}
