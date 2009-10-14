// calcxywhDlg.h : header file
//

#if !defined(AFX_CALCXYWHDLG_H__4E07C2FB_83FB_448D_B525_A790D5C68069__INCLUDED_)
#define AFX_CALCXYWHDLG_H__4E07C2FB_83FB_448D_B525_A790D5C68069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CCalcxywhDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CCalcxywhDlg dialog

class CCalcxywhDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalcxywhDlg);
	friend class CCalcxywhDlgAutoProxy;
public:
	void calc(const int idcEdit, const int idcLabel, const float radix);
	void calc(const int idcEdit, const int idcLabel, const int xDepend, const int xlDepend);
	// Construction
public:
	CCalcxywhDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CCalcxywhDlg();
// Dialog Data
	//{{AFX_DATA(CCalcxywhDlg)
	enum { IDD = IDD_CALCXYWH_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcxywhDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCalcxywhDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	
	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CCalcxywhDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillfocusEDITLeft();
	afx_msg void OnKillfocusEDITTop();
	afx_msg void OnKillfocusEDITWidth();
	afx_msg void OnKillfocusEDITHeight();
	afx_msg void OnBUTTONClear();
	afx_msg void OnCHECKDepend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCXYWHDLG_H__4E07C2FB_83FB_448D_B525_A790D5C68069__INCLUDED_)
