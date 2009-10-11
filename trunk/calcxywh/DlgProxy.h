// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__A2B389AA_5994_43A5_B31B_0CE70DA85C97__INCLUDED_)
#define AFX_DLGPROXY_H__A2B389AA_5994_43A5_B31B_0CE70DA85C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalcxywhDlg;

/////////////////////////////////////////////////////////////////////////////
// CCalcxywhDlgAutoProxy command target

class CCalcxywhDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CCalcxywhDlgAutoProxy)

	CCalcxywhDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CCalcxywhDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcxywhDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCalcxywhDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CCalcxywhDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CCalcxywhDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCalcxywhDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__A2B389AA_5994_43A5_B31B_0CE70DA85C97__INCLUDED_)
