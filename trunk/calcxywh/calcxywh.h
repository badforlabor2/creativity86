// calcxywh.h : main header file for the CALCXYWH application
//

#if !defined(AFX_CALCXYWH_H__3148BDE7_8619_499D_9B30_A87ABF648A5E__INCLUDED_)
#define AFX_CALCXYWH_H__3148BDE7_8619_499D_9B30_A87ABF648A5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCalcxywhApp:
// See calcxywh.cpp for the implementation of this class
//

class CCalcxywhApp : public CWinApp
{
public:
	CCalcxywhApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcxywhApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCalcxywhApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCXYWH_H__3148BDE7_8619_499D_9B30_A87ABF648A5E__INCLUDED_)
