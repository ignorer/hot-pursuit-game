#include <Windows.h>

#include "resource.h"


namespace UI {
	class CPlayerSettingsDialog {
	public:
		static void Init( HWND hwndDlg );
		static void OnDialogOk( HWND hwndDlg, WPARAM wParam );
		static void OnDialogCancel( HWND hwndDlg, WPARAM wParam );
		static BOOL CALLBACK PlayerSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
		static int playerNumber;
		static const int EDIT_ID = IDC_EDIT1;
		static HWND posOwner;
	};
}