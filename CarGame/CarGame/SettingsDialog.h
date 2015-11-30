#include <Windows.h>
#include "Core\GameMode.h"


namespace UI {
	class CSettingsDialog {
	public:
		static void Init( HWND hwndDlg );
		static void OnDialogOk( HWND hwndDlg, WPARAM wParam );
		static void OnDialogCancel( HWND hwndDlg, WPARAM wParam );
		static void OnDialogLapsNumberEdit( HWND hwndDlg );
		static void OnDialogVscroll( HWND hwndDlg );
		static int GetLapsNumber( HWND hwndDlg, int editId );
		static BOOL CALLBACK DialogSettingsProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
		//специальный флаг, определ€ющий, посылаетс€ ли EN_CHANGE из UI-окна
		static bool sent;
	};

	class CModeToItemIdConverter {
	public:
		static int getMoveItemId( Core::CGameMode::MovementMode mode );
		static int getCrashItemId( Core::CGameMode::DeathPenalty mode );
		static int getBarrierItemId( Core::CGameMode::ObjectRate mode );
		static int getObjectChangeModelItemId( Core::CGameMode::ObjectChangeModel mode );
	};
}