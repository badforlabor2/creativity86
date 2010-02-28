
#ifndef WINDOW_H
#define WINDOW_H

#include "THWin32.h"

extern HINSTANCE g_hInstanceWindow;


inline void MakeWindowClassName(TCHAR *result, const TCHAR *className)
{
	thStrcpy(result, TEXT("__"));
	thStrcat(result, className);
}

#define DECLARE_ABSTRACT_WINDOWCLASS(cls) /*抽象函数*/ 

#define DECLARE_WINDOWCLASS(cls, parentcls, pkg) \
public: \
void GetWindowClassName(TCHAR *result) {MakeWindowClassName(result, cls);} \
~cls() {MaybeDestroy()} \
virtual const TCHAR *GetPkgName() {return TEXT(##pkg);}

#define IMPLEMENT_WINDOWCLASS(cls, cls_style) \
{\
	TCHAR temp[256];\
	MakeWindowClassName(temp, #cls);\
	cls::RegisteWindowClass(temp, cls_style);\
}


class THWindow
{	
	DECLARE_ABSTRACT_WINDOWCLASS(THWindow)
public:
	THWindow(THWindow *OwnerWindow = NULL)
		:	wndParent(OwnerWindow),
			hSelf(NULL),
			bShow(0)
	{
	
	}

public:
	virtual void GetWindowClassName(TCHAR *Result) = 0;
	RECT GetClientRect() const;
	RECT GetWindowRect(BOOL bConvert) const;
	POINT ClientToScreen(const POINT &point);
	POINT ScreenToClient(const POINT &point);
	RECT ClientToScreen(const RECT &rect);
	RECT ScreenToClient(const RECT &rect);
	POINT GetCursorPos();
	void MoveWindow(RECT r, BOOL bRepaint);
	void MoveWindow(INT left, INT top, INT width, INT height, BOOL bRepaint);
	void ResizeWindow(INT width, INT height, BOOL bRepaint);
	virtual void Show(BOOL bShow);
	virtual void DoDestroy();
	virtual void OnSetFocus(HWND hLosingFocus) {}
	virtual void OnLoseFocus(HWND hGaininFocus) {}
	virtual void OnSize(DWORD flags, INT x, INT y) {}
	virtual void OnWindowPosChanging(INT *NewX, INT *NewY, INT *NewW, INT *NewH) {}
	virtual void OnMove(INT newX, int newY) {}
	virtual void OnCommand(INT command);
	virtual INT OnSysCommand(INT command);
	virtual void OnActivate(BOOL Active);
	virtual void OnChar(TCHAR ch) {}
	virtual void OnKeyDown(TCHAR ch) {}
	virtual void OnPaint() {}
	virtual void OnCreate() {}
	virtual void OnCut() {}
	virtual void OnCopy() {}
	virtual void OnCopyData(HWND hWndSender, COPYDATASTRUCT *DataCopy) {}
	virtual void OnPaste() {}
	virtual void OnShowWindow(BOOL bShow) {}
	virtual void OnUndo() {}
	virtual void OnEraseBackground() {}
	virtual BOOL OnSetCursor() {return 0;}
	virtual void OnVScroll(WPARAM wParam, LPARAM lParam) {}
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam) {}
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam) {}
	virtual void OnInitDialog() {}
	virtual void OnEnterIdle() {}
	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}
	virtual void OnMouseHover() {}
	virtual void OnReleaseCapture() {}
	virtual void OnTimer() {}
	virtual void OnMouseMove(DWORD flags, POINT location);
	virtual void OnLeftButtonDown() {}
	virtual void OnLeftButtonClick();
	virtual void OnLeftButtonDoubleClick() {}
	virtual void OnMiddleButtonDoubleClick() {}
	virtual void OnRightButtonClick();
	virtual void OnRightButtonDoubleClick() {}
	virtual void OnRightButtonDown() {}
	virtual void OnLeftButtonUp() {}
	virtual void OnRightButtonUp() {}
	virtual BOOL OnClose() ;
	virtual void OnDestroy();
	void SaveWindowPos();
	void MaybeDestroy();
	void CloseWindow();
	void SetFont(HFONT hFont);
	void VerifyPosition();
	void SetRedraw(BOOL redraw);
	void PerformCreateWindowEx(DWORD dwExStyle, 
			LPCTSTR lpWindowName, 
			DWORD dwStyle, 
			INT x, INT y,
			INT nWidth, INT nHeight,
			HWND hParent, HMENU hMenu,
			HINSTANCE hInstance);

public:
	virtual LONG WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual INT CallDefaultProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK StaticDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LONG APIENTRY StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static WNDPROC RegisteWindowClass(const TCHAR *Name, DWORD style); 

public:
	HWND hSelf;
	THWindow *wndParent;
	BOOL bShow;
	BOOL bDestroy;
	WORD nControlID;	//显示的样式ID
};

class THDialog : public THWindow
{
	DECLARE_ABSTRACT_WINDOWCLASS(THDialog);
public:
	THDialog() {}
	THDialog(INT dialogID, THWindow *ownerWindow)
		:	THWindow(ownerWindow)
	{
		nControlID = dialogID;
	}
	INT CallDefaultProc(UINT msg, WPARAM wparam, LPARAM lparam) {return 0;}
	virtual INT DoModal(HINSTANCE hInst = g_hInstanceWindow);
	void OpenChildWindow(INT controlID, BOOL visible);
	static BOOL CALLBACK LocalizeTextEnumProc(HWND hwnd, LPARAM lparam);
	virtual void LocalizedText(const TCHAR *section);
	virtual void OnInitDialog();
	void EndDialog(INT result);
	void EndDialogTrue();
	void EndDialogFalse();
	void CenterInOwnerWindow();
	virtual void Show(BOOL show);
	
private:
	
};

#endif