#include <Windows.h>

#include <CommCtrl.h>
#include <string>
#include <memory>

#include "SettingsDialog.h"
#include "Core\GameMode.h"
#include "resource.h"

void UI::CSettingsDialog::Init( HWND hwndDlg ) {
	HWND hSpin = ::GetDlgItem( hwndDlg, IDC_SPIN3 ); //Получаем дескрипторы окон
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );

	//Задаём приятельское окно
	SendMessage( hSpin, UDM_SETBUDDY, WPARAM( dialogEditCtrl ), 0 );

	//Задаём диапазон
	SendMessage( hSpin, UDM_SETRANGE, 0, MAKELONG( Core::CGameMode::GetMaxLapsCount(), 1 ) );

	//Задаём позицию
	SendMessage( hSpin, UDM_SETPOS, 0, Core::CGameMode::GetLapCount() );

	::SetWindowText( dialogEditCtrl, std::to_wstring( Core::CGameMode::GetLapCount() ).c_str() );


	Core::CGameMode::MovementMode initMovState = Core::CGameMode::GetMovementMode();
	Core::CGameMode::DeathPenalty initDeathPenalty = Core::CGameMode::GetDeathPenalty();
	Core::CGameMode::ObjectRate initBarrierRate = Core::CGameMode::GetObjectRate();
	Core::CGameMode::ObjectChangeModel initObjectChangeModel = Core::CGameMode::GetObjectChangeModel();


	::CheckRadioButton( hwndDlg, IDC_SEQUENTIAL_MOV, IDC_CONCURRENT_MOV, CModeToItemIdConverter::getMoveItemId( initMovState ) );
	::CheckRadioButton( hwndDlg, IDC_DESTROY_CRASH, IDC_STOP_CRASH, CModeToItemIdConverter::getCrashItemId( initDeathPenalty ) );
	::CheckRadioButton( hwndDlg, IDC_NO_BARRIERS, IDC_MANY_BARRIERS, CModeToItemIdConverter::getBarrierItemId( initBarrierRate ) );
	::CheckRadioButton( hwndDlg, IDC_NO_CHANGE, IDC_RANDOM, CModeToItemIdConverter::getObjectChangeModelItemId(initObjectChangeModel ) );
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
	::EndDialog( hwndDlg, wParam );
}

void UI::CSettingsDialog::OnDialogLapsNumberEdit( HWND hwndDlg )
{
	if (CSettingsDialog::sent){
		HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );
		int lapCount = GetLapsNumber( hwndDlg, IDC_EDIT2 );
		int maxLapsCount = Core::CGameMode::GetMaxLapsCount();
		if (lapCount > maxLapsCount) {
			HWND hSpin = ::GetDlgItem( hwndDlg, IDC_SPIN3 );
			LRESULT curPos = ::SendMessage( hSpin, UDM_SETPOS, 0, maxLapsCount );
			CSettingsDialog::sent = !CSettingsDialog::sent;
			::SetWindowText( dialogEditCtrl, std::to_wstring( maxLapsCount ).c_str() );
		}
	}
	else {
		CSettingsDialog::sent = !CSettingsDialog::sent;
	}
}

void UI::CSettingsDialog::OnDialogVscroll( HWND hwndDlg )
{
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, IDC_EDIT2 );
	HWND hSpin = ::GetDlgItem( hwndDlg, IDC_SPIN3 );
	LRESULT curPos = ::SendMessage( hSpin, UDM_GETPOS, 0, 0 );
	int pos = LOWORD( curPos );
	CSettingsDialog::sent = 0;
	::SetWindowText( dialogEditCtrl, std::to_wstring( LOWORD( curPos ) ).c_str() );
}

int UI::CSettingsDialog::GetLapsNumber( HWND hwndDlg, int editId )
{
	const size_t MAX_LENGTH = 1024;
	std::unique_ptr<wchar_t> text = std::unique_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	int symbCount = ::GetDlgItemText( hwndDlg, editId, text.get(), MAX_LENGTH );
	if (symbCount == 0) {
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
