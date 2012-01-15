////////////////////////////////////////////////////////////////////////
// WavTipCtrl.cpp : implementation file
//
// Tooltip control.with popup sound
//
// Copyright (C) 1997, 1998 Giancarlo Iovino (giancarlo@saria.com)
// All rights reserved. May not be sold for profit.
//
// Thanks to Anthony Petruso for the PlayResource function.
//

#include "stdafx.h"
#include "WavTipSample.h"
#include "WavTipCtrl.h"

#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWavTipCtrl

CWavTipCtrl::CWavTipCtrl()
{
	m_wWaveResourceID = 0;
}

CWavTipCtrl::~CWavTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CWavTipCtrl, CToolTipCtrl)
	//{{AFX_MSG_MAP(CWavTipCtrl)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWavTipCtrl message handlers

void CWavTipCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CToolTipCtrl::OnWindowPosChanged(lpwndpos);
	
	//if (lpwndpos->flags & SWP_SHOWWINDOW)
	//	if (m_wWaveResourceID)
		//	PlayResource(m_wWaveResourceID);
}

// Sets the popup WAVE
void CWavTipCtrl::SetPopupSound(WORD wResourceID) {
	m_wWaveResourceID = wResourceID;
}

// Gets the popup WAVE resource id
WORD CWavTipCtrl::GetPopupSound() const {
	return m_wWaveResourceID;
}


/*// Plays a WAVE resource
BOOL CWavTipCtrl::PlayResource(WORD wResourceID)
{	
	// Get the handle to the current instance of the application
	HINSTANCE hInstance = AfxGetInstanceHandle();
    ASSERT (hInstance != NULL);

	// Find the WAVE resource
	HRSRC hResInfo = 
	  FindResource(hInstance, MAKEINTRESOURCE(wResourceID), _T("WAVE"));
	if(hResInfo == NULL)
		return FALSE;
	
	// Load the WAVE resource
    HANDLE hRes = LoadResource(hInstance, hResInfo);
	if (hRes == NULL)
		return FALSE;
    
	// Lock the WAVE resource and play it
    LPSTR lpRes = (LPSTR) LockResource(hRes);
	if(lpRes==NULL)
		return FALSE;
    
	if (sndPlaySound(lpRes, SND_MEMORY | SND_ASYNC) == NULL)
		return FALSE;	

    // Free the WAVE resource and return success or failure.
    FreeResource(hRes);
	
	return TRUE;
}
*/