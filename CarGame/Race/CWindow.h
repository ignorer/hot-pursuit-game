#pragma once

#include "stdafx.h"
#include "resource.h"
#include "toolbar.h"
#include "CCommandHandler.h"
#include "CMap.h"


class CApplicationInterface : public IUIApplication, public CComObjectRootEx<CComSingleThreadModel> {

public:
    BEGIN_COM_MAP( CApplicationInterface )
        COM_INTERFACE_ENTRY( IUIApplication )
    END_COM_MAP()

};

class CWindow : public CApplicationInterface {
public:
    friend CCommandHandler;

    CWindow();
    ~CWindow();

    static bool RegisterClass();
    bool Create();
    void Show( int cmdShow );
    HWND GetHandle();
    HWND GetHandleDialog();

    void SetHandle( HWND handle );
    //инициирует перерисовку окна
    void UpdateState();

    // Static method to create an instance of the object.
    static HRESULT CreateInstance( __deref_out IUIApplication **ppApplication );

    BType GetBrush();
    void SetBrush( BType bType );

    // IUnknown methods.
    STDMETHOD_( ULONG, AddRef() );
    STDMETHOD_( ULONG, Release() );
    STDMETHOD( QueryInterface( REFIID iid, void** ppv ) );

    // IUIApplication methods
    STDMETHOD( OnCreateUICommand )( UINT nCmdID,
        __in UI_COMMANDTYPE typeID,
        __deref_out IUICommandHandler** ppCommandHandler );

    STDMETHOD( OnViewChanged )( UINT viewId,
        __in UI_VIEWTYPE typeId,
        __in IUnknown* pView,
        UI_VIEWVERB verb,
        INT uReasonCode );

    STDMETHOD( OnDestroyUICommand )( UINT32 commandId,
        __in UI_COMMANDTYPE typeID,
        __in_opt IUICommandHandler* commandHandler );

protected:
    void OnDestroy();
    void OnSize();
    void OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    void OnClose();
    void OnPaint();
    void OnLButtonDown( LPARAM lParam );
    void OnMouseMove( LPARAM lParam );
    void OnLButtonUp( LPARAM lParam );
	void OnRButtonDown( LPARAM lParam );
	void OnRButtonUp( LPARAM lParam );
    void onDialogOk();
    void SaveFile();
    void LoadFile();
    void Draw( LPARAM lParam );
    static INT_PTR __stdcall dialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
    CMap map;
    HWND handle;
    HWND handleDlg;
    int cellSize;
    std::vector<HBRUSH> brushes;
    HBRUSH backgroundBrush;
    LONG m_cRef;                           // Reference count.
    IUICommandHandler* m_pCommandHandler;  // Generic Command Handler
    BType brush;
    BOOL lButtonPressed;
	BOOL rButtonPressed;

	std::pair<int, int> coordsOfCurrentView;
	std::pair<int, int> coordsRMouseButMoveStart;
	std::pair<int, int> coordsRMouseButMoveFinish;

	bool drawFirstTime;

    static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};
