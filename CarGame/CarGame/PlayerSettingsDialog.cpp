#include <string>
#include <memory>

#include "PlayerSettingsDialog.h"
#include "UI/MapSettingsWindow.h"


int UI::CPlayerSettingsDialog::playerNumber = 1;
HWND UI::CPlayerSettingsDialog::posOwner = nullptr;

void UI::CPlayerSettingsDialog::Init( HWND hwndDlg )
{
	HWND dialogEditCtrl = ::GetDlgItem( hwndDlg, EDIT_ID );
	::SetWindowText( dialogEditCtrl, ( std::wstring( L"Player" ) + std::to_wstring( playerNumber ) ).c_str() );
	++playerNumber;
}

void UI::CPlayerSettingsDialog::OnDialogOk( HWND hwndDlg, WPARAM wParam )
{
	const size_t MAX_LENGTH = 1024;
	std::unique_ptr<wchar_t> text = std::unique_ptr<wchar_t>( new wchar_t[MAX_LENGTH] );
	int symbCount = ::GetDlgItemText( hwndDlg, EDIT_ID, text.get(), MAX_LENGTH );
	if (symbCount != 0) {
		::SendMessage( posOwner, CB_ADDSTRING, 0, LPARAM( text.get() ) );
		::SendMessage( posOwner, CB_SELECTSTRING, 0, LPARAM( text.get() ) );
	}
	else {
		::SendMessage( posOwner, CB_ADDSTRING, 0,
			LPARAM( (std::wstring( L"Player" ) + std::to_wstring( playerNumber - 1 )).c_str() ) );
		::SendMessage( posOwner, CB_SELECTSTRING, 0,
			LPARAM( (std::wstring( L"Player" ) + std::to_wstring( playerNumber - 1 )).c_str() ) );
	}
	::EndDialog( hwndDlg, wParam );
}

void UI::CPlayerSettingsDialog::OnDialogCancel( HWND hwndDlg, WPARAM wParam )
{
	::EndDialog( hwndDlg, wParam );
}


BOOL CALLBACK UI::CPlayerSettingsDialog::PlayerSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
		case WM_INITDIALOG:
			CPlayerSettingsDialog::Init( hwndDlg );
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD( wParam ))
			{
				case IDOK:
					CPlayerSettingsDialog::OnDialogOk( hwndDlg, wParam );
					return TRUE;
				case IDCANCEL:
					CPlayerSettingsDialog::OnDialogCancel( hwndDlg, wParam );
					return TRUE;
				default:
					return FALSE;
			}
	}

	return FALSE;
}
