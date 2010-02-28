#include <windows.h>

#include <commctrl.h>
#include <richedit.h>

#include "THWin32.h"
#include "Window.h"


///全局变量
HINSTANCE g_hInstanceWindow;

///获得窗口的Client区域
RECT THWindow::GetClientRect() const
{
	RECT rect;
	::GetClientRect(hSelf, &rect);
	return rect;
}
///获得窗口所占区域
RECT THWindow::GetWindowRect(BOOL bConvert) const
{
	RECT r;
	::GetWindowRect(hSelf, &r);
	if(bConvert)
		return wndParent?wndParent->ScreenToClient(r):r;
	else
		return r;
}

void THWindow::MoveWindow(RECT r, BOOL bRepaint)
{
	::MoveWindow(hSelf, r.left, r.top, r.right-r.left, r.bottom-r.top, bRepaint);
}
void THWindow::MoveWindow(INT left, INT top, INT width, INT height, BOOL bRepaint)
{
	::MoveWindow(hSelf, left, top, width, height, bRepaint);
}
void THWindow::ResizeWindow(INT width, INT height, BOOL bRepaint)
{
	RECT r;
	::GetWindowRect(hSelf, &r);
	::MoveWindow(hSelf, r.left, r.top, r.right-r.left, r.bottom-r.top, bRepaint);
}


///窗口区域与屏幕区域的坐标转化
POINT THWindow::ClientToScreen(const POINT &point)
{
	POINT P;
	P.x = point.x;
	P.y = point.y;
	::ClientToScreen(hSelf, &P);
	return P;
}
POINT THWindow::ScreenToClient(const POINT &point)
{
	POINT P;
	P.x = point.x;
	P.y = point.y;
	::ScreenToClient(hSelf, &P);
	return P;
}
RECT THWindow::ClientToScreen(const RECT &rect)
{
	POINT p, p2;
	p.x = rect.left;
	p.y = rect.top;
	p = ClientToScreen(p);
	p2.x = rect.right - rect.left;
	p2.y = rect.bottom - rect.top;
	p2 = ClientToScreen(p2);
	RECT r;
	r.left = p.x;
	r.top = p.y;
	r.right = p.x + p2.x;
	r.bottom = p.y + p2.y;
	return r;

}
RECT THWindow::ScreenToClient(const RECT &rect)
{
	POINT p, p2;
	p.x = rect.left;
	p.y = rect.top;
	p = ScreenToClient(p);
	p2.x = rect.right - rect.left;
	p2.y = rect.bottom - rect.top;
	p2 = ScreenToClient(p2);
	RECT r;
	r.left = p.x;
	r.top = p.y;
	r.right = p.x + p2.x;
	r.bottom = p.y + p2.y;
	return r;
}

///获得鼠标在窗口中的坐标
POINT THWindow::GetCursorPos()
{
	POINT mouse;
	::GetCursorPos(&mouse);
	return ScreenToClient(mouse);
}

//SW_HIDE   隐藏窗口，活动状态给令一个窗口     
//SW_MINIMIZE   最小化窗口，活动状态给令一个窗口     
//SW_RESTORE   用原来的大小和位置显示一个窗口，同时令其进入活动状态     
//SW_SHOW   用当前的大小和位置显示一个窗口，同时令其进入活动状态     
//SW_SHOWMAXIMIZED   最大化窗口，并将其激活     
//SW_SHOWMINIMIZED   最小化窗口，并将其激活     
//SW_SHOWMINNOACTIVE   最小化一个窗口，同时不改变活动窗口     
//SW_SHOWNA   用当前的大小和位置显示一个窗口，不改变活动窗口     
//SW_SHOWNOACTIVATE   用最近的大小和位置显示一个窗口，同时不改变活动窗口     
//SW_SHOWNORMAL   与SW_RESTORE相
void THWindow::Show(BOOL show)
{
	bShow = show;
	::ShowWindow(hSelf, show?(::IsIconic(hSelf) ? SW_RESTORE : SW_SHOW) : SW_HIDE);
}

void THWindow::DoDestroy()
{
	if(hSelf)
		::DestroyWindow(hSelf);
	hSelf = NULL;
}
BOOL THWindow::OnClose()
{
	//如果是MDI,那么执行SendMessage( OwnerWindow->hWnd, WM_MDIDESTROY, (WPARAM)hWnd, 0 )
	if(hSelf){
		::DestroyWindow(hSelf);
		hSelf = NULL;
	}
	return true;
}
void THWindow::OnActivate(BOOL Active)
{
	VerifyPosition();
}
void THWindow::SaveWindowPos()
{

}
void THWindow::MaybeDestroy()
{
	if(!bDestroy){
		DoDestroy();
		bDestroy = 1;
	}
}
void THWindow::CloseWindow()
{
	THCheck(hSelf);
	DestroyWindow(hSelf);
}
void THWindow::SetFont(HFONT hFont)
{
	::SendMessage(hSelf, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(0, 0));
}

///如果窗口没在屏幕上，那么使得窗口显示在左上角
void THWindow::VerifyPosition()
{
	RECT r;
	::GetWindowRect(hSelf, &r);
	RECT screen;
	screen.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	screen.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	screen.right = ::GetSystemMetrics(SM_CXVIRTUALSCREEN) + screen.left;
	screen.bottom = ::GetSystemMetrics(SM_CYVIRTUALSCREEN) + screen.top;

	if(r.left > screen.right || r.left < screen.left)
		r.left = 0;
	if(r.top > screen.bottom || r.top < screen.top)
		r.bottom = 0;

	//把窗口拉到最前端
	//调用这个参数时，SWP_NOSIZE表示不改变大小，即忽略cx、cy的值
	//SWP_NOSENDCHANGING表示不发送ChangePos的消息了。
	::SetWindowPos(hSelf, HWND_TOP,
		r.left, r.top, r.right-r.left, r.bottom-r.top,
		SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOSIZE);

}

void THWindow::SetRedraw(BOOL redraw)
{
	::SendMessage(hSelf, WM_SETREDRAW, redraw, 0);
}
void THWindow::PerformCreateWindowEx(DWORD dwExStyle, 
					LPCTSTR lpWindowName, 
					DWORD dwStyle, 
					INT x, INT y, 
					INT nWidth, INT nHeight, 
					HWND hParent, 
					HMENU hMenu, 
					HINSTANCE hInstance)
{
	TCHAR ClassName[256];
	GetWindowClassName(ClassName);

	//创建窗口，在接收到Init的消息后，会记录窗口句柄给hSelf
	HWND hCreate = ::CreateWindowEx(dwExStyle, ClassName, lpWindowName,
				dwStyle,
				x, y, nWidth, nHeight,
				hParent, hMenu, hInstance, this);
	THCheck(hCreate);
	THCheck(hCreate == hSelf);
}
INT_PTR CALLBACK THWindow::StaticDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_INITDIALOG){
		THWindow *wnd = (THWindow *)lParam;
		THCheck(wnd);
		THCheck(!wnd->hSelf);
		wnd->hSelf = hWnd;
	}
	THWindow *wnd = reinterpret_cast<THWindow *>(::GetWindowLong(hWnd, GWL_USERDATA));
	if(wnd)
		return wnd->CallDefaultProc(msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);

}
LONG APIENTRY THWindow::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static UINT message = RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));
	if(msg == message){
		SetWindowLong(hWnd, DWL_MSGRESULT, TRUE);
		return 1;
	}
	if(msg == WM_NCCREATE || msg == WM_INITDIALOG){
		//先发送WM_NCCREATE，此时刚刚创建好窗口
		//在请求创建窗口的时候，传入了参数(this)，所以可以通过强转lParam获得
		THWindow *wnd = msg != WM_NCCREATE 
			? (THWindow *)lParam 
			: (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
			? (THWindow *)((MDICREATESTRUCT *)((CREATESTRUCT *)lParam)->lpCreateParams)->lParam
			: (THWindow *)((CREATESTRUCT *)lParam)->lpCreateParams;
		THCheck(wnd);
		THCheck(!wnd->hSelf);
		//wnd就是指向自己的一个指针
		wnd->hSelf = hWnd;	
	}
	//如果创建完窗口，那么发送的lParam就不一定是this了，所以这个时候通过hwnd获得对应的窗口
	THWindow *wnd = reinterpret_cast<THWindow *>(::GetWindowLong(hWnd, GWL_USERDATA));
	if(wnd)
		return wnd->CallDefaultProc(msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}
WNDPROC THWindow::RegisteWindowClass(const TCHAR *ClassName, DWORD style)
{
	WNDCLASSEX cls;
	memset(&cls, 0, sizeof(cls));
	cls.cbSize = sizeof(cls);
	cls.style = style;
	cls.lpfnWndProc = StaticWndProc;
	cls.hInstance = g_hInstanceWindow;
	cls.hIcon = NULL; //::LoadIcon(g_hInstanceWindow, g_GameIcon);
	cls.lpszClassName = ClassName;
	cls.hIconSm = NULL; //::LoadIcon(g_hInstanceWindow, g_GameIcon);
	THCheck(RegisterClassEx(cls));

	return NULL;
}


LONG THWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	try{
		if(msg == WM_DESTROY){
			OnDestroy();
		}else if(msg = WM_DRAWITEM){
			//画子窗口，有的控件中可以放多个子控件，有的不可以
			//所以这个可以拿到能存放多个控件的控件中写
		}else if(msg == WM_MEASUREITEM){
			//丈量子窗口的大小
		}else if(msg == WM_CLOSE){
			if(!OnClose())
				return 0;
		}else if(msg == WM_CHAR){
			OnChar(static_cast<TCHAR>(wParam));
		}else if(msg == WM_KEYDOWN){
			OnKeyDown(static_cast<TCHAR>(wParam));
		}else if(msg == WM_PAINT){
			OnPaint();
		}else if(msg == WM_CREATE){
			OnCreate();
		}else if(msg == WM_TIMER){
			OnTimer();
		}else if(msg == WM_INITDIALOG){
			OnInitDialog();
		}else if(msg == WM_ENTERIDLE){
			OnEnterIdle();
		}else if(msg == WM_SETFOCUS){
			OnSetFocus((HWND)wParam);
		}else if(msg == WM_ACTIVATE){
			OnActivate(LOWORD(wParam) != 0);
		}else if(msg == WM_KILLFOCUS){
			OnLoseFocus(HWND(wParam));	
		}else if(msg == WM_SIZE){
			OnSize(static_cast<DWORD>(wParam), LOWORD(lParam), HIWORD(lParam));
		}else if(msg == WM_WINDOWPOSCHANGING){
			WINDOWPOS *wpos = (LPWINDOWPOS)lParam;
			OnWindowPosChanging(&wpos->x, &wpos->y, &wpos->cx, &wpos->cy);
		}else if(msg == WM_MOVE){
			OnMove(LOWORD(lParam), HIWORD(lParam));
		}else if(msg == WM_PASTE){
			OnPaste();
		}else if(msg == WM_SHOWWINDOW){
			OnShowWindow(static_cast<BOOL>(wParam));
		}else if(msg == WM_COPYDATA){
			OnCopyData(HWND(wParam), (COPYDATASTRUCT *)lParam);
		}else if(msg == WM_CAPTURECHANGED){
			OnReleaseCapture();
		}else if(msg == WM_MDIACTIVATE){
			;	
		}else if(msg == WM_MOUSEMOVE){
			POINT p;
			p.x = LOWORD(lParam);
			p.y = HIWORD(lParam);
			OnMouseMove(static_cast<DWORD>(wParam), p);
		}else if(msg == WM_LBUTTONDOWN){
			OnLeftButtonDown();
		}else if(msg == WM_LBUTTONDBLCLK){
			OnLeftButtonDoubleClick();
		}else if(msg == WM_MBUTTONDBLCLK){
			OnMiddleButtonDoubleClick();
		}else if(msg == WM_RBUTTONDBLCLK){
			OnRightButtonDoubleClick();
		}else if(msg == WM_RBUTTONDOWN){
			OnRightButtonDown();
		}else if(msg == WM_LBUTTONUP){
			OnLeftButtonUp();
		}else if(msg == WM_RBUTTONUP){
			OnRightButtonUp();
		}else if(msg == WM_CUT){
			OnCut();
		}else if(msg == WM_COPY){
			OnCopy();
		}else if(msg == WM_UNDO){
			OnUndo();
		}else if(msg == WM_ERASEBKGND){
			
		}else if(msg == WM_SETCURSOR){
			if(OnSetCursor())
				return 1;
		}else if(msg == WM_NOTIFY){
		
		}else if(msg == WM_VSCROLL){
			OnVScroll(wParam, lParam);	
		}else if(msg == WM_KEYUP){
			OnKeyUp(wParam, lParam);
		}else if(msg == WM_COMMAND || msg == WM_HSCROLL){
			if(msg == WM_HSCROLL)
				OnHScroll(wParam, lParam);
			//通知子控件处理事件
			OnCommand(static_cast<INT>(wParam));
		}else if(msg == WM_SYSCOMMAND){
			if(OnSysCommand(static_cast<INT>(wParam)))
				return 1;
		}
		return CallDefaultProc(msg, wParam, lParam);
	}catch(const TCHAR *){
		return 0;
	}
}
INT THWindow::CallDefaultProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//如果是MDI，执行DefMDIChildProcX
	return DefWindowProc(hSelf, msg, wParam, lParam);
}

/************************************************************************/
/* Dialog                                                               */
/************************************************************************/
INT THDialog::DoModal(HINSTANCE hinst)
{
	//执行这个的时候开始初始化对话框
	THCheck(hSelf == NULL);
	INT result = ::DialogBoxParam(hinst, 
						MAKEINTRESOURCE(nControlID), 
						wndParent?wndParent->hSelf:NULL,
						(DLGPROC)StaticWndProc, (LPARAM)this);
	return result;
}
void THDialog::OpenChildWindow(INT controlID, BOOL visible)
{
	
}

//本地化对话框按钮中的文字
BOOL CALLBACK THDialog::LocalizeTextEnumProc(HWND hwnd, LPARAM lparam)
{
	//TCHAR **temp = (TCHAR **)lParam;
	TCHAR ch[1024];
	ch[0] = 0;
#define ARRAY_COUNT( array ) \
	( sizeof(array) / sizeof((array)[0]) )
	::SendMessage(hwnd, WM_GETTEXT, ARRAY_COUNT(ch), (LPARAM)ch);

	//if( FString(String).Left(4)==TEXT("IDC_") )
	//	SendMessageLX( hInWmd, WM_SETTEXT, 0, *LineFormat(*Localize(Temp[0],*String,Temp[1])) );
	//else if( String==TEXT("IDOK") )
	//	SendMessageLX( hInWmd, WM_SETTEXT, 0, *LineFormat(*LocalizeGeneral("OkButton",TEXT("Window"))) );
	//else if( String==TEXT("IDCANCEL") )
	//	SendMessageLX( hInWmd, WM_SETTEXT, 0, *LineFormat(*LocalizeGeneral("CancelButton",TEXT("Window"))) );
	//SendMessageX( hInWmd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(1,0) );

	return 1;
}
void THDialog::LocalizedText(const TCHAR *section)
{
	const TCHAR *temp[2];
	temp[0] = section;
	temp[1] = (TCHAR *)this;
	
	//本地化子控件的文字
	::EnumChildWindows(hSelf, LocalizeTextEnumProc, LPARAM(this));
	//本地化自己的文字
	LocalizeTextEnumProc(hSelf, LPARAM(temp));
}
void THDialog::OnInitDialog()
{
	THWindow::OnInitDialog();
	::SendMessage(hSelf, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(1, 0));
	TCHAR temp[256];
	temp[0] = 0;
	//获得该控件的名称。
	LocalizedText(temp);
}
void THDialog::EndDialog(INT result)
{
	::EndDialog(hSelf, result);
}
void THDialog::EndDialogFalse()
{
	EndDialog(0);
}
void THDialog::EndDialogTrue()
{
	EndDialog(1);
}
void THDialog::Show(BOOL show)
{
	THWindow::Show(show);
	if(show)
		::BringWindowToTop(hSelf);
}
void THDialog::CenterInOwnerWindow()
{
	if(wndParent){
		RECT r = GetWindowRect(0);
		RECT rp = wndParent->GetWindowRect(0);
		INT x = (rp.right-rp.left)/2-(r.right-r.left)/2;
		INT y = (rp.bottom-rp.bottom)/2-(r.bottom-r.top)/2;
		::SetWindowPos(hSelf, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}
}






