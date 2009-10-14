// calcxywhDlg.cpp : implementation file
//

#include "stdafx.h"
#include "calcxywh.h"
#include "calcxywhDlg.h"
#include "DlgProxy.h"
#include <tchar.h>
#include <string>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MAX_LEN 256
/////////////////////////////////////////////////////////////////////////////
// CCalcxywhDlg dialog

IMPLEMENT_DYNAMIC(CCalcxywhDlg, CDialog);

CCalcxywhDlg::CCalcxywhDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcxywhDlg::IDD, pParent) 
{
	//{{AFX_DATA_INIT(CCalcxywhDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	
	//初始化恍┎问
	
	
}

CCalcxywhDlg::~CCalcxywhDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CCalcxywhDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcxywhDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCalcxywhDlg, CDialog)
	//{{AFX_MSG_MAP(CCalcxywhDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_Left, OnKillfocusEDITLeft)
	ON_EN_KILLFOCUS(IDC_EDIT_Top, OnKillfocusEDITTop)
	ON_EN_KILLFOCUS(IDC_EDIT_Width, OnKillfocusEDITWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_Height, OnKillfocusEDITHeight)
	ON_BN_CLICKED(IDC_BUTTON_Clear, OnBUTTONClear)
	ON_BN_CLICKED(IDC_CHECK_Depend, OnCHECKDepend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcxywhDlg message handlers

BOOL CCalcxywhDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalcxywhDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalcxywhDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CCalcxywhDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnCancel();
}

void CCalcxywhDlg::OnOK() 
{
// 	if (CanExit())
// 		CDialog::OnOK();
}

void CCalcxywhDlg::OnCancel() 
{
//	if (CanExit())
//		CDialog::OnCancel();
}

BOOL CCalcxywhDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void calcEX(HWND hwnd, int idcEdit, int idcLabel, float radix)
{
	TCHAR content [MAX_LEN], *stopContent;
	
	int len;
	double result;
	len = GetDlgItemText(hwnd, idcEdit, content, MAX_LEN);
	content[len] = CHAR('\0');
	result = _tcstod(content, &stopContent);
	result = result / radix;
	_stprintf(content, "%f", result);
	SetDlgItemText(hwnd, idcLabel, content);	
}
///数值转换
void CCalcxywhDlg::calc(int idcEdit, int idcLabel, float radix)
{
	TCHAR content [MAX_LEN], *stopContent;
	
	int len;
	double result;
	len = GetDlgItemText(idcEdit, content, MAX_LEN);
	content[len] = CHAR('\0');
	result = _tcstod(content, &stopContent);
	result = result / radix;
	_stprintf(content, "%f", result);
	this->SetDlgItemText(idcLabel, content);
}
///相对坐标的
void CCalcxywhDlg::calc(const int idcEdit, const int idcLabel, const int xDepend, const int xlDepend)
{
	TCHAR content [MAX_LEN];
	int len;
	double result;
	float x1, xl1, x0, xl0;
	
	len = GetDlgItemText(idcEdit, content, MAX_LEN);
	result = _tcstod(content, NULL);
	x1 = float(result);
	
	len = GetDlgItemText(xDepend, content, MAX_LEN);
	result = _tcstod(content, NULL);
	x0 = float(result);

	len = GetDlgItemText(xlDepend, content, MAX_LEN);
	result = _tcstod(content, NULL);
	xl0 = float(result);

	if(xl0 == 0)
		xl1 = 0;
	else
		xl1 = (x1-x0)/xl0;

	_stprintf(content, "%f", xl1);
	this->SetDlgItemText(idcLabel, content);
}

void CCalcxywhDlg::OnKillfocusEDITLeft() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHECK_Depend)){
		calc(IDC_EDIT_Left, IDC_STATIC_Left, IDC_EDIT_XDepend, IDC_EDIT_WDepend);	
	}else{
		//其实，完全可以调用calc(IDC_EDIT_Left, IDC_STATIC_Left, IDC_EDIT_XDepend, IDC_EDIT_WDepend);
		calc(IDC_EDIT_Left, IDC_STATIC_Left, 1024);
	}	
}

void CCalcxywhDlg::OnKillfocusEDITTop() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHECK_Depend))
		calc(IDC_EDIT_Top, IDC_STATIC_Top, IDC_EDIT_YDepend, IDC_EDIT_HDepend);
	else
		calc(IDC_EDIT_Top, IDC_STATIC_Top, 768);	
}

void CCalcxywhDlg::OnKillfocusEDITWidth() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHECK_Depend)){
		calc(IDC_EDIT_Width, IDC_STATIC_Width, 0, IDC_EDIT_WDepend);	
	}else{
		calc(IDC_EDIT_Width, IDC_STATIC_Width, 1024);	
	}
}

void CCalcxywhDlg::OnKillfocusEDITHeight() 
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHECK_Depend))
		calc(IDC_EDIT_Height, IDC_STATIC_Height, 0, IDC_EDIT_HDepend);
	else
		calc(IDC_EDIT_Height, IDC_STATIC_Height, 768);
}

void CCalcxywhDlg::OnBUTTONClear() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_Left, TEXT(""));
	SetDlgItemText(IDC_EDIT_Top, TEXT(""));
	SetDlgItemText(IDC_EDIT_Width, TEXT(""));
	SetDlgItemText(IDC_EDIT_Height, TEXT(""));

	SetDlgItemText(IDC_STATIC_Left, TEXT(""));
	SetDlgItemText(IDC_STATIC_Top, TEXT(""));
	SetDlgItemText(IDC_STATIC_Width, TEXT(""));
	SetDlgItemText(IDC_STATIC_Height, TEXT(""));
}

void CCalcxywhDlg::OnCHECKDepend() 
{
	// TODO: Add your control notification handler code here
	OnKillfocusEDITLeft();
	OnKillfocusEDITTop();
	OnKillfocusEDITWidth();
	OnKillfocusEDITHeight();
}
