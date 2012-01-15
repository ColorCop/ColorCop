////////////////////////////////////////////////////////////////////////
// WavTipCtrl.h : header file
//
// Tooltip control.with popup sound
//
// Copyright (C) 1997, 1998 Giancarlo Iovino (giancarlo@saria.com)
// All rights reserved. May not be sold for profit.
//
// Thanks to Anthony Petruso for the PlayResource function.
//

/////////////////////////////////////////////////////////////////////////////
// CWavTipCtrl window

#if !defined(AFX_WAVTIPCTRL_H_02345FK301_02D3F0_022GE54938_ENG_INCLUDED_)
#define AFX_WAVTIPCTRL_H_02345FK301_02D3F0_022GE54938_ENG_INCLUDED_

class CWavTipCtrl : public CToolTipCtrl
{
// Construction
public:
	CWavTipCtrl();

// Attributes
public:

// Operations
public:
	void SetPopupSound(WORD wResourceID);
	WORD GetPopupSound() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWavTipCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWavTipCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWavTipCtrl)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//static BOOL PlayResource(WORD wResourceID);

	WORD m_wWaveResourceID;
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_WAVTIPCTRL_H_02345FK301_02D3F0_022GE54938_ENG_INCLUDED_CTRL_H_02345FK301_02D3F0_022GE54938_ENG_INCLUDED_)
