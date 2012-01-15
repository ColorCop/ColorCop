// ColorCop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include <windows.h>
//#include "AggressiveOptimize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHtmlcopApp

BEGIN_MESSAGE_MAP(CHtmlcopApp, CWinApp)
	//{{AFX_MSG_MAP(CHtmlcopApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlcopApp construction


CHtmlcopApp::CHtmlcopApp()
{
	ATLTRACE2(atlTraceGeneral, 0, "*** Color Cop Constructor\n");

	m_hMutex=NULL;
}

CHtmlcopApp::~CHtmlcopApp()
{
	ATLTRACE2(atlTraceGeneral, 0, "*** Color Cop Destructor\n");
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHtmlcopApp object

CHtmlcopApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHtmlcopApp initialization

BOOL CHtmlcopApp::InitInstance()
{

	ATLTRACE2(atlTraceGeneral, 0, "Begin InitInstance\n");

	// enable tracing if 
	#ifdef _DEBUG
		::afxTraceEnabled = true;
	#endif


	// Depreciated Functions in MFC 5.0
	#if _MSC_VER < 1300
		#ifdef _AFXDLL
			Enable3dControls();			// shared 
		#else
			Enable3dControlsStatic();	// static
		#endif
	#endif

	// see srand
	srand( (unsigned)time(NULL) );

	ATLTRACE2(atlTraceGeneral, 0, "Color Cop CreateMutex\n");
	
	//CreateMutex(NULL,TRUE,_T("ColorCopMutex-Jay-Prall")); // mutex will be automatically deleted when process ends. 

//	BOOL bAlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS); 
	

	// multiple instances are not allowed?
	if (!(dlg.m_Appflags & MultipleInstances))
	{
		// multiple instances are not allowed. check if we have one running
			if(InstanceRunning())
			{
				// release the obj that we tried to create
				ReleaseMutex(m_hMutex);

				// TODO: focus on the current instance

				// error instead
				AfxMessageBox("Color Cop is already running.");

				return false;
			}

		// lets attempt to get the current instance of color cop and use that.

	}






	// if already running and only one instance allowed, errror

//	if ((bAlreadyRunning) && (!(dlg.m_Appflags & MultipleInstances))) {

//		ATLTRACE2(atlTraceGeneral, 0, "Color Cop Already Running\n");
//		return FALSE; // abandon ship
	
//	} else {	

		ATLTRACE2(atlTraceGeneral, 0, "Color Cop Starting\n");
		m_pMainWnd = &dlg;		// set the main window
		
		int nResponse = dlg.DoModal();		// Launch the color cop dialog
	
		if ((nResponse == IDOK)||(nResponse == IDCANCEL)) {
		
			ATLTRACE2(atlTraceGeneral, 0, "Color Cop Closing\n");
			CloseApplication();		// write the data to a file
		}

//	}
		ATLTRACE2(atlTraceGeneral, 0, "End InitInstance\n");

	return FALSE;
}

//http://www.koders.com/cpp/fidDA7CC61A9668A4356119E1B5E6F54836B44200D8.aspx?s=InstanceRunning
// uses a Mutex to figure out if there is an instance of color cop running
bool CHtmlcopApp::InstanceRunning()
{
	m_hMutex = CreateMutex(NULL, false, "ColorCop_Mutex");
	
	if(m_hMutex)
	{
		DWORD dwWait = WaitForSingleObject(m_hMutex, 10);

		switch(dwWait)
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			return true;
		case WAIT_FAILED:
			break;
		}
	}

	return false;
}

CString CHtmlcopApp::GetTempFolder() 
{
	CString strTmpPath;
	DWORD dwL;

	dwL = GetTempPath(MAX_PATH, strTmpPath.GetBuffer(MAX_PATH));
	strTmpPath.ReleaseBuffer(dwL);
	
	return strTmpPath;
}

BOOL CHtmlcopApp::InitApplication() 
{
	////////////////////////////////////////////////////////////
	// This function reads the settings from a file.  It should 
	// do this every time no matter what.
	//
	ATLTRACE2(atlTraceGeneral, 0, "Begin InitApplication\n");

	CString strInitFile = GetTempFolder();
	strInitFile.Append(INI_FILE);
	
	CFile file;
	if(file.Open(strInitFile, CFile::modeRead)) {
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
	} else {
		LoadDefaultSettings();
	}

	ATLTRACE2(atlTraceGeneral, 0, "End InitApplication\n");

	return CWinApp::InitApplication();
}

void CHtmlcopApp::LoadDefaultSettings() {

		// This function is called when we can't find a .DAT file
		// with the persistent variables, or it was an old dat file

		// OK - better set the default settings and custom colors

		dlg.m_Reddec   = 171;	// load navy blue websafe, a cool color to start with
		dlg.m_Greendec = 208;
		dlg.m_Bluedec  = 188;
		dlg.m_Appflags = 0;		// reset
		dlg.m_Appflags |= AlwaysOnTop;
		dlg.m_Appflags |= UpperCaseHex;
		dlg.m_Appflags |= AutoCopytoClip;
		dlg.m_Appflags |= ModeHTML;
		dlg.m_Appflags |= EasyMove;
		dlg.m_Appflags |= Sampling1;
		dlg.m_Appflags |= ExpandedDialog;
		dlg.m_Appflags |= MultipleInstances;
		dlg.m_Appflags |= MAGWHILEEYEDROP;
		

		// set all custom color blocks to white
		for(int initcolor = 0; initcolor < 16; initcolor++)
		{
			dlg.CustColorBank[initcolor] = (COLORREF) 0x00FFFFFF;  
		}
		            //C++ explicit hex 0x00bbggrr 

		dlg.ColorHistory[0] = (COLORREF) 0x00223300;	// setup default colors for the
		dlg.ColorHistory[1] = (COLORREF) 0x000000FF;	// color history bar
		dlg.ColorHistory[2] = (COLORREF) 0x0000FF99;
		dlg.ColorHistory[3] = (COLORREF) 0x009999FF;	
		dlg.ColorHistory[4] = (COLORREF) 0x00FF9900;
		dlg.ColorHistory[5] = (COLORREF) 0x00FFFF99;
		dlg.ColorHistory[6] = (COLORREF) 0x00999900;

	return;
}

void CHtmlcopApp::CloseApplication() {

	////////////////////////////////////////////////////////////
	// This function writes the settings to a file.  It is the 
	// last thing the application will do. It will only write to
	// a file when the dialog has been closed IDOK or IDCANCEL

	ATLTRACE2(atlTraceGeneral, 0, "Begin CloseApplication\n");


	CString strInitFile = GetTempFolder();
	strInitFile.Append(INI_FILE);


	CFile file;
	if(file.Open(strInitFile,CFile::modeWrite|CFile::modeCreate))
	{
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
	}

	// might as well release this now
	ATLTRACE2(atlTraceGeneral, 0, "- Release Mutex\n");
	ReleaseMutex(m_hMutex);


	ATLTRACE2(atlTraceGeneral, 0, "End CloseApplication\n");


	return;
}

void CHtmlcopApp::Serialize(CArchive& ar) 
{
	ATLTRACE2(atlTraceGeneral, 0, "Serialize\n");

	// anything that is serialized here should also


	if (ar.IsStoring())	{	
		// storing code
		try {
			ar << dlg.m_Reddec;
			ar << dlg.m_Greendec;
			ar << dlg.m_Bluedec;
			ar << dlg.m_Appflags;
			ar << dlg.WinLocX;
			ar << dlg.WinLocY;
			for (int j = 0; j<16; j++)		//load custom color values to array
				ar << dlg.CustColorBank[j];
			for(int w = 0; w<7; w++)		// save color history values
				ar << dlg.ColorHistory[w];
		} catch (CArchiveException*) {
			AfxMessageBox(_T("Error writing data.."));
		}
	} else {
		// loading code
		try {
			ar >> dlg.m_Reddec;
			ar >> dlg.m_Greendec;
			ar >> dlg.m_Bluedec;
			ar >> dlg.m_Appflags;
			ar >> dlg.WinLocX;
			ar >> dlg.WinLocY;
			for (int j = 0; j<16; j++)		//load custom color values to array
				ar >> dlg.CustColorBank[j];
			for(int w = 0; w<7; w++)		// save color history values
				ar >> dlg.ColorHistory[w];
	
		} catch (CArchiveException*) {
			AfxMessageBox(_T("Please delete ColorCop.dat"));
		}
	}
}
