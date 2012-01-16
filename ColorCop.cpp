// ColorCop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include <windows.h>
#include "AggressiveOptimize.h"

#define INI_FILE "\\Color_Cop_5.4.dat"
#define INI_FILE_DIR "\\ColorCop"

#define MONITOR_CENTER     0x0001        // center rect to monitor
#define MONITOR_CLIP     0x0000        // clip rect to monitor
#define MONITOR_WORKAREA 0x0002        // use monitor work area
#define MONITOR_AREA     0x0000        // use monitor entire area


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColorCopApp

BEGIN_MESSAGE_MAP(CColorCopApp, CWinApp)
	//{{AFX_MSG_MAP(CColorCopApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp construction


CColorCopApp::CColorCopApp()
{
///	//ATLTRACE2(atlTraceGeneral, 0, "*** Color Cop Constructor\n");

	m_hMutex=NULL;
}

CColorCopApp::~CColorCopApp()
{
	//ATLTRACE2(atlTraceGeneral, 0, "*** Color Cop Destructor\n");
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CColorCopApp object

CColorCopApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CColorCopApp initialization

BOOL CColorCopApp::InitInstance()
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

	//ATLTRACE2(atlTraceGeneral, 0, "Color Cop CreateMutex\n");
	
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
				AfxMessageBox(IDS_APP_RUNNING);

				// error instead

				return false;
			}

		// lets attempt to get the current instance of color cop and use that.

	}





	// if already running and only one instance allowed, error

//	if ((bAlreadyRunning) && (!(dlg.m_Appflags & MultipleInstances))) {

//		//ATLTRACE2(atlTraceGeneral, 0, "Color Cop Already Running\n");
//		return FALSE; // abandon ship
	
//	} else {	

		//ATLTRACE2(atlTraceGeneral, 0, "Color Cop Starting\n");
		m_pMainWnd = &dlg;		// set the main window
		
		int nResponse = dlg.DoModal();		// Launch the color cop dialog
	
		if ((nResponse == IDOK)||(nResponse == IDCANCEL)) {
		
			//ATLTRACE2(atlTraceGeneral, 0, "Color Cop Closing\n");
			CloseApplication();		// write the data to a file
		}

//	}
		//ATLTRACE2(atlTraceGeneral, 0, "End InitInstance\n");

	return FALSE;
}

//http://www.koders.com/cpp/fidDA7CC61A9668A4356119E1B5E6F54836B44200D8.aspx?s=InstanceRunning
// uses a Mutex to figure out if there is an instance of color cop running
bool CColorCopApp::InstanceRunning()
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

BOOL CColorCopApp::GetShellFolderPath(char* pShellFolder, char* pShellPath)
{
	// pShellFolder can be one of the following
	// AppData, Cache, Cookies, Desktop, Favorites, Fonts, History, NetHood,
	// Personal, Printhood, Programs, Recent, SendTo, Start Menu, Startup,
	// Templates, ShellNew
	DWORD rc;
	DWORD length = MAX_PATH;
	DWORD type = REG_SZ;
	HKEY hkey;

	rc = RegOpenKeyEx(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
        0, KEY_READ, &hkey);

    if (rc == ERROR_SUCCESS){
        rc = RegQueryValueEx(hkey, pShellFolder, NULL, &type,
                (BYTE *) pShellPath, &length);
    RegCloseKey(hkey);
    }

    if (rc == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

CString CColorCopApp::GetTempFolder()
{
	CString strTmpPath;
//	DWORD dwL;

	//dwL = GetTempPath(MAX_PATH, strTmpPath.GetBuffer(MAX_PATH));
	
	GetShellFolderPath("AppData", strTmpPath.GetBuffer(MAX_PATH));



/*
	DWORD dwL;

	// temp path isn't what we want
	//dwL = GetTempPath(MAX_PATH, strTmpPath.GetBuffer(MAX_PATH));

	//char ShellPath[MAX_PATH];

	GetShellFolderPath("AppData", strTmpPath.GetBuffer(MAX_PATH));


  char appDataPath [MAX_PATH + 1];
  csString path;
  
  // Try to retrieve "Application Data" directory
  if (!GetShellFolderPath (CSIDL_APPDATA, appDataPath))
  {
    // Fall back to My Documents
    if (!GetShellFolderPath (CSIDL_PERSONAL, appDataPath))
    {
      // Guess...
      strcpy (appDataPath, ".");
    }
  }*/

	strTmpPath.ReleaseBuffer();

	
	return strTmpPath;
}

void CColorCopApp::ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags)
{
	RECT rc;
	GetWindowRect(hwnd, &rc);
	//ClipOrCenterRectToMonitor(&rc, flags);
	SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	dlg.WinLocX= rc.left;
	dlg.WinLocY= rc.top;
}


BOOL CColorCopApp::InitApplication() 
{
	////////////////////////////////////////////////////////////
	// This function reads the settings from a file.  It should 
	// do this every time no matter what.
	//
	//ATLTRACE2(atlTraceGeneral, 0, "Begin InitApplication\n");

	CString strInitFile = GetTempFolder();

	strInitFile += INI_FILE_DIR;

	strInitFile += INI_FILE;

	
	CFile file;
	if(file.Open(strInitFile, CFile::modeRead)) {
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
	} else {

		// First time we are running color Cop
		CString strAppDirectory = GetTempFolder();
		strAppDirectory += INI_FILE_DIR;
		CreateDirectory(strAppDirectory.GetBuffer(MAX_PATH), NULL);
		strAppDirectory.ReleaseBuffer();

		LoadDefaultSettings();

		

		ClipOrCenterWindowToMonitor(::GetForegroundWindow(), MONITOR_CENTER);;

		// set the window to be in the middle
	}

	//ATLTRACE2(atlTraceGeneral, 0, "End InitApplication\n");

	return CWinApp::InitApplication();
}


/*

//
//  ClipOrCenterRectToMonitor
//
//  The most common problem apps have when running on a
//  multimonitor system is that they "clip" or "pin" windows
//  based on the SM_CXSCREEN and SM_CYSCREEN system metrics.
//  Because of app compatibility reasons these system metrics
//  return the size of the primary monitor.
//
//  This shows how you use the multi-monitor functions
//  to do the same thing.
//
void CColorCopApp::ClipOrCenterRectToMonitor(LPRECT prc, UINT flags) {
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
    int         w = prc->right  - prc->left;
    int         h = prc->bottom - prc->top;
    rc.left = prc->left;
    rc.right = prc->left+1;
    rc.top = prc->top;
    rc.bottom = prc->top+1;
    //
    // get the nearest monitor to the passed rect.
    //
    hMonitor = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);

    //
    // get the work area or entire monitor rect.
    //
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (flags & MONITOR_WORKAREA)
	   rc = mi.rcWork;
    else
	   rc = mi.rcMonitor;

    //
    // center or clip the passed rect to the monitor rect
    //
    if (flags & MONITOR_CENTER)
    {
	   prc->left   = rc.left + (rc.right  - rc.left - w) / 2;
	   prc->top    = rc.top  + (rc.bottom - rc.top  - h) / 2;
	   prc->right  = prc->left + w;
	   prc->bottom = prc->top  + h;
    }
    else
    {
	   prc->left   = max(rc.left, min(rc.right-w,  prc->left));
	   prc->top    = max(rc.top,  min(rc.bottom-h, prc->top));
	   prc->right  = prc->left + w;
	   prc->bottom = prc->top  + h;
    }


}
*/



void CColorCopApp::LoadDefaultSettings() {

		// This function is called when we can't find a .DAT file
		// with the persistent variables, or it was an old dat file

		// OK - better set the default settings and custom colors

		dlg.m_Reddec   = 171;	// load navy blue websafe, a cool color to start with
		dlg.m_Greendec = 208;
		dlg.m_Bluedec  = 188;
		dlg.m_Appflags = 0;		// reset
		dlg.m_Appflags |= AlwaysOnTop;
		
		// uppercase hex is now not a default options
		//dlg.m_Appflags |= UpperCaseHex;
		dlg.m_Appflags |= AutoCopytoClip;
		dlg.m_Appflags |= ModeHTML;
		dlg.m_Appflags |= EasyMove;
		dlg.m_Appflags |= Sampling1;
		dlg.m_Appflags |= ExpandedDialog;
		dlg.m_Appflags |= MultipleInstances;
		dlg.m_Appflags |= MAGWHILEEYEDROP;
		dlg.m_Appflags |= SpaceRGB;
		
		dlg.WinLocX=200;
		dlg.WinLocY=200;

		dlg.m_iSamplingOffset=3;


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

void CColorCopApp::CloseApplication() {

	////////////////////////////////////////////////////////////
	// This function writes the settings to a file.  It is the 
	// last thing the application will do. It will only write to
	// a file when the dialog has been closed IDOK or IDCANCEL

	//ATLTRACE2(atlTraceGeneral, 0, "Begin CloseApplication\n");


	CString strInitFile = GetTempFolder();

	strInitFile += INI_FILE_DIR;

	strInitFile += INI_FILE;

	CFile file;
	if(file.Open(strInitFile,CFile::modeWrite|CFile::modeCreate))
	{
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
	}

	// might as well release this now
	//ATLTRACE2(atlTraceGeneral, 0, "- Release Mutex\n");
	ReleaseMutex(m_hMutex);


	//ATLTRACE2(atlTraceGeneral, 0, "End CloseApplication\n");


	return;
}

void CColorCopApp::Serialize(CArchive& ar) 
{
	//ATLTRACE2(atlTraceGeneral, 0, "Serialize\n");

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

			ar <<  dlg.m_iSamplingOffset;

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
	
			ar >>  dlg.m_iSamplingOffset;

		} catch (CArchiveException*) {

			AfxMessageBox(_T("Please delete ColorCop.dat"));
		}
	}
}
