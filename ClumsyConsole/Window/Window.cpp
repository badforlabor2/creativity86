#include <windows.h>

#include <commctrl.h>
#include <richedit.h>

#include "THWin32.h"
#include "Window.h"


///ȫ�ֱ���
HINSTANCE g_hInstanceWindow;

///��ô��ڵ�Client����
RECT THWindow::GetClientRect() const
{
	RECT rect;
	::GetClientRect(hSelf, &rect);
	return rect;
}
///��ô�����ռ����
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


///������������Ļ���������ת��
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

///�������ڴ����е�����
POINT THWindow::GetCursorPos()
{
	POINT mouse;
	::GetCursorPos(&mouse);
	return ScreenToClient(mouse);
}

//SW_HIDE   ���ش��ڣ��״̬����һ������     
//SW_MINIMIZE   ��С�����ڣ��״̬����һ������     
//SW_RESTORE   ��ԭ���Ĵ�С��λ����ʾһ�����ڣ�ͬʱ�������״̬     
//SW_SHOW   �õ�ǰ�Ĵ�С��λ����ʾһ�����ڣ�ͬʱ�������״̬     
//SW_SHOWMAXIMIZED   ��󻯴��ڣ������伤��     
//SW_SHOWMINIMIZED   ��С�����ڣ������伤��     
//SW_SHOWMINNOACTIVE   ��С��һ�����ڣ�ͬʱ���ı�����     
//SW_SHOWNA   �õ�ǰ�Ĵ�С��λ����ʾһ�����ڣ����ı�����     
//SW_SHOWNOACTIVATE   ������Ĵ�С��λ����ʾһ�����ڣ�ͬʱ���ı�����     
//SW_SHOWNORMAL   ��SW_RESTORE��
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
	//�����MDI,��ôִ��SendMessage( OwnerWindow->hWnd, WM_MDIDESTROY, (WPARAM)hWnd, 0 )
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

///�������û����Ļ�ϣ���ôʹ�ô�����ʾ�����Ͻ�
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

	//�Ѵ���������ǰ��
	//�����������ʱ��SWP_NOSIZE��ʾ���ı��С��������cx��cy��ֵ
	//SWP_NOSENDCHANGING��ʾ������ChangePos����Ϣ�ˡ�
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

	//�������ڣ��ڽ��յ�Init����Ϣ�󣬻��¼���ھ����hSelf
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
		//�ȷ���WM_NCCREATE����ʱ�ոմ����ô���
		//�����󴴽����ڵ�ʱ�򣬴����˲���(this)�����Կ���ͨ��ǿתlParam���
		THWindow *wnd = msg != WM_NCCREATE 
			? (THWindow *)lParam 
			: (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
			? (THWindow *)((MDICREATESTRUCT *)((CREATESTRUCT *)lParam)->lpCreateParams)->lParam
			: (THWindow *)((CREATESTRUCT *)lParam)->lpCreateParams;
		THCheck(wnd);
		THCheck(!wnd->hSelf);
		//wnd����ָ���Լ���һ��ָ��
		wnd->hSelf = hWnd;	
	}
	//��������괰�ڣ���ô���͵�lParam�Ͳ�һ����this�ˣ��������ʱ��ͨ��hwnd��ö�Ӧ�Ĵ���
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
			//���Ӵ��ڣ��еĿؼ��п��ԷŶ���ӿؼ����еĲ�����
			//������������õ��ܴ�Ŷ���ؼ��Ŀؼ���д
		}else if(msg == WM_MEASUREITEM){
			//�����Ӵ��ڵĴ�С
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
			//֪ͨ�ӿؼ������¼�
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
	//�����MDI��ִ��DefMDIChildProcX
	return DefWindowProc(hSelf, msg, wParam, lParam);
}

/************************************************************************/
/* Dialog                                                               */
/************************************************************************/
INT THDialog::DoModal(HINSTANCE hinst)
{
	//ִ�������ʱ��ʼ��ʼ���Ի���
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

//���ػ��Ի���ť�е�����
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
	
	//���ػ��ӿؼ�������
	::EnumChildWindows(hSelf, LocalizeTextEnumProc, LPARAM(this));
	//���ػ��Լ�������
	LocalizeTextEnumProc(hSelf, LPARAM(temp));
}
void THDialog::OnInitDialog()
{
	THWindow::OnInitDialog();
	::SendMessage(hSelf, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(1, 0));
	TCHAR temp[256];
	temp[0] = 0;
	//��øÿؼ������ơ�
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






