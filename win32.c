Pasted by: AlphaZeta from 24.71.223.142
Language: C++
Description: ATL = BAD = NOT WORKING ATM
Add Line Numbers
Other recent pastes 
It crashes on execute. Need a flash file called FSCommand.swf.
---- Load.cpp ----
#include <objbase.h>
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "atlwin.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <initguid.h>
#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <process.h>


#include "swflash.tlh"
#include "resource.h"
#define MAX_LOADSTRING 100
#pragma comment(lib, "atl.lib")

// Globals
HINSTANCE      hInst;
CComModule     _Module;
TCHAR          szTitle[MAX_LOADSTRING];
TCHAR          szWindowClass[MAX_LOADSTRING];
HWND           hMain;
_ATL_FUNC_INFO FSCommandInfo = {CC_STDCALL, VT_ERROR, 2, {VT_BSTR,VT_BSTR}}; // ATL event information
_ATL_FUNC_INFO OnProgressInfo = {CC_STDCALL, VT_ERROR, 1, {VT_I4}};
_ATL_FUNC_INFO OnReadyStateChangeInfo = {CC_STDCALL, VT_ERROR, 1, {VT_I4}};
DEFINE_GUID(CLSID_Client, 
			0x30bf0abb, 0x1f5b, 0x424c, 0xa1, 0x8a, 0xd0, 0x44, 0x9c, 0x28, 0x5c, 0x5c);

// Sink
IShockwaveFlash *pIShockwave=NULL; // Connection to ActiveX control
IConnectionPoint *pCP;



// Global function prototypes
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// Initialize event sink class
class ATL_NO_VTABLE CFlashEvents : public CComObjectRootEx<CComSingleThreadModel>, 
								   public CComCoClass<CFlashEvents, &CLSID_Client>,
								   public IDispatchImpl<DShockwaveFlashEvents, &DIID_DShockwaveFlashEvents, &LIBID_ShockwaveFlashObjects>,
								   public IDispEventSimpleImpl<1, CFlashEvents, &DIID_DShockwaveFlashEvents>
{

public:
   CFlashEvents() {return;}



BEGIN_COM_MAP(CFlashEvents)
   COM_INTERFACE_ENTRY(DShockwaveFlashEvents)
   COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// Process FSCommand event
    HRESULT __stdcall FSCommand ( _bstr_t command, _bstr_t args )
	{
		MessageBox (NULL, "FS COMMAND!", "FS COMMAND!", MB_OK);
		return S_OK;
	}
	HRESULT __stdcall OnReadyStateChange (long newState)
	{
		MessageBox (NULL, "FS COMMAND!", "FS COMMAND!", MB_OK);
		return S_OK;
	}
	HRESULT __stdcall OnProgress (long percentDone) 
	{
		
		MessageBox (NULL, "FS COMMAND!", "FS COMMAND!", MB_OK);
		return S_OK;
	}

// Map events
BEGIN_SINK_MAP(CFlashEvents)
   SINK_ENTRY_INFO(1, DIID_DShockwaveFlashEvents, 150, FSCommand, &FSCommandInfo)
   SINK_ENTRY_INFO(1, DIID_DShockwaveFlashEvents, 4294966687, OnReadyStateChange, &OnReadyStateChangeInfo)
   SINK_ENTRY_INFO(1, DIID_DShockwaveFlashEvents, 1958, OnProgress, &OnProgressInfo)
END_SINK_MAP()

};


// Global event sink object
//class CFlashEvents CSinkObject;
CComObjectGlobal<CFlashEvents> CSinkObject;




int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG           msg;

	// Initialize windows
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_WINDOW_CLASS, szWindowClass, MAX_LOADSTRING);

	CoInitialize (NULL);
	//CoInitializeEx (NULL, COINIT_SPEED_OVER_MEMORY);
	// Intialize OLE
	if (OleInitialize(NULL)!=S_OK)
	{
		MessageBox (NULL, "Error: Could not initialize ole interface.", szTitle, MB_OK);
	    return 0;
	}
	// Fix me: Check for shockwave capabilities here
	// Fix me: Check mpeg capabilities of control here!

	MyRegisterClass(hInstance);
	AtlAxWinInit();

	if (!InitInstance (hInstance, SW_SHOW)) 
	{
		MessageBox (NULL, "Error: Unable to create window. (InitInstance)", szTitle, MB_OK);
		return 0;
	}

	
	// Initialize ATL
	_Module.Init(NULL, hInstance);

	// Message loop
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Perform any ATL cleanup
	_Module.Term();
	CoUninitialize ();
	return msg.wParam;
}


// Register our main window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LOADER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


// Initialize main window and other misc. modules
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   int screenx, screeny;
   hInst = hInstance;

   // Fix me: Set screen resolution to 640x480

   // Create window at maximum resolution
   screeny=GetSystemMetrics(SM_CYSCREEN);
   screenx=GetSystemMetrics(SM_CXSCREEN);

   hWnd = CreateWindowEx(WS_EX_APPWINDOW/*|WS_EX_TOPMOST*/,szWindowClass, 
	                     szTitle, WS_POPUP, 0,0,screenx, screeny,NULL,
						 NULL, hInstance, NULL);
   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow); /* 3-lines - Probably redundant */
   SetFocus(hWnd);
   UpdateWindow(hWnd);

   return TRUE;
}
	

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int             wmId,       wmEvent;
	int             screenx,    screeny;

	static HWND				  hWndShockWave;
	IUnknown				  *pIUnknown, *plIUnknown;
	IDispatch				  *pIDispatchEvents;
	IConnectionPointContainer *pICE;
	HRESULT					  hr;
	static DWORD			  pdwCookie;
	char					  cOut[250],  path[512];
	BSTR					  bsOut;

	switch (message) 
	{
	case WM_CREATE:
		// Initialize window specific items
		hMain=hWnd;

		// Make the flash control full screen
		screeny=GetSystemMetrics(SM_CYSCREEN);
		screenx=GetSystemMetrics(SM_CXSCREEN);	

		// Create flash control
		hWndShockWave = CreateWindowEx(0,"AtlAxWin", "{D27CDB6E-AE6D-11CF-96B8-444553540000}", WS_CHILD|WS_VISIBLE, 0,0,screenx,screeny,hWnd, (HMENU)NULL, GetModuleHandle(NULL), NULL);

		// Make connection to OCX on appropriate interfaces
		hr = AtlAxGetControl(hWndShockWave, &pIUnknown);
		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Unknown error occured while obtaining IUnknown interface from Shockwave object.", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}
		// Attempt to obtain a connection point container
		hr = pIUnknown->QueryInterface (IID_IConnectionPointContainer, (void **)&pICE);
		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Target control does not support IConnectionPointContainer method!", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}


		// Attempt to obtain a connection over the shockwave events interface
		hr = pICE->FindConnectionPoint (DIID_DShockwaveFlashEvents, &pCP);
		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Target control does not support IShockwaveFlashEvents method!", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}

	
		hr = CSinkObject.QueryInterface (__uuidof(IUnknown), (void **)&plIUnknown);
		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Could not obtain local lowest level interface.", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}

		

		hr = pCP->Advise (plIUnknown, &pdwCookie);
		

		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Target control did not accept event sink interface.", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}

		// Now obtain the outgoing interface
		hr = pIUnknown->QueryInterface(IID_IShockwaveFlash, (void **)&pIShockwave);

		if (!SUCCEEDED(hr))
		{
			MessageBox (hWnd, "Error: Target control does not support IShockwaveFlash interface.", szTitle, MB_OK);
			DestroyWindow (hWnd);
			PostQuitMessage(0);
			return TRUE;
		}
		
		// Add reference - ?? Transparently handeled?

		// Send our movie URL
		getcwd(path,512);
		sprintf(cOut, "%s%s%s", path,((*(path+(strlen(path)-1))=='\\')?"":"\\"),"FSCommand.swf");
		bsOut=T2BSTR(cOut);
		
		pIShockwave->put_Movie(bsOut);
		pIShockwave->put_Menu(0);


		
		return TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		break;
	case WM_DESTROY:
		// If we have an open connections - clean up
		if (hWndShockWave && pCP) 
			pCP->Unadvise (pdwCookie);
		if (hWndShockWave && pIShockwave)
			pIShockwave->Release ();
		if (hWndShockWave) 
			DestroyWindow(hWndShockWave);// Kill Shockwave
		PostQuitMessage(0);
		return TRUE;
		break;
   }
   return DefWindowProc (hWnd, message, wParam, lParam);
}

--------------------------------RESOURCE.H------------------------------------
//{{NO_DEPENDENCIES}}
// Microsoft Developer Studio generated include file.
// Used by Resources.rc
//
#define IDS_APP_TITLE                   1
#define IDS_WINDOW_CLASS                2
#define IDI_LOADER                      101
#define IDI_SMALL                       102

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
----------------------------------------swflash.tlh----------------------------
// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (50bb371a).
//
// e:\backupfolder\physics 30 bonus\loader\loader\debug\swflash.tlh
//
// C++ source equivalent of Win32 type library swflash.ocx
// compiler-generated file created 01/20/03 at 12:54:03 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

//namespace ShockwaveFlashObjects {

//
// Forward references and typedefs
//

struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
/* dual interface */ IShockwaveFlash;
struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
/* dispinterface */ DShockwaveFlashEvents;
struct /* coclass */ ShockwaveFlash;
struct /* coclass */ FlashProp;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IShockwaveFlash, __uuidof(IShockwaveFlash));
_COM_SMARTPTR_TYPEDEF(DShockwaveFlashEvents, __uuidof(IDispatch));

//
// Type library items
//

struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
IShockwaveFlash : IDispatch
{
    //
    // Property data
    //

    __declspec(property(get=GetWMode,put=PutWMode))
    _bstr_t WMode;
    __declspec(property(get=GetMovie,put=PutMovie))
    _bstr_t Movie;
    __declspec(property(get=GetSAlign,put=PutSAlign))
    _bstr_t SAlign;
    __declspec(property(get=GetMenu,put=PutMenu))
    VARIANT_BOOL Menu;
    __declspec(property(get=GetQuality,put=PutQuality))
    int Quality;
    __declspec(property(get=GetLoop,put=PutLoop))
    VARIANT_BOOL Loop;
    __declspec(property(get=GetFrameNum,put=PutFrameNum))
    long FrameNum;
    __declspec(property(get=GetBase,put=PutBase))
    _bstr_t Base;
    __declspec(property(get=Getscale,put=Putscale))
    _bstr_t scale;
    __declspec(property(get=GetDeviceFont,put=PutDeviceFont))
    VARIANT_BOOL DeviceFont;
    __declspec(property(get=GetEmbedMovie,put=PutEmbedMovie))
    VARIANT_BOOL EmbedMovie;
    __declspec(property(get=GetBGColor,put=PutBGColor))
    _bstr_t BGColor;
    __declspec(property(get=GetQuality2,put=PutQuality2))
    _bstr_t Quality2;
    __declspec(property(get=GetReadyState))
    long ReadyState;
    __declspec(property(get=GetScaleMode,put=PutScaleMode))
    int ScaleMode;
    __declspec(property(get=GetAlignMode,put=PutAlignMode))
    int AlignMode;
    __declspec(property(get=GetBackgroundColor,put=PutBackgroundColor))
    long BackgroundColor;
    __declspec(property(get=GetTotalFrames))
    long TotalFrames;
    __declspec(property(get=GetPlaying,put=PutPlaying))
    VARIANT_BOOL Playing;
    __declspec(property(get=GetSWRemote,put=PutSWRemote))
    _bstr_t SWRemote;

    //
    // Wrapper methods for error-handling
    //

    long GetReadyState ( );
    long GetTotalFrames ( );
    VARIANT_BOOL GetPlaying ( );
    void PutPlaying (
        VARIANT_BOOL Playing );
    int GetQuality ( );
    void PutQuality (
        int Quality );
    int GetScaleMode ( );
    void PutScaleMode (
        int scale );
    int GetAlignMode ( );
    void PutAlignMode (
        int align );
    long GetBackgroundColor ( );
    void PutBackgroundColor (
        long color );
    VARIANT_BOOL GetLoop ( );
    void PutLoop (
        VARIANT_BOOL Loop );
    _bstr_t GetMovie ( );
    void PutMovie (
        _bstr_t path );
    long GetFrameNum ( );
    void PutFrameNum (
        long FrameNum );
    HRESULT SetZoomRect (
        long left,
        long top,
        long right,
        long bottom );
    HRESULT Zoom (
        int factor );
    HRESULT Pan (
        long x,
        long y,
        int mode );
    HRESULT Play ( );
    HRESULT Stop ( );
    HRESULT Back ( );
    HRESULT Forward ( );
    HRESULT Rewind ( );
    HRESULT StopPlay ( );
    HRESULT GotoFrame (
        long FrameNum );
    long CurrentFrame ( );
    VARIANT_BOOL IsPlaying ( );
    long PercentLoaded ( );
    VARIANT_BOOL FrameLoaded (
        long FrameNum );
    long FlashVersion ( );
    _bstr_t GetWMode ( );
    void PutWMode (
        _bstr_t pVal );
    _bstr_t GetSAlign ( );
    void PutSAlign (
        _bstr_t pVal );
    VARIANT_BOOL GetMenu ( );
    void PutMenu (
        VARIANT_BOOL pVal );
    _bstr_t GetBase ( );
    void PutBase (
        _bstr_t pVal );
    _bstr_t Getscale ( );
    void Putscale (
        _bstr_t pVal );
    VARIANT_BOOL GetDeviceFont ( );
    void PutDeviceFont (
        VARIANT_BOOL pVal );
    VARIANT_BOOL GetEmbedMovie ( );
    void PutEmbedMovie (
        VARIANT_BOOL pVal );
    _bstr_t GetBGColor ( );
    void PutBGColor (
        _bstr_t pVal );
    _bstr_t GetQuality2 ( );
    void PutQuality2 (
        _bstr_t pVal );
    HRESULT LoadMovie (
        int layer,
        _bstr_t url );
    HRESULT TGotoFrame (
        _bstr_t target,
        long FrameNum );
    HRESULT TGotoLabel (
        _bstr_t target,
        _bstr_t label );
    long TCurrentFrame (
        _bstr_t target );
    _bstr_t TCurrentLabel (
        _bstr_t target );
    HRESULT TPlay (
        _bstr_t target );
    HRESULT TStopPlay (
        _bstr_t target );
    HRESULT SetVariable (
        _bstr_t name,
        _bstr_t value );
    _bstr_t GetVariable (
        _bstr_t name );
    HRESULT TSetProperty (
        _bstr_t target,
        int property,
        _bstr_t value );
    _bstr_t TGetProperty (
        _bstr_t target,
        int property );
    HRESULT TCallFrame (
        _bstr_t target,
        int FrameNum );
    HRESULT TCallLabel (
        _bstr_t target,
        _bstr_t label );
    HRESULT TSetPropertyNum (
        _bstr_t target,
        int property,
        double value );
    double TGetPropertyNum (
        _bstr_t target,
        int property );
    _bstr_t GetSWRemote ( );
    void PutSWRemote (
        _bstr_t pVal );

    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_ReadyState (
        long * thestate ) = 0;
    virtual HRESULT __stdcall get_TotalFrames (
        long * numframes ) = 0;
    virtual HRESULT __stdcall get_Playing (
        VARIANT_BOOL * Playing ) = 0;
    virtual HRESULT __stdcall put_Playing (
        VARIANT_BOOL Playing ) = 0;
    virtual HRESULT __stdcall get_Quality (
        int * Quality ) = 0;
    virtual HRESULT __stdcall put_Quality (
        int Quality ) = 0;
    virtual HRESULT __stdcall get_ScaleMode (
        int * scale ) = 0;
    virtual HRESULT __stdcall put_ScaleMode (
        int scale ) = 0;
    virtual HRESULT __stdcall get_AlignMode (
        int * align ) = 0;
    virtual HRESULT __stdcall put_AlignMode (
        int align ) = 0;
    virtual HRESULT __stdcall get_BackgroundColor (
        long * color ) = 0;
    virtual HRESULT __stdcall put_BackgroundColor (
        long color ) = 0;
    virtual HRESULT __stdcall get_Loop (
        VARIANT_BOOL * Loop ) = 0;
    virtual HRESULT __stdcall put_Loop (
        VARIANT_BOOL Loop ) = 0;
    virtual HRESULT __stdcall get_Movie (
        BSTR * path ) = 0;
    virtual HRESULT __stdcall put_Movie (
        BSTR path ) = 0;
    virtual HRESULT __stdcall get_FrameNum (
        long * FrameNum ) = 0;
    virtual HRESULT __stdcall put_FrameNum (
        long FrameNum ) = 0;
    virtual HRESULT __stdcall raw_SetZoomRect (
        long left,
        long top,
        long right,
        long bottom ) = 0;
    virtual HRESULT __stdcall raw_Zoom (
        int factor ) = 0;
    virtual HRESULT __stdcall raw_Pan (
        long x,
        long y,
        int mode ) = 0;
    virtual HRESULT __stdcall raw_Play ( ) = 0;
    virtual HRESULT __stdcall raw_Stop ( ) = 0;
    virtual HRESULT __stdcall raw_Back ( ) = 0;
    virtual HRESULT __stdcall raw_Forward ( ) = 0;
    virtual HRESULT __stdcall raw_Rewind ( ) = 0;
    virtual HRESULT __stdcall raw_StopPlay ( ) = 0;
    virtual HRESULT __stdcall raw_GotoFrame (
        long FrameNum ) = 0;
    virtual HRESULT __stdcall raw_CurrentFrame (
        long * FrameNum ) = 0;
    virtual HRESULT __stdcall raw_IsPlaying (
        VARIANT_BOOL * Playing ) = 0;
    virtual HRESULT __stdcall raw_PercentLoaded (
        long * __MIDL_0015 ) = 0;
    virtual HRESULT __stdcall raw_FrameLoaded (
        long FrameNum,
        VARIANT_BOOL * loaded ) = 0;
    virtual HRESULT __stdcall raw_FlashVersion (
        long * version ) = 0;
    virtual HRESULT __stdcall get_WMode (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_WMode (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_SAlign (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_SAlign (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_Menu (
        VARIANT_BOOL * pVal ) = 0;
    virtual HRESULT __stdcall put_Menu (
        VARIANT_BOOL pVal ) = 0;
    virtual HRESULT __stdcall get_Base (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_Base (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_scale (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_scale (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_DeviceFont (
        VARIANT_BOOL * pVal ) = 0;
    virtual HRESULT __stdcall put_DeviceFont (
        VARIANT_BOOL pVal ) = 0;
    virtual HRESULT __stdcall get_EmbedMovie (
        VARIANT_BOOL * pVal ) = 0;
    virtual HRESULT __stdcall put_EmbedMovie (
        VARIANT_BOOL pVal ) = 0;
    virtual HRESULT __stdcall get_BGColor (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_BGColor (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall get_Quality2 (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_Quality2 (
        BSTR pVal ) = 0;
    virtual HRESULT __stdcall raw_LoadMovie (
        int layer,
        BSTR url ) = 0;
    virtual HRESULT __stdcall raw_TGotoFrame (
        BSTR target,
        long FrameNum ) = 0;
    virtual HRESULT __stdcall raw_TGotoLabel (
        BSTR target,
        BSTR label ) = 0;
    virtual HRESULT __stdcall raw_TCurrentFrame (
        BSTR target,
        long * FrameNum ) = 0;
    virtual HRESULT __stdcall raw_TCurrentLabel (
        BSTR target,
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall raw_TPlay (
        BSTR target ) = 0;
    virtual HRESULT __stdcall raw_TStopPlay (
        BSTR target ) = 0;
    virtual HRESULT __stdcall raw_SetVariable (
        BSTR name,
        BSTR value ) = 0;
    virtual HRESULT __stdcall raw_GetVariable (
        BSTR name,
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall raw_TSetProperty (
        BSTR target,
        int property,
        BSTR value ) = 0;
    virtual HRESULT __stdcall raw_TGetProperty (
        BSTR target,
        int property,
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall raw_TCallFrame (
        BSTR target,
        int FrameNum ) = 0;
    virtual HRESULT __stdcall raw_TCallLabel (
        BSTR target,
        BSTR label ) = 0;
    virtual HRESULT __stdcall raw_TSetPropertyNum (
        BSTR target,
        int property,
        double value ) = 0;
    virtual HRESULT __stdcall raw_TGetPropertyNum (
        BSTR target,
        int property,
        double * pVal ) = 0;
    virtual HRESULT __stdcall get_SWRemote (
        BSTR * pVal ) = 0;
    virtual HRESULT __stdcall put_SWRemote (
        BSTR pVal ) = 0;
};

struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
DShockwaveFlashEvents : IDispatch
{
    //
    // Wrapper methods for error-handling
    //

    // Methods:
    HRESULT OnReadyStateChange (
        long newState );
    HRESULT OnProgress (
        long percentDone );
    HRESULT FSCommand (
        _bstr_t command,
        _bstr_t args );
};

struct __declspec(uuid("d27cdb6e-ae6d-11cf-96b8-444553540000"))
ShockwaveFlash;
    // [ default ] interface IShockwaveFlash
    // [ default, source ] dispinterface DShockwaveFlashEvents

struct __declspec(uuid("1171a62f-05d2-11d1-83fc-00a0c9089c5a"))
FlashProp;
    // [ default ] interface IUnknown

//
// Named GUID constants initializations
//

extern "C" const GUID __declspec(selectany) LIBID_ShockwaveFlashObjects =
    {0xd27cdb6b,0xae6d,0x11cf,{0x96,0xb8,0x44,0x45,0x53,0x54,0x00,0x00}};
extern "C" const GUID __declspec(selectany) IID_IShockwaveFlash =
    {0xd27cdb6c,0xae6d,0x11cf,{0x96,0xb8,0x44,0x45,0x53,0x54,0x00,0x00}};
extern "C" const GUID __declspec(selectany) DIID_DShockwaveFlashEvents =
    {0xd27cdb6d,0xae6d,0x11cf,{0x96,0xb8,0x44,0x45,0x53,0x54,0x00,0x00}};
extern "C" const GUID __declspec(selectany) CLSID_ShockwaveFlash =
    {0xd27cdb6e,0xae6d,0x11cf,{0x96,0xb8,0x44,0x45,0x53,0x54,0x00,0x00}};
extern "C" const GUID __declspec(selectany) CLSID_FlashProp =
    {0x1171a62f,0x05d2,0x11d1,{0x83,0xfc,0x00,0xa0,0xc9,0x08,0x9c,0x5a}};

//
// Wrapper method implementations
//

#include "swflash.tli"

//} // namespace ShockwaveFlashObjects

#pragma pack(pop)

----------------------------------swflash.tli-----------------------------------
// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (50bb371a).
//
// e:\backupfolder\physics 30 bonus\loader\loader\debug\swflash.tli
//
// Wrapper implementations for Win32 type library swflash.ocx
// compiler-generated file created 01/20/03 at 12:54:03 - DO NOT EDIT!

#pragma once

//
// interface IShockwaveFlash wrapper method implementations
//

inline long IShockwaveFlash::GetReadyState ( ) {
    long _result;
    HRESULT _hr = get_ReadyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IShockwaveFlash::GetTotalFrames ( ) {
    long _result;
    HRESULT _hr = get_TotalFrames(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline VARIANT_BOOL IShockwaveFlash::GetPlaying ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Playing(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutPlaying ( VARIANT_BOOL Playing ) {
    HRESULT _hr = put_Playing(Playing);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IShockwaveFlash::GetQuality ( ) {
    int _result;
    HRESULT _hr = get_Quality(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutQuality ( int Quality ) {
    HRESULT _hr = put_Quality(Quality);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IShockwaveFlash::GetScaleMode ( ) {
    int _result;
    HRESULT _hr = get_ScaleMode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutScaleMode ( int scale ) {
    HRESULT _hr = put_ScaleMode(scale);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline int IShockwaveFlash::GetAlignMode ( ) {
    int _result;
    HRESULT _hr = get_AlignMode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutAlignMode ( int align ) {
    HRESULT _hr = put_AlignMode(align);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IShockwaveFlash::GetBackgroundColor ( ) {
    long _result;
    HRESULT _hr = get_BackgroundColor(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutBackgroundColor ( long color ) {
    HRESULT _hr = put_BackgroundColor(color);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IShockwaveFlash::GetLoop ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Loop(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutLoop ( VARIANT_BOOL Loop ) {
    HRESULT _hr = put_Loop(Loop);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::GetMovie ( ) {
    BSTR _result;
    HRESULT _hr = get_Movie(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutMovie ( _bstr_t path ) {
    HRESULT _hr = put_Movie(path);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IShockwaveFlash::GetFrameNum ( ) {
    long _result;
    HRESULT _hr = get_FrameNum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutFrameNum ( long FrameNum ) {
    HRESULT _hr = put_FrameNum(FrameNum);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IShockwaveFlash::SetZoomRect ( long left, long top, long right, long bottom ) {
    HRESULT _hr = raw_SetZoomRect(left, top, right, bottom);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Zoom ( int factor ) {
    HRESULT _hr = raw_Zoom(factor);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Pan ( long x, long y, int mode ) {
    HRESULT _hr = raw_Pan(x, y, mode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Play ( ) {
    HRESULT _hr = raw_Play();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Stop ( ) {
    HRESULT _hr = raw_Stop();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Back ( ) {
    HRESULT _hr = raw_Back();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Forward ( ) {
    HRESULT _hr = raw_Forward();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::Rewind ( ) {
    HRESULT _hr = raw_Rewind();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::StopPlay ( ) {
    HRESULT _hr = raw_StopPlay();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::GotoFrame ( long FrameNum ) {
    HRESULT _hr = raw_GotoFrame(FrameNum);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline long IShockwaveFlash::CurrentFrame ( ) {
    long _result;
    HRESULT _hr = raw_CurrentFrame(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline VARIANT_BOOL IShockwaveFlash::IsPlaying ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = raw_IsPlaying(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IShockwaveFlash::PercentLoaded ( ) {
    long _result;
    HRESULT _hr = raw_PercentLoaded(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline VARIANT_BOOL IShockwaveFlash::FrameLoaded ( long FrameNum ) {
    VARIANT_BOOL _result;
    HRESULT _hr = raw_FrameLoaded(FrameNum, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IShockwaveFlash::FlashVersion ( ) {
    long _result;
    HRESULT _hr = raw_FlashVersion(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IShockwaveFlash::GetWMode ( ) {
    BSTR _result;
    HRESULT _hr = get_WMode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutWMode ( _bstr_t pVal ) {
    HRESULT _hr = put_WMode(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::GetSAlign ( ) {
    BSTR _result;
    HRESULT _hr = get_SAlign(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutSAlign ( _bstr_t pVal ) {
    HRESULT _hr = put_SAlign(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IShockwaveFlash::GetMenu ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Menu(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutMenu ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_Menu(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::GetBase ( ) {
    BSTR _result;
    HRESULT _hr = get_Base(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutBase ( _bstr_t pVal ) {
    HRESULT _hr = put_Base(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::Getscale ( ) {
    BSTR _result;
    HRESULT _hr = get_scale(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::Putscale ( _bstr_t pVal ) {
    HRESULT _hr = put_scale(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IShockwaveFlash::GetDeviceFont ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_DeviceFont(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutDeviceFont ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_DeviceFont(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IShockwaveFlash::GetEmbedMovie ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_EmbedMovie(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IShockwaveFlash::PutEmbedMovie ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_EmbedMovie(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::GetBGColor ( ) {
    BSTR _result;
    HRESULT _hr = get_BGColor(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutBGColor ( _bstr_t pVal ) {
    HRESULT _hr = put_BGColor(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IShockwaveFlash::GetQuality2 ( ) {
    BSTR _result;
    HRESULT _hr = get_Quality2(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutQuality2 ( _bstr_t pVal ) {
    HRESULT _hr = put_Quality2(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IShockwaveFlash::LoadMovie ( int layer, _bstr_t url ) {
    HRESULT _hr = raw_LoadMovie(layer, url);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::TGotoFrame ( _bstr_t target, long FrameNum ) {
    HRESULT _hr = raw_TGotoFrame(target, FrameNum);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::TGotoLabel ( _bstr_t target, _bstr_t label ) {
    HRESULT _hr = raw_TGotoLabel(target, label);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline long IShockwaveFlash::TCurrentFrame ( _bstr_t target ) {
    long _result;
    HRESULT _hr = raw_TCurrentFrame(target, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IShockwaveFlash::TCurrentLabel ( _bstr_t target ) {
    BSTR _result;
    HRESULT _hr = raw_TCurrentLabel(target, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IShockwaveFlash::TPlay ( _bstr_t target ) {
    HRESULT _hr = raw_TPlay(target);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::TStopPlay ( _bstr_t target ) {
    HRESULT _hr = raw_TStopPlay(target);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::SetVariable ( _bstr_t name, _bstr_t value ) {
    HRESULT _hr = raw_SetVariable(name, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _bstr_t IShockwaveFlash::GetVariable ( _bstr_t name ) {
    BSTR _result;
    HRESULT _hr = raw_GetVariable(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IShockwaveFlash::TSetProperty ( _bstr_t target, int property, _bstr_t value ) {
    HRESULT _hr = raw_TSetProperty(target, property, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _bstr_t IShockwaveFlash::TGetProperty ( _bstr_t target, int property ) {
    BSTR _result;
    HRESULT _hr = raw_TGetProperty(target, property, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IShockwaveFlash::TCallFrame ( _bstr_t target, int FrameNum ) {
    HRESULT _hr = raw_TCallFrame(target, FrameNum);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::TCallLabel ( _bstr_t target, _bstr_t label ) {
    HRESULT _hr = raw_TCallLabel(target, label);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IShockwaveFlash::TSetPropertyNum ( _bstr_t target, int property, double value ) {
    HRESULT _hr = raw_TSetPropertyNum(target, property, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline double IShockwaveFlash::TGetPropertyNum ( _bstr_t target, int property ) {
    double _result;
    HRESULT _hr = raw_TGetPropertyNum(target, property, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IShockwaveFlash::GetSWRemote ( ) {
    BSTR _result;
    HRESULT _hr = get_SWRemote(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IShockwaveFlash::PutSWRemote ( _bstr_t pVal ) {
    HRESULT _hr = put_SWRemote(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// dispinterface DShockwaveFlashEvents wrapper method implementations
//

inline HRESULT DShockwaveFlashEvents::OnReadyStateChange ( long newState ) {
    return _com_dispatch_method(this, DISPID_READYSTATECHANGE, DISPATCH_METHOD, VT_EMPTY, NULL, 
        L"\x0003", newState);
}

inline HRESULT DShockwaveFlashEvents::OnProgress ( long percentDone ) {
    return _com_dispatch_method(this, 0x7a6, DISPATCH_METHOD, VT_EMPTY, NULL, 
        L"\x0003", percentDone);
}

inline HRESULT DShockwaveFlashEvents::FSCommand ( _bstr_t command, _bstr_t args ) {
    return _com_dispatch_method(this, 0x96, DISPATCH_METHOD, VT_EMPTY, NULL, 
        L"\x0008\x0008", (BSTR)command, (BSTR)args);
}
