// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

/************************************************************************************
 *                                                                                  *
 * ColorCopDlg.cpp :: Color Cop
 *
 ************************************************************************************/


#include "stdafx.h"
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include "Label.h"            // used for the Links in the AboutDlg
#include "SystemTray.h"        // used to minimize to the systray
#include <commctrl.h>
#include <math.h>
#include <windows.h>
#include <winuser.h>
#include <random>
#include <cstdint>
#include <htmlhelp.h>

constexpr int WEBSAFE_STEP = 51;
constexpr int RGB_MIN = 0;
constexpr int RGB_MAX = 255;

class CAboutDlg : public CDialog
{
public:

    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    CLabel    m_maillink;
    CLabel    m_link;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation

protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_MAILLINK, m_maillink);
    DDX_Control(pDX, IDC_LINK, m_link);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Constants

const char* kpcTrayNotificationMsg_ = "color cop tray notification";

/////////////////////////////////////////////////////////////////////////////
// CColorCopDlg dialog

CColorCopDlg::CColorCopDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CColorCopDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CColorCopDlg)
    m_Greendec = 0;
    m_Bluedec = 0;
    m_Reddec = 0;
    m_Hexcolor = _T("");
    r = 0;
    g = 0;
    b = 0;
    m_bvisible = false;
    m_Black = 0;
    m_Cyan = 0;
    m_Magenta = 0;
    m_Yellow = 0;
    //}}AFX_DATA_INIT
    //m_hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, NULL);
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorCopDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorCopDlg)
    DDX_Control(pDX, IDC_ColorPick, m_ColorPick);
    DDX_Control(pDX, IDC_LBL4, m_Lbl4);
    DDX_Control(pDX, IDC_LBL3, m_Lbl3);
    DDX_Control(pDX, IDC_LBL2, m_Lbl2);
    DDX_Control(pDX, IDC_LBL1, m_Lbl1);
    DDX_Control(pDX, IDC_MAG_PLUS, m_MagPlus);
    DDX_Control(pDX, IDC_MAG_MINUS, m_MagMinus);
    DDX_Control(pDX, IDC_COLORPAL, m_ColorPalette);
    DDX_Control(pDX, IDC_MagWindow, m_MagWindow);
    DDX_Control(pDX, IDC_Q7, m_Q7);
    DDX_Control(pDX, IDC_Q6, m_Q6);
    DDX_Control(pDX, IDC_Q5, m_Q5);
    DDX_Control(pDX, IDC_Q4, m_Q4);
    DDX_Control(pDX, IDC_Q3, m_Q3);
    DDX_Control(pDX, IDC_Q2, m_Q2);
    DDX_Control(pDX, IDC_Q1, m_Q1);
    DDX_Control(pDX, IDC_CPreview, m_ColorPreview);
    DDX_Control(pDX, IDC_EXPAND_DIALOG, m_ExpandDialog);
    DDX_Control(pDX, IDC_MAGE, m_Magnifier);
    DDX_Control(pDX, IDC_EYELOC, m_EyeLoc);
    DDX_Text(pDX, IDC_GREEN, m_Greendec);
    DDX_Text(pDX, IDC_BLUE, m_Bluedec);
    DDX_Text(pDX, IDC_RED, m_Reddec);
    DDX_Text(pDX, IDC_HEXCOLOR, m_Hexcolor);
    DDX_Text(pDX, IDC_BLACK, m_Black);
    DDX_Text(pDX, IDC_CYAN, m_Cyan);
    DDX_Text(pDX, IDC_MAGENTA, m_Magenta);
    DDX_Text(pDX, IDC_YELLOW, m_Yellow);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorCopDlg, CDialog)

    ON_COMMAND(ID_OPTIONMENU, FireOptionMenu)
    ON_COMMAND(ID_FLOATUP, FloatPrecisionUp)
    ON_COMMAND(ID_FLOATDOWN, FloatPrecisionDown)

    //{{AFX_MSG_MAP(CColorCopDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_About, OnAbout)
    ON_WM_QUERYDRAGICON()
    ON_EN_CHANGE(IDC_GREEN, OnChangeGreen)
    ON_EN_CHANGE(IDC_BLUE, OnChangeBlue)
    ON_EN_CHANGE(IDC_RED, OnChangeRed)
    ON_BN_CLICKED(IDC_ColorPick, OnColorPick)
    ON_BN_CLICKED(IDC_COPYTOCLIP, OnCopytoclip)
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_EN_CHANGE(IDC_HEXCOLOR, OnChangeHexcolor)
    ON_WM_DESTROY()
    ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_COMMAND(ID_COLOR_REVERSE, OnColorReverse)
    ON_COMMAND(ID_OPTIONS_ALWAYSONTOP, OnOptionsAlwaysontop)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALWAYSONTOP, OnUpdateOptionsAlwaysontop)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELPHIMODE, OnUpdateOptionsDelphimode)
    ON_COMMAND(ID_OPTIONS_DELPHIMODE, OnOptionsDelphimode)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_AUTOCOPYTOCLIPBOARD, OnUpdateOptionsAutocopytoclipboard)
    ON_COMMAND(ID_OPTIONS_AUTOCOPYTOCLIPBOARD, OnOptionsAutocopytoclipboard)
    ON_COMMAND(ID_COLOR_RANDOM, OnColorRandom)
    ON_UPDATE_COMMAND_UI(ID_VIEW_HTMLHEXMODE, OnUpdateViewHtmlhexmode)
    ON_BN_CLICKED(IDC_EXPAND_DIALOG, OnExpandDialog)
    ON_COMMAND(ID_VIEW_HTMLHEXMODE, OnViewHtmlhexmode)
    ON_COMMAND(ID_FILE_ABOUT, OnFileAbout)
    ON_COMMAND(ID_COLOR_SNAPTOWEBSAFE, OnColorSnaptowebsafe)
    ON_COMMAND(ID_OPTIONS_OMITSYMBOL, OnOptionsOmitsymbol)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_OMITSYMBOL, OnUpdateOptionsOmitsymbol)
    ON_UPDATE_COMMAND_UI(ID_COLOR_SNAPTOWEBSAFE, OnUpdateColorSnaptowebsafe)
    ON_COMMAND(ID_OPTIONS_MINIMIZETOSYSTRAY, OnOptionsMinimizetosystray)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_MINIMIZETOSYSTRAY, OnUpdateOptionsMinimizetosystray)
    ON_COMMAND(ID_OPTIONS_UPPERCASEHEX, OnOptionsUppercasehex)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_UPPERCASEHEX, OnUpdateOptionsUppercasehex)
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_POPUP_SAMPLING_1PIXEL, ChangeTo1pixelSampling)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SAMPLING_1PIXEL, OnUpdatePopupSampling1pixel)
    ON_COMMAND(ID_POPUP_SAMPLING_3BY3AVERAGE, ChangeTo3x3Sampling)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SAMPLING_3BY3AVERAGE, OnUpdatePopupSampling3by3average)
    ON_COMMAND(ID_POPUP_SAMPLING_5BY5AVERAGE, ChangeTo5x5Sampling)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SAMPLING_5BY5AVERAGE, OnUpdatePopupSampling5by5average)
    ON_COMMAND(ID_POPUP_APPLICATION_EXPANDEDDIALOG, OnPopupApplicationExpandeddialog)
    ON_UPDATE_COMMAND_UI(ID_POPUP_APPLICATION_EXPANDEDDIALOG, OnUpdatePopupApplicationExpandeddialog)
    ON_COMMAND(ID_POPUP_HEXMODE_POWERBUILDER, OnPopupHexmodePowerbuilder)
    ON_UPDATE_COMMAND_UI(ID_POPUP_HEXMODE_POWERBUILDER, OnUpdatePopupHexmodePowerbuilder)
    ON_COMMAND(ID_POPUP_MODE_VISUALBASICHEX, OnPopupModeVisualbasichex)
    ON_UPDATE_COMMAND_UI(ID_POPUP_MODE_VISUALBASICHEX, OnUpdatePopupModeVisualbasichex)
    ON_COMMAND(ID_POPUP_MODE_VISUALCHEX, OnPopupModeVisualchex)
    ON_UPDATE_COMMAND_UI(ID_POPUP_MODE_VISUALCHEX, OnUpdatePopupModeVisualchex)
    ON_COMMAND(ID_POPUP_RESTORE, OnPopupRestore)
    ON_COMMAND(ID_POPUP_EXIT, OnPopupExit)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEWHEEL()
    ON_WM_TIMER()
    ON_COMMAND(ID_POPUP_APPLICATION_HELP, OnPopupApplicationHelp)
    ON_COMMAND(ID_POPUP_APPLICATION_EASYMOVE, OnPopupApplicationEasymove)
    ON_UPDATE_COMMAND_UI(ID_POPUP_APPLICATION_EASYMOVE, OnUpdatePopupApplicationEasymove)
    ON_COMMAND(ID_POPUP_APPLICATION_MINIMIZETOSYSTEMTRAYONSTART, OnPopupApplicationMinimizetosystemtrayonstart)
    ON_UPDATE_COMMAND_UI(ID_POPUP_APPLICATION_MINIMIZETOSYSTEMTRAYONSTART, OnUpdatePopupApplicationMinimizetosystemtrayonstart)
    ON_WM_CAPTURECHANGED()
    ON_COMMAND(ID_POPUP_COLOR_CONVERTTOGRAYSCALE, OnPopupColorConverttograyscale)
    ON_COMMAND(ID_POPUP_APPLICATION_ALLOWMULTIPLEINSTANCES, OnPopupApplicationAllowmultipleinstances)
    ON_UPDATE_COMMAND_UI(ID_POPUP_APPLICATION_ALLOWMULTIPLEINSTANCES, OnUpdatePopupApplicationAllowmultipleinstances)
    ON_COMMAND(ID_POPUP_COLOR_DETECTWEBSAFE, OnPopupColorDetectwebsafe)
    ON_UPDATE_COMMAND_UI(ID_POPUP_COLOR_DETECTWEBSAFE, OnUpdatePopupColorDetectwebsafe)
    ON_COMMAND(ID_POPUP_MODE_RGBFLOAT, OnPopupModeRgbfloat)
    ON_UPDATE_COMMAND_UI(ID_POPUP_MODE_RGBFLOAT, OnUpdatePopupModeRgbfloat)
    ON_UPDATE_COMMAND_UI(ID_POPUP_MODE_RGBINT, OnUpdatePopupModeRgbint)
    ON_COMMAND(ID_POPUP_MODE_RGBINT, OnPopupModeRgbint)
    ON_COMMAND(ID_POPUP_OPTIONS_MAGNIFYWHILEEYEDROPPING, OnPopupOptionsMagnifywhileeyedropping)
    ON_UPDATE_COMMAND_UI(ID_POPUP_OPTIONS_MAGNIFYWHILEEYEDROPPING, OnUpdatePopupOptionsMagnifywhileeyedropping)
    ON_COMMAND(ID_POPUP_OPTIONS_USECROSSHAIRCURSOR, OnPopupOptionsUsecrosshaircursor)
    ON_UPDATE_COMMAND_UI(ID_POPUP_OPTIONS_USECROSSHAIRCURSOR, OnUpdatePopupOptionsUsecrosshaircursor)
    ON_UPDATE_COMMAND_UI(ID_POPUP_OPTIONS_STARTCURSORONEYEDROPPER, OnUpdatePopupOptionsStartcursoroneyedropper)
    ON_COMMAND(ID_POPUP_OPTIONS_STARTCURSORONEYEDROPPER, OnPopupOptionsStartcursoroneyedropper)
    ON_COMMAND(ID_POPUP_SAMPLING_DECREASEMULTIPIXELAVERAGE, OnPopupSamplingDecreasemultipixelaverage)
    ON_COMMAND(ID_POPUP_SAMPLING_INCREASEMULTIPIXELAVERAGE, OnPopupSamplingIncreasemultipixelaverage)
    ON_COMMAND(ID_POPUP_SAMPLING_MULTIPIXEL, OnPopupSamplingMultipixel)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SAMPLING_MULTIPIXEL, OnUpdatePopupSamplingMultipixel)
    ON_COMMAND(ID_POPUP_SPACE_RGB, OnPopupSpaceRgb)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SPACE_RGB, OnUpdatePopupSpaceRgb)
    ON_COMMAND(ID_POPUP_SPACE_CMYK, OnPopupSpaceCmyk)
    ON_UPDATE_COMMAND_UI(ID_POPUP_SPACE_CMYK, OnUpdatePopupSpaceCmyk)
    ON_EN_CHANGE(IDC_BLACK, OnChangeBlack)
    ON_EN_CHANGE(IDC_CYAN, OnChangeCyan)
    ON_EN_CHANGE(IDC_MAGENTA, OnChangeMagenta)
    ON_EN_CHANGE(IDC_YELLOW, OnChangeYellow)
    ON_WM_INITMENUPOPUP()
    ON_COMMAND(ID_POPUP_MODE_CLARIONHEX, OnPopupModeClarionhex)
    ON_UPDATE_COMMAND_UI(ID_POPUP_MODE_CLARIONHEX, OnUpdatePopupModeClarionhex)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCopDlg message handlers

BOOL CColorCopDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetupSystemMenu();            // add about and always on top to the system menu

    bool FoundDatFile = LoadPersistentVariables();

    ToggleOnTop(false); //make always on top, unless save file said not to

    SetupWindowRects();
    SetupStatusBar();

    TestForExpand();    // do not call this before SetupWindowRects();

    if (!m_ToolTip.Create(this)) {
        TRACE0("Unable to create a tool tip obj");
    } else {
        // Add tool tips to the controls, either by hard coded string
        // or using the string table resource
        m_ToolTip.AddTool( &m_ExpandDialog, IDS_EXPANDEDDIALOG);
        m_ToolTip.AddTool( &m_ColorPick, IDS_CUSTOM_COLOR);
        m_ToolTip.Activate(TRUE);
    }

    nTrayNotificationMsg_ = RegisterWindowMessage(kpcTrayNotificationMsg_);

    //application variables
    m_isMagPlusDown = m_isMagMinusDown = bOldClrExist = FALSE;
    m_bCalcColorPal = m_isEyedropping = m_isMagnifying = FALSE;
    m_OldRed = m_OldBlue = m_OldGreen = 0;
    bMinimized_= false;
    pTrayIcon_ = 0;

    // small one
    hIcon_ = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), NULL);
    // large one
    m_hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
        GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), NULL);


    SetIcon(hIcon_, FALSE);    // small icon 16x16
    SetIcon(m_hIcon, TRUE);    // big icon 32x32


    bRelativePosition = false;
    m_MagDrop = false;

    // PreLoad Cursors
    CWinApp* pApp = AfxGetApp();    // get a pointer to the one and only CWinApp

    if (pApp)
    {
        m_hMagCursor = pApp->LoadCursor(IDC_MEDIUM_MAGNIFY);

        m_Magnifier.SetIcon(m_hMagCursor);
        m_hHandCursor = pApp->LoadCursor(IDC_HANDPOINTER);
        m_hMoveCursor = pApp->LoadCursor(IDC_CURMOVE);
        m_hBlank = pApp->LoadIcon(IDI_BLANK);
        m_hStandardCursor = pApp->LoadStandardCursor(IDC_ARROW);
    }

    // upgrade case -- make sure there is a color space set
    if ((!(m_Appflags & SpaceCMYK)) && (!(m_Appflags & SpaceRGB))) {
        m_Appflags |= SpaceRGB;
    } else if (m_Appflags & SpaceCMYK) {
        ChangeColorSpace(false);
    } else {//rgb
        ChangeColorSpace(true);
    }

    if (m_Appflags & USECROSSHAIR) {

            CWinApp* pApp = AfxGetApp();
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));



        m_hEyeCursor = pApp->LoadCursor(IDC_MYCROSS);
        m_EyeLoc.SetIcon(m_hEyeCursor);                        // location cursor

    } else {

        m_hEyeCursor = pApp->LoadCursor(IDC_EYEDROPPER);    // eyedropper cursor

        if (m_Appflags & Sampling5x5) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_5X5));
        } else if (m_Appflags & Sampling3x3) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_3X3));
        } else { // 1x1 or multi
            m_EyeLoc.SetIcon(m_hEyeCursor);                    // location cursor
        }
    }




    m_MagLevel = 5;
    m_FloatPrecision = 2;
    OnconvertRGB();
    CalcColorPal();
    OnCopytoclip();


    // set focus on the eyedropper so there is no cursor
    m_EyeLoc.SetFocus();


    // user wants to minimize on app start
    if (m_Appflags & MinimizeonStart)
    {
        m_bvisible = true;
        bMinimized = true;
        ShowWindow(SW_MINIMIZE);

        SetupTaskBarButton();
        SetupTrayIcon();



    } else     if (m_Appflags & SETCURSORONEYEDROP)
    {
        m_bvisible = true;
        // don't move the cursor if the app is minimized
        CRect eyerect;
        m_EyeLoc.GetWindowRect(&eyerect);
        SetCursorPos(eyerect.CenterPoint().x, eyerect.CenterPoint().y);


    }


    return FALSE;  // return TRUE  unless you set the focus to a control

}

void CColorCopDlg::SetupSystemMenu()
{
    // Load accelerator resource..
    m_hAcceleratorTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_COLORCOP_ACCEL));

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    //ASSERT(IDM_ABOUTBOX < 0xF000);
    //ASSERT(IDM_ALWAYSONTOP < 0xF000);
    //ASSERT(IDM_OPTIONMENU < 0xF000);

    //get the handle to the control menu
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        // Load string and add 'About Color Cop' to the system menu
        CString tempstr;
        tempstr.LoadString(IDS_ABOUTBOX);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_ABOUTBOX, tempstr);

        // Load string and add 'Option Menu' to the system menu
        tempstr.LoadString(IDS_OPTIONMENU);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_OPTIONMENU, tempstr);

         // Load stringon top to system menu
        tempstr.LoadString(IDS_ALWAYSONTOP);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_ALWAYSONTOP, tempstr);
        pSysMenu->InsertMenu(2, MF_BYPOSITION | MF_SEPARATOR, NULL);

        // disable maxmize in the system menu
        pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED );

        // disable sizing in the system menu
        pSysMenu->EnableMenuItem(SC_SIZE, MF_BYCOMMAND | MF_GRAYED );
    }
    return;
}

bool CColorCopDlg::LoadPersistentVariables() {
  bool retval = false;

  // Build full bitmap path safely
  CString strBMPFile = GetTempFolder();
  strBMPFile += BMP_FILE_DIR;
  strBMPFile += BMP_FILE;

  // Load bitmap from file
  hBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, strBMPFile,
                               IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  if (hBitmap != nullptr) {
    hZoomBitmap = hBitmap;
  } else {
    // Bitmap missing or unreadable — not fatal
    TRACE(_T("Warning: Could not load bitmap: %s\n"), strBMPFile.GetString());
  }

  // Fix window position if saved as "minimized"
  // Win9x stored minimized windows at (3000,3000)
  // NT stored them at (-32000,-32000)
  //
  if (WinLocX < 0 || WinLocY < 0 || WinLocX == 3000 || WinLocY == 3000) {
    WinLocX = 200;
    WinLocY = 200;
  }

  // Restore window position (no size change)
  SetWindowPos(&wndTopMost, WinLocX, WinLocY, 0, 0, SWP_NOSIZE);

  retval = true;
  return retval;
}

void CColorCopDlg::SetupWindowRects()
{

    // setup color window rect based on the Static box
    HWND temphandle;
    CWnd::GetDlgItem(IDC_CPreview,&temphandle);
    ::GetWindowRect(temphandle,&buttonrect);
    CWnd::ScreenToClient(&buttonrect);


    // Setup Q1rect for color history
    CWnd::GetDlgItem(IDC_Q1, &temphandle);
    ::GetWindowRect(temphandle, &Q1rect);
    CWnd::ScreenToClient(&Q1rect);

    // Setup Q2rect for color history
    CWnd::GetDlgItem(IDC_Q2, &temphandle);
    ::GetWindowRect(temphandle, &Q2rect);
    CWnd::ScreenToClient(&Q2rect);

    // Setup Q3rect for color history
    CWnd::GetDlgItem(IDC_Q3, &temphandle);
    ::GetWindowRect(temphandle, &Q3rect);
    CWnd::ScreenToClient(&Q3rect);

    // Setup Q4rect for color history
    CWnd::GetDlgItem(IDC_Q4, &temphandle);
    ::GetWindowRect(temphandle, &Q4rect);
    CWnd::ScreenToClient(&Q4rect);

    // Setup Q5rect for color history
    CWnd::GetDlgItem(IDC_Q5, &temphandle);
    ::GetWindowRect(temphandle, &Q5rect);
    CWnd::ScreenToClient(&Q5rect);

    // Setup Q6rect for color history
    CWnd::GetDlgItem(IDC_Q6, &temphandle);
    ::GetWindowRect(temphandle, &Q6rect);
    CWnd::ScreenToClient(&Q6rect);

    // Setup Q7rect for color history
    CWnd::GetDlgItem(IDC_Q7, &temphandle);
    ::GetWindowRect(temphandle, &Q7rect);
    CWnd::ScreenToClient(&Q7rect);

    // Setup color palette  rect
    CWnd::GetDlgItem(IDC_COLORPAL, &temphandle);
    ::GetWindowRect(temphandle, &colorpalrect);
    CWnd::ScreenToClient(&colorpalrect);

    HWND maghand;
    CWnd::GetDlgItem(IDC_MagWindow,&maghand);
    ::GetWindowRect(maghand, &magrect);
    CWnd::ScreenToClient(magrect);

    RECT exprect;
    HWND expandbutton;
    CWnd::GetDlgItem(IDC_EXPAND_DIALOG, &expandbutton);
    ::GetWindowRect(expandbutton, &exprect);
    CWnd::ScreenToClient(&exprect);

    // Get a rect of the entire window.
    CWnd::GetWindowRect(&CCopRect);
    CWnd::ScreenToClient(&CCopRect);
    CCopsmRect = CCopRect;            // copy rect

    // large sizes
    lgWidth = CCopRect.right - CCopRect.left;
    lgHeight = CCopRect.bottom - CCopRect.top;

    // small sizes
    smWidth = (exprect.right - CCopRect.left) + 11;
    smHeight = (exprect.bottom - CCopRect.top) + 11;

    // Setup magnify plus level rect
    CWnd::GetDlgItem(IDC_MAG_PLUS, &temphandle);
    ::GetWindowRect(temphandle, &magplus);
    CWnd::ScreenToClient(&magplus);

    magplus.right = magplus.left + 11;
    magplus.bottom = magplus.top + 11;

    // Setup magnify minus level rect
    CWnd::GetDlgItem(IDC_MAG_MINUS, &temphandle);
    ::GetWindowRect(temphandle, &magminus);
    CWnd::ScreenToClient(&magminus);

    magminus.right = magminus.left + 11;
    magminus.bottom = magminus.top + 11;


    CRect recttemp;

    // align Cyan with Red
    CWnd::GetDlgItem(IDC_RED, &temphandle);
    ::GetWindowRect(temphandle, &recttemp);
    CWnd::ScreenToClient(&recttemp);
    GetDlgItem(IDC_CYAN)->MoveWindow(&recttemp);

    // align Magenta with Green

    CWnd::GetDlgItem(IDC_GREEN, &temphandle);
    ::GetWindowRect(temphandle, &recttemp);
    CWnd::ScreenToClient(&recttemp);
    GetDlgItem(IDC_MAGENTA)->MoveWindow(&recttemp);

    // align Yellow with Blue
    CWnd::GetDlgItem(IDC_BLUE, &temphandle);
    ::GetWindowRect(temphandle, &recttemp);
    CWnd::ScreenToClient(&recttemp);
    GetDlgItem(IDC_YELLOW)->MoveWindow(&recttemp);

    // align Black with Black
    CWnd::GetDlgItem(IDC_BLACK, &temphandle);
    ::GetWindowRect(temphandle, &recttemp);
    CWnd::ScreenToClient(&recttemp);
    GetDlgItem(IDC_BLACK)->MoveWindow(&recttemp);

    // align Blue with Black
    //GetDlgItem(IDC_BLUE)->MoveWindow(&recttemp);


}

void CColorCopDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    // this is function is called when the user selects an item
    // from the system menu. (right clicking on the minimized program,
    // or right clicking on the system icon

    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();

    } else if ((nID & 0xFFF0) == IDM_OPTIONMENU) {
        FireOptionMenu();
    } else if ((nID & 0xFFF0) == IDM_ALWAYSONTOP) {
        m_Appflags ^= AlwaysOnTop; //AutobOnTop=bOnTop?FALSE:TRUE;
        ToggleOnTop(true);

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
            if (m_Appflags & AlwaysOnTop)
            {
                // check the menu item
                pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_CHECKED);
            } else {
                pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_UNCHECKED);
                // uncheck the item
            }
        }
    } else
    {
        BOOL bOldMin = bMinimized;    // remember previous state

        if (nID == SC_MINIMIZE) {
            bMinimized = true;
        } else if (nID == SC_RESTORE) {
            bMinimized = false;
        }

        CDialog::OnSysCommand(nID, lParam);

        if (bOldMin != bMinimized)
        {
            SetupTrayIcon();
            SetupTaskBarButton();

        }
    }

}

void CColorCopDlg::SetupTaskBarButton()
{

    if ((bMinimized) && (m_Appflags & MimimizetoTray ))
    {
        // hide the tray icon if it belongs in the system tray
        ShowWindow(SW_HIDE);
        m_bvisible = false;

    } else {
        ShowWindow(SW_SHOW);
        m_bvisible = true;
    }

}

void CColorCopDlg::SetupTrayIcon()
{
    if (bMinimized && (pTrayIcon_ == 0) && (m_Appflags & MimimizetoTray)) {

        CString strAppName;
        strAppName.LoadString(IDS_APPLICATION_NAME);

        pTrayIcon_ = new CSystemTray;
        pTrayIcon_->Create(0, nTrayNotificationMsg_, strAppName,
                hIcon_, IDR_SYSTRAY_MENU);
    } else {
        delete pTrayIcon_;
        pTrayIcon_ = 0;
    }
}

void CColorCopDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    } else
    {
        CDialog::OnPaint();
        // only paint here..

        HWND CCopHWNDtemp=AfxGetApp()->GetMainWnd()->m_hWnd;

        //::SetForegroundWindow(CCopHWND);        // warning:        the task bar will flash if you call this
                                                //                when the window is losing focus

        hdc = ::GetDC(CCopHWNDtemp);
        if (hBitmap) {

            hdcMem = ::CreateCompatibleDC(hdc);

            ::SelectObject (hdcMem, hBitmap);

            if (m_Appflags & ExpandedDialog)

            ::BitBlt(hdc, // destination DC
                     magrect.TopLeft().x+2, magrect.TopLeft().y+2 , // upper left dest
                     magrect.Width()-4, magrect.Height()-4,  // width of dest rect
                     hdcMem,  // source DC
                     0, 0,      // upper left source
                     SRCCOPY);// mode

            ::DeleteDC (hdcMem);                        // kill the temporary DC
            hdcMem = NULL;
          }

        if (m_Appflags & ExpandedDialog) {
            ::DrawEdge(hdc, &magrect, EDGE_SUNKEN, BF_RECT);
        }
        ::ReleaseDC(CCopHWNDtemp, hdc);  // let go of the memory

        DisplayColor();    // keep the color window showing
    }
}




void CColorCopDlg::RecalcZoom()
{
    // zoom level has changed while not magnifying..

    int    magwidth = magrect.Width() / m_MagLevel;
    int magheight = magrect.Height() / m_MagLevel;
    HWND CCopHWNDtemp=AfxGetApp()->GetMainWnd()->m_hWnd;

    if (hZoomBitmap)
    {
        hdc = ::GetDC(CCopHWNDtemp);   // needed only for color depth

        hdcMem = ::CreateCompatibleDC(hdc);
        hdcZoomMem = ::CreateCompatibleDC(hdc);

       ::SelectObject(hdcMem, hBitmap);
       ::SelectObject(hdcZoomMem, hZoomBitmap);

           ::StretchBlt(hdcMem, // destination DC
                 0, 0, // upper left dest
                 magrect.Width(), magrect.Height(),  // width of dest rect
                 hdcZoomMem,  // source DC
                    ((magrect.right - magrect.left)/2) - (magwidth/2), // x coordinate of source
                 ((magrect.bottom - magrect.top)/2) - (magheight/2), // y coordinate of source
                 magwidth, magheight,        // width of source
                 SRCCOPY);    // raster mode


        ::DeleteDC (hdcZoomMem);// kill the temporary DC
        ::DeleteDC (hdcMem);    // kill the temporary DC

        ::ReleaseDC (CCopHWNDtemp, hdc);  // let go of the memory
    }
    InvalidateRect(&buttonrect, FALSE);
    return;
}

void CColorCopDlg::OnconvertRGB() {

    TestForWebsafe();    // before conversion

    if (m_Appflags & ModeHTML) {

        m_Hexcolor.Format("#%.2x%.2x%.2x", m_Reddec, m_Greendec, m_Bluedec);
        if (m_Appflags & OmitPound) {
            if (m_Hexcolor.Left(1) == '#') {
                m_Hexcolor.Delete(0);
            }
        }
    } else if (m_Appflags & ModeDelphi) {

        m_Hexcolor.Format("$00%.2x%.2x%.2x", m_Bluedec, m_Greendec, m_Reddec);
        if (m_Appflags & OmitPound) {
            if (m_Hexcolor.Left(1) == '$') {
                m_Hexcolor.Delete(0);
            }
        }

    } else if (m_Appflags & ModePowerBuilder) {

        m_Hexcolor.Format("%d", (65536 * m_Bluedec) + (256 * m_Greendec) + (m_Reddec));

    } else if (m_Appflags & ModeVisualBasic) {

        m_Hexcolor.Format("&H%.2x%.2x%.2x", m_Bluedec, m_Greendec, m_Reddec);

    } else if (m_Appflags & ModeClarion) {

        // same as VB, but in addition
        //1) remove the &H at start.
        //2) prefix with 0
        //3) add H after the color
        m_Hexcolor.Format("0%.2x%.2x%.2xH", m_Bluedec, m_Greendec, m_Reddec);


    } else if (m_Appflags & RGBINT) {

        m_Hexcolor.Format("%d,%d,%d", m_Reddec, m_Greendec, m_Bluedec);

    } else if (m_Appflags & RGBFLOAT) {
        r = (float)m_Reddec / 255.0f;
        g = (float)m_Greendec / 255.0f;
        b = (float)m_Bluedec / 255.0f;
        m_Hexcolor.Format(_T("%0.*f,%0.*f,%0.*f"), m_FloatPrecision, r, m_FloatPrecision, g, m_FloatPrecision, b);

    } else if (m_Appflags & ModeVisualC) {

        m_Hexcolor.Format("0x00%.2x%.2x%.2x", m_Bluedec, m_Greendec, m_Reddec);
    }

    // Only apply uppercase formatting when NOT in Visual C++ hex mode.
    // // VC+ mode requires lowercase hex, so skip TestForUpperHex() in that case.
    if ((m_Appflags & ModeVisualC) == 0)
    {
        TestForUpperHex();
    }

    // Push updated member variables (including m_Hexcolor) out to their dialog
    // controls.
    UpdateData(FALSE);


    if ((m_isEyedropping) && (m_bCalcColorPal)) {
        CalcColorPal();        // Re-Calculate color palette
    }
    Invalidate(FALSE);    // Call WM_PAINT, but don't erase background
}

void CColorCopDlg::OnconvertHEX()
{
    TestForWebsafe();    // before

    // --- Format hex string ---
    if (m_Appflags & ModeHTML)
    {
        // Always emit full 6‑digit HTML hex
        m_Hexcolor.Format("#%.2x%.2x%.2x",
                          m_Reddec,
                          m_Greendec,
                          m_Bluedec);
    } else {
        // Delphi format: $00BBGGRR
        m_Hexcolor.Format("$00%.2x%.2x%.2x",
                          m_Bluedec,
                          m_Greendec,
                          m_Reddec);
    }

    // --- Optional prefix removal ---
    if (m_Appflags & OmitPound)
    {
        if ((m_Appflags & ModeHTML) && m_Hexcolor.Left(1) == "#")
            m_Hexcolor.Delete(0);

        if ((m_Appflags & ModeDelphi) && m_Hexcolor.Left(1) == "$")
            m_Hexcolor.Delete(0);
    }

    OnCopytoclip();
    Invalidate(FALSE);    // Call WM_PAINT, but don't erase background
}

void CColorCopDlg::CalcColorPal()
{
    // generate a new color palette from the current color

    setSeedColor();        // step 1. - set the RGB color
                        //         - calc the Hue, Sat, Light
                        //         - create the swatch
    palcol = 0;
    handleShifts();
}

double CColorCopDlg::plusValue(double num)
{
    num = num + 0.15;
    if (num > 1.0)
        num = 1.0;

    return num;
}

double CColorCopDlg::minusValue(double num)
{
    num = num - 0.15;
    if (num < 0.0)
        num = num + 1.0;
    return num;
}


void CColorCopDlg::handleShifts()
{
    int i;
    setupSwatches();
    printSwatch();

    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = plusValue(Swatch[i].B);
        Swatch[i].C = plusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = plusValue(Swatch[i].B);
    }
    printSwatch();

    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = plusValue(Swatch[i].B);
        Swatch[i].C = minusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = minusValue(Swatch[i].B);
        Swatch[i].C = plusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = minusValue(Swatch[i].B);
    }
    printSwatch();
    setupSwatches();
    for(i = 0; i < 6; i++){
        Swatch[i].B = minusValue(Swatch[i].B);
        Swatch[i].C = minusValue(Swatch[i].C);
    }
    printSwatch();

}

void CColorCopDlg::printSwatch()
{
    for(int i = 0; i < 6; i++) {

        HSLtoRGB(Swatch[i].A,Swatch[i].B,Swatch[i].C);
        Swatch[i].A = r;
        Swatch[i].B = g;
        Swatch[i].C = b;

        ColorPal[i][palcol] = RGB(Swatch[i].A, Swatch[i].B, Swatch[i].C);
    }
    palcol++;
}

void CColorCopDlg::setupSwatches()
{
    Swatch[0].A = OrigSwatch.A;
    Swatch[0].B = OrigSwatch.B;
    Swatch[0].C = OrigSwatch.C;

    for(int i = 1; i < 6; i++) {
        Swatch[i].A = shiftHue(Swatch[i-1].A);
        Swatch[i].B = Swatch[i-1].B;
        Swatch[i].C = Swatch[i-1].C;
    }
}

double CColorCopDlg::shiftHue(double hue)
{
    double rethue = (hue + (60.0 / 360.0));

    return rethue;
}

void CColorCopDlg::setSeedColor()
{
    // here we set the RGB color
    OrigSwatch.A = m_Reddec;
    OrigSwatch.B = m_Greendec;
    OrigSwatch.C = m_Bluedec;

    // convert the color to HSL
    RGBtoHSL((int) OrigSwatch.A, (int) OrigSwatch.B, (int) OrigSwatch.C);

    // set the swatch to the HSL values
    OrigSwatch.A = Hue;
    OrigSwatch.B = Sat;
    OrigSwatch.C = Light;
}

void CColorCopDlg::HSLtoRGB(double H, double S, double L) {

    if(S == 0){
        r = g = b = S * 255.0;
    } else {


        double h = (H - int(H)) * 6.0;
        int caseH = (int) int(h);

        double f = h - int(h);
        double p = L * (1.0 - S);
        double q = L * (1.0 - S * f);
        double t = L * (1.0 - (S * (1.0 - f)));

        switch (caseH) {
            case 0:
            r = (int) (L * 255.0);
            g = (int) (t * 255.0);
            b = (int) (p * 255.0);
            break;
            case 1:
            r = (int) (q * 255.0);
            g = (int) (L * 255.0);
            b = (int) (p * 255.0);
            break;
            case 2:
            r = (int) (p * 255.0);
            g = (int) (L * 255.0);
            b = (int) (t * 255.0);
            break;
            case 3:
            r = (int) (p * 255.0);
            g = (int) (q * 255.0);
            b = (int) (L * 255.0);
            break;
            case 4:
            r = (int) (t * 255.0);
            g = (int) (p * 255.0);
            b = (int) (L * 255.0);
            break;
            case 5:
            r = (int) (L * 255.0);
            g = (int) (p * 255.0);
            b = (int) (q * 255.0);
            break;
          }
    }
}

void CColorCopDlg::UpdateCMYKFromRGB(int red, int green, int blue) {

    double r, g, b;
    r = (double)red / 255.0;
    g = (double)green / 255.0;
    b = (double)blue / 255.0;
    m_Cyan  = (int) pow(1.0 - r,  45);
    m_Magenta = (int) pow(1.0 - g, 45);
    m_Yellow = (int) pow(1.0 - b, 45);

    m_Black = __min(__min(m_Cyan, m_Magenta), m_Yellow);

    m_Cyan = m_Cyan - m_Black;
    m_Magenta = m_Magenta - m_Black;
    m_Yellow = m_Yellow - m_Black;

}

void CColorCopDlg::RGBtoHSL(double R, double G, double B)
{
    // the function converts the RGB model to the HSL model.

    double MinNum, MaxNum, Diff;
    double R_Dist, G_Dist, B_Dist;

    /////////////////////////////////////
    // get min, max, and diff
    //
    MinNum = __min(R, __min(G,B));        // min
    MaxNum = __max(R, __max(G,B));        // max

    Diff = (MaxNum - MinNum);            // diff

    if (Diff == 0) {    // this is a greyscale color
        Diff = 5.0;    // since greyscale colors don't have compliments,
    }                    // lets give it something

    Light = MaxNum / 255.0;            // find the Light




    // find the Saturation
    if((MaxNum == 255) || (MinNum == 0)) {
        Sat = 1.0;
    }

    if (MaxNum != 0) {
        Sat = ((double) Diff) / ((double) MaxNum);
    }


    // find the Hue
    R_Dist = (double) (MaxNum - R) / (double) (Diff);
    G_Dist = (double) (MaxNum - G) / (double) (Diff);
    B_Dist = (double) (MaxNum - B) / (double) (Diff);

    if (R == MaxNum) {
        Hue = B_Dist - G_Dist;
    } else     if (G == MaxNum) {
        Hue = 2.0 + R_Dist - B_Dist;
    } else     if (B == MaxNum) {
        Hue = 4.0 + G_Dist - R_Dist;
    }
    Hue = Hue / 6.0;


    if (Hue < 0.0){
        Hue = (Hue + 1.0);
    }

}

void CColorCopDlg::DisplayColor()
{
    CDC *pDC = GetDC();

    int soff = 0;
    CBrush blackbrush;    //create a black brush
    blackbrush.CreateSolidBrush(0x00000000);

    CBrush greybrush;    //create a grey brush
    greybrush.CreateSolidBrush(0x00808080);

    pDC->FillSolidRect(buttonrect, RGB(m_Reddec,m_Greendec,m_Bluedec));
    pDC->DrawEdge(buttonrect, EDGE_SUNKEN, BF_RECT);

    if (m_Appflags & ExpandedDialog) {

        pDC->FillSolidRect(&Q1rect, ColorHistory[0]);
        pDC->FrameRect(&Q1rect, &blackbrush);

        pDC->FillSolidRect(&Q2rect, ColorHistory[1]);
        pDC->FrameRect(&Q2rect, &blackbrush);

        pDC->FillSolidRect(&Q3rect, ColorHistory[2]);
        pDC->FrameRect(&Q3rect, &blackbrush);

        pDC->FillSolidRect(&Q4rect, ColorHistory[3]);
        pDC->FrameRect(&Q4rect, &blackbrush);

        pDC->FillSolidRect(&Q5rect, ColorHistory[4]);
        pDC->FrameRect(&Q5rect, &blackbrush);

        pDC->FillSolidRect(&Q6rect, ColorHistory[5]);
        pDC->FrameRect(&Q6rect, &blackbrush);

        pDC->FillSolidRect(&Q7rect, ColorHistory[6]);
        pDC->FrameRect(&Q7rect, &blackbrush);

        int palWidth = (colorpalrect.right - colorpalrect.left);
        int palHeight = (colorpalrect.bottom - colorpalrect.top);

        m_nwide = (int) (palWidth / 6.0);
        m_ntall = (int) (palHeight / 7.0);

        CRect insiderect = colorpalrect;
        insiderect.right = insiderect.left + m_nwide;
        insiderect.bottom = insiderect.top + m_ntall;

        for (int col = 0; col < 7; col++) {
            if (col) {
                // not the top row

                insiderect.top+=m_ntall;
                insiderect.bottom+=m_ntall;
            }
            for (int row = 0; row< 6; row++) {
                pDC->FillSolidRect(insiderect, ColorPal[row][col]);

                //    pDC->FrameRect(insiderect, &blackbrush);    // frame each color
                insiderect.right+=m_nwide;
                insiderect.left+=m_nwide;
            }

            insiderect.right-=m_nwide * 6;
            insiderect.left-=m_nwide * 6;
        }

        // calculate the rect for FrameRect
        colorpalrect.right = colorpalrect.left + m_nwide * 6;
        colorpalrect.bottom = colorpalrect.top + m_ntall * 7;
        colorpalrect.InflateRect(1, 1, 1, 1);

        pDC->FrameRect(colorpalrect, &blackbrush);

        colorpalrect.DeflateRect(1, 1, 1, 1);

        //magmod.SetRect(colorpalrect.right-9,magrect.top, colorpalrect.right, magrect.top+9);

        pDC->FrameRect(magplus, &greybrush);
        pDC->MoveTo(magplus.left + 2, magplus.top + (magplus.bottom - magplus.top)/2);
        pDC->LineTo(magplus.right - 2,magplus.top +  (magplus.bottom - magplus.top)/2);

        pDC->MoveTo(magplus.left + (magplus.right - magplus.left)/2, magplus.top + 2);
        pDC->LineTo(magplus.left + (magplus.right - magplus.left)/2, magplus.bottom - 2);

        //pDC->LineTo(magplus.right - 2,magplus.top +  (magplus.bottom - magplus.top)/2);

        pDC->FrameRect(magminus, &greybrush);
        pDC->MoveTo(magminus.left + 2, magminus.top + (magminus.bottom - magminus.top)/2);
        pDC->LineTo(magminus.right - 2,magminus.top +  (magminus.bottom - magminus.top)/2);

        if ((m_Appflags & MAGWHILEEYEDROP)&&(m_isEyedropping)&&(!m_MagDrop)) {

            int pxwid = m_MagLevel;
            insiderect = magrect;
            insiderect.DeflateRect(magrect.Width()/2, magrect.Height()/2);
            insiderect.InflateRect(3,3); // mag 4
            insiderect.left+=4;
            insiderect.right+=4;
            insiderect.bottom+=4;
            insiderect.top+=4;


            if ((m_Appflags & Sampling3x3) ||
                (m_Appflags & Sampling5x5) ||
                ((m_Appflags & SamplingMULTI )&&(m_iSamplingOffset <= 10))
                ) {

                insiderect.InflateRect(4 *m_iSamplingOffset, 4*m_iSamplingOffset);
            }

            pDC->FrameRect(&insiderect, &blackbrush);
            // show the point that the eyedropper is on
        }
    }

    ReleaseDC(pDC);        // free memory
    return;
}

void CColorCopDlg::OnAbout()
{
    CAboutDlg dlg;
    dlg.DoModal();
}

void CColorCopDlg::OnChangeGreen()
{
    UpdateData(TRUE);
    if (m_Greendec > 255)
    {
        m_Greendec = 255;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();

}

void CColorCopDlg::OnChangeBlue()
{
    UpdateData(TRUE);
    if (m_Bluedec > 255)
    {
        m_Bluedec = 255;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeRed()
{
    UpdateData(TRUE);
    if (m_Reddec > 255) {
        m_Reddec = 255;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}
void CColorCopDlg::OnChangeBlack()
{
    UpdateData(TRUE);
    if (m_Black > 100) {
        m_Black = 100;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeCyan()
{
    UpdateData(TRUE);
    if (m_Cyan > 100) {
        m_Cyan = 100;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeMagenta()
{
    UpdateData(TRUE);
    if (m_Magenta > 100) {
        m_Magenta = 100;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeYellow()
{
    UpdateData(TRUE);
    if (m_Yellow > 100) {
        m_Yellow = 100;
    }
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}


void CColorCopDlg::OnColorPick()
{
    // set up the common windows color dialog
    COLORREF temp;
    CColorDialog dlgcolor;
    dlgcolor.m_cc.rgbResult = RGB(m_Reddec,m_Greendec,m_Bluedec);
    dlgcolor.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN;
    dlgcolor.m_cc.lpCustColors = CustColorBank;        // pointer to custom colors aray

    // show the dialog
    if (dlgcolor.DoModal() == IDOK)
    {
        // if they pressed OK
        temp=dlgcolor.GetColor();

        m_Reddec=GetRValue(temp);
        m_Greendec=GetGValue(temp);
        m_Bluedec=GetBValue(temp);

        UpdateData(false);
        CalcColorPal();
        OnconvertRGB();
        OnCopytoclip();

    }
    // else if they didn't hit OK, do nothing

    // just to be safe..
    if (m_isEyedropping || m_isMagnifying) {
        StopCapture();
        m_isEyedropping = m_isMagnifying = FALSE;
    }
}

void CColorCopDlg::OnCopytoclip()
{
    if (m_Appflags & AutoCopytoClip)
    {
        HWND hWndOwner = ::GetForegroundWindow();

        if (::OpenClipboard(hWndOwner))
        {
            ::EmptyClipboard();

            // CString is UTF‑16 in Unicode builds, so copy its raw buffer.
            const int len = m_Hexcolor.GetLength();
            const SIZE_T bytes = (len + 1) * sizeof(wchar_t);

            HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, bytes);
            if (hMem)
            {
                void* pMem = ::GlobalLock(hMem);
                if (pMem)
                {
                    memcpy(pMem, m_Hexcolor.GetString(), bytes);
                    ::GlobalUnlock(hMem);

                    // Correct modern clipboard format
                    ::SetClipboardData(CF_UNICODETEXT, hMem);
                } else {
                    ::GlobalFree(hMem);
                }
            }

            ::CloseClipboard();
        }
    }
}

void CColorCopDlg::StopCapture()
{
    // we don't want to capture anymore, they let up the left mouse button, or hit ESC
    m_isMagnifying = m_isEyedropping = FALSE;
    SetCursor(m_hStandardCursor);    // put standard cursor back on

    ReleaseCapture();                // let go of the mouse

    CWinApp* pApp = AfxGetApp();

    // put the icons back in their holders
    if (m_Appflags & USECROSSHAIR) {

        m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_MYCROSS));                        // location cursor

    } else {

        if (m_Appflags & Sampling5x5) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_5X5));
        } else if (m_Appflags & Sampling3x3) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_3X3));
        } else { // 1x1 or multi
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));                    // location cursor

        }
    }
    //m_EyeLoc.SetIcon(m_hEyeCursor);
    m_Magnifier.SetIcon(m_hMagCursor);

    bRelativePosition=false;
    // stop the timer
    KillTimer(1);

    return;
}

void CColorCopDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);
//
    CString strStatus = "";

    // the left mouse button was pressed, lets find out where

    CWnd* pWnd = ChildWindowFromPoint(point);
    if (pWnd && pWnd->GetSafeHwnd() == m_EyeLoc.GetSafeHwnd()) {

        // left mouse button down on the eyedropper
        //SetStatusBarText(_T("Drag this control around the screen"));
        m_isEyedropping = TRUE;
        m_InitialMove = TRUE;
        m_bCalcColorPal = TRUE; // ReCalculate Color Palette


        SetCapture();
        SetTimer(1,100,NULL);        // install the timer

        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Magnifier.GetSafeHwnd()) {
       //  left mouse button down on the magnifier

        SetCursor(m_hMagCursor);
        m_isMagnifying = true;
        SetCapture();
        SetTimer(1,100,NULL);        // install the timer

        m_Magnifier.SetIcon(m_hBlank);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagWindow.GetSafeHwnd()) {
        // left mouse button down on magnification window
        // we should get the color, because we can.
        // it's in the client window.


        if (m_Appflags & MAGWHILEEYEDROP)
        {
            m_MagDrop = true;
        }


        m_isEyedropping = TRUE;
        m_InitialMove = TRUE;    // Push color to color history
        m_bCalcColorPal = TRUE; // ReCalculate Color Palette
        SetCapture();
        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM (point.x, point.y));
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_ColorPalette.GetSafeHwnd()) {

        m_isEyedropping = TRUE;
        m_InitialMove = FALSE;    // Push color to color history
        m_bCalcColorPal = FALSE; // ReCalculate Color Palette
        SetCapture();
        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM (point.x, point.y));
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagPlus.GetSafeHwnd()) {

        if (m_MagLevel != 16) {
             m_MagLevel++;
        }

        strStatus.LoadString(IDS_MAG_INCREASED);
        strStatus.Format(strStatus, m_MagLevel);
        SetStatusBarText(strStatus);

        RecalcZoom();

        m_isMagPlusDown = TRUE;
        SetTimer(2,145,NULL);        // install the timer
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagMinus.GetSafeHwnd()) {

        if (m_MagLevel != 1) {
            m_MagLevel--;
        }

        strStatus.LoadString(IDS_MAG_DECREASED);
        strStatus.Format(strStatus, m_MagLevel);
        SetStatusBarText(strStatus);

        RecalcZoom();

        m_isMagMinusDown = TRUE;
        SetTimer(2,145,NULL);        // install the timer
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q1.GetSafeHwnd()) {

        GetHistoryColor(0);    // calls on copy clip
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q2.GetSafeHwnd()) {

        GetHistoryColor(1);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q3.GetSafeHwnd()) {

        GetHistoryColor(2);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q4.GetSafeHwnd()) {

        GetHistoryColor(3);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q5.GetSafeHwnd()) {

        GetHistoryColor(4);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q6.GetSafeHwnd()) {

        GetHistoryColor(5);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q7.GetSafeHwnd()) {

        GetHistoryColor(6);
        return;

    } else {  //  left mouse button down somewhere else in the app

        // To allow the user to drag the window by left clicking anywhere
        // in the app, we need to fake windows into thinking the user
        // has left clicked on the  the caption, Note: does not maximize on double click
        // from MSDN:
        //
        //        The WM_NCLBUTTONDOWN message is posted when the user presses
        //        the left mouse button while the cursor is within the nonclient
        //        area of a window.

        if (m_Appflags & EasyMove)    {    // one click move is enabled
            PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
            SetCursor(m_hMoveCursor);
        }
    }
    return;
}

void CColorCopDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    HWND CCopHWND=AfxGetApp()->GetMainWnd()->m_hWnd;

    CWnd* pWnd = ChildWindowFromPoint(point);

    if (pWnd && pWnd->GetSafeHwnd() == m_ColorPreview.GetSafeHwnd())
    {
        OnColorPick();    // user double clicked in the color preview control
                        // open custom color dialog
        return;
    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagWindow.GetSafeHwnd())
    {
        if (hBitmap)
        {
            CString strQuestion;
            strQuestion.LoadString(IDS_COPY_BITMAP);

            if (IDYES == AfxMessageBox(strQuestion, MB_YESNO))
            {
                if(::OpenClipboard(CCopHWND)) {

                    ::EmptyClipboard();

                    hBitmapClip = CopyBitmap(hBitmap);
                    SetClipboardData (CF_BITMAP, hBitmapClip);
                    ::CloseClipboard();
                    return;
                }
            }

        }
    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagPlus.GetSafeHwnd()) {

        // treat double click on mag plus like two clicks
        CColorCopDlg::OnLButtonDown(nFlags, point);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagMinus.GetSafeHwnd()) {

        // treat double click on mag minus like two clicks
        CColorCopDlg::OnLButtonDown(nFlags, point);
        return;

    } else {
        SetCursor(m_hStandardCursor);    // put standard cursor back on
        return;
    }
    CDialog::OnLButtonDblClk(nFlags, point);
}

HBITMAP CColorCopDlg::CopyBitmap (HBITMAP hBitmapSrc)
{
     BITMAP  bitmap;
     HBITMAP hBitmapDst;
     HDC     hdcSrc, hdcDst;

     ::GetObject (hBitmapSrc, sizeof (BITMAP), &bitmap);
     hBitmapDst = ::CreateBitmapIndirect (&bitmap);

     hdcSrc = ::CreateCompatibleDC(NULL);
     hdcDst = ::CreateCompatibleDC(NULL);

     ::SelectObject (hdcSrc, hBitmapSrc);
     ::SelectObject (hdcDst, hBitmapDst);

     ::BitBlt (hdcDst, 0, 0, bitmap.bmWidth*2, bitmap.bmHeight*2,
             hdcSrc, 0, 0, SRCCOPY);

     ::DeleteDC(hdcSrc);
     ::DeleteDC(hdcDst);

     return hBitmapDst;
}


void CColorCopDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // left mouse buttom was released
    // OK - no more eyedropping or magnifying because we don't own the cursor anymore..

    if ((m_isEyedropping) || (m_isMagnifying)) {

            OnCopytoclip();
            if (bRelativePosition) {
                // invert the old one back
                 bRelativePosition = FALSE;

                 // be nice and put the screen back the way it was
                 //BlockMeasure(RelativePoint, RelativePointEnd);


            }
         Invalidate(TRUE);

        StopCapture();
    }
    if (m_MagDrop) {
        m_MagDrop = false;
    }

    if (m_isMagMinusDown) {
        m_isMagMinusDown = FALSE;
        KillTimer(2);
    }

    if (m_isMagPlusDown) {
        m_isMagPlusDown = FALSE;
        KillTimer(2);
    }

    if (m_Appflags & EasyMove)    {
        // easy move is on so the cursor is probably wrong on the way up
        SetCursor(m_hStandardCursor);
    }
    CDialog::OnLButtonUp(nFlags, point);
}

/*
void CColorCopDlg::BlockMeasure(POINT ptBeg, POINT ptEnd) {

    HWND hwndScr = ::GetDesktopWindow ();
    HDC hdc = ::GetDCEx (hwndScr, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;

    // ClientToScreen (hwnd, &ptBeg) ;
    // ClientToScreen (hwnd, &ptEnd) ;
    ::PatBlt (hdc, ptBeg.x, ptBeg.y, ptEnd.x - ptBeg.x, ptEnd.y - ptBeg.y, DSTINVERT) ;
    ::ReleaseDC (hwndScr, hdc) ;
    return;

}
*/
void CColorCopDlg::OnMouseMove(UINT nFlags, CPoint point)
{
        bool bSkipColor = false;
        CString strStatus = "";


        //HWND CCopHWND=AfxGetApp()->GetMainWnd()->m_hWnd;
        if ((m_isEyedropping) || (m_isMagnifying)) {

            if (::GetCapture() == NULL) {

                //StopCapture();                                            // and do we have capture?
                //SetStatusBarText("Error, Lost Mouse Capture.");            // if not, stopcapturing..
                //m_isEyedropping = m_isMagnifying = FALSE;

                                // we used to abandon ship
                                // if we don't have capture, get it back
                                // hopefully this will stop all the emails

                SetCapture();

            } else  {

                //////////////////////////////////////
                //
                // We have capture and we are either eyedropping or magnifying

                ClientToScreen(&point); //use screen coordinates..



            }


        }// else {
        //
        //        CWnd* pWnd = ChildWindowFromPoint(point);
        //}

        if (m_isEyedropping) {

            if (m_InitialMove) {
                AdvanceColorHistory();
                m_InitialMove = FALSE;
            }

            COLORREF crefxy;
            hdc = ::GetDC(NULL);

            if (m_Appflags & Sampling1)    // only sample one pixel.
            {

                crefxy=::GetPixel(hdc, point.x, point.y);    // api call
                if (crefxy != CLR_INVALID) {

                    if (RGB(m_Reddec, m_Greendec, m_Bluedec) != crefxy) {
                        // current color is not the selected color
                        // it's ok to grab it

                        m_Reddec   = GetRValue(crefxy);
                        m_Greendec = GetGValue(crefxy);
                        m_Bluedec  = GetBValue(crefxy);


                        UpdateCMYKFromRGB(m_Reddec, m_Greendec, m_Bluedec);

    //Black   = minimum(1-Red,1-Green,1-Blue)
    //Cyan    = (1-Red-Black)/(1-Black)
    //Magenta = (1-Green-Black)/(1-Black)
    //Yellow  = (1-Blue-Black)/(1-Black)

                    } else {
                        bSkipColor = true;
                    }
                }
            } else {
                // the user wants to sample a 3x3 or a 5x5 average of pixels.
                bSkipColor = AveragePixelArea(hdc, &m_Reddec, &m_Greendec, &m_Bluedec, point);
                UpdateCMYKFromRGB(m_Reddec, m_Greendec, m_Bluedec);

            }

            if (bRelativePosition) {
                // store the new relative point.

//                BlockMeasure(RelativePoint, RelativePointEnd);
                RelativePointEnd.x = point.x;
                RelativePointEnd.y = point.y;
//                BlockMeasure(RelativePoint, RelativePointEnd);


            }

            ::ReleaseDC (::GetForegroundWindow(), hdc);    // free up the memory

            CString strStatus="",strWebSafe = "";


                if (bRelativePosition)  {

                    // L (Length line = hypotenuse) = SQRT(W² + H²) (show it to 1 decimal)
                    int iWidth = (point.x - RelativePoint.x);
                    int iHeight = (point.y - RelativePoint.y);

                    // we need to add one since they are zero based
                    double dWidth = (double)iWidth+1;
                    double dHeight = (double)iHeight+1;

                    double dLength = sqrt(dWidth*dWidth + dHeight * dHeight);
                    //If W=0 then A=90 else A=ATAN(H/W)*(180/PI())
                    double dAngle = 0.0;

                    if (iWidth == 0) {
                        // prevent the divide by zero
                        dAngle =90;

                    } else {
                        dAngle=atan(dHeight/dWidth)*((double)180/(double)PI);
                    }



                    strStatus.LoadString(IDS_RELATIVE_POS);
                    strStatus.Format(strStatus, iWidth,
                                                iHeight,
                                                dLength,
                                                dAngle
                                                );

                } else     if ((m_Appflags & Sampling3x3)||
                            (m_Appflags & Sampling5x5)||
                            (m_Appflags & SamplingMULTI)) {

                    strStatus.LoadString(IDS_EYEDROPPING);
                    strStatus += ", %dx%d";
                    strStatus.Format(strStatus, point.x, point.y, m_iSamplingOffset*2+1, m_iSamplingOffset*2+1);

                } else { // SampleRate == 1

                    strStatus.LoadString(IDS_EYEDROPPING);
                    if (m_Appflags & DetectWebsafeColors) {
                        if (isWebsafeColor(m_Reddec, m_Greendec, m_Bluedec)) {
                            strWebSafe.LoadString(IDS_WEBSAFE);
                        } else {
                            strWebSafe.LoadString(IDS_NOT_WEBSAFE);
                        }
                    }
                    strStatus +=strWebSafe;

                    strStatus.Format(CString(strStatus), point.x, point.y);

                }


                if (m_Appflags & MAGWHILEEYEDROP) {

                    if (!m_MagDrop) {
                        m_MagLevel = 4;
                        // default mag level to 4x
                        GetScreenBitmap(point);
                        InvalidateRect(&magrect, FALSE);
                    }
                }
                SetStatusBarText(strStatus);

                //}

                if (!bSkipColor) {
                    OnconvertRGB();
                }
        // no copy to clip on move.  it will copy on mouse up
        return;

    } else if (m_isMagnifying) {        // or are we magnifiying??

        strStatus.LoadString(IDS_MAGNIFYING);
        strStatus.Format(strStatus, point.x, point.y, m_MagLevel);
        SetStatusBarText(strStatus);

        GetScreenBitmap(point);
        InvalidateRect(magrect, FALSE);
        // go redraw... but don't erase or it will flicker
        return;
    } else
    {
        // not magnifying oreyedropping

        CWnd* pWnd = ChildWindowFromPoint(point);
        if (pWnd && pWnd->GetSafeHwnd() == m_EyeLoc.GetSafeHwnd()) {

            SetCursor(m_hHandCursor);

        } else if (pWnd && pWnd->GetSafeHwnd() == m_Magnifier.GetSafeHwnd()) {
            //  left mouse button down on the magnifier

            SetCursor(m_hHandCursor);


        } else if (pWnd && pWnd->GetSafeHwnd() == m_MagWindow.GetSafeHwnd()) {
        // left mouse button down on magnification window
        // we should get the color, because we can.
        // it's in the client window.


        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hEyeCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_ColorPalette.GetSafeHwnd()) {

        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hEyeCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagPlus.GetSafeHwnd()) {

            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagMinus.GetSafeHwnd()) {

            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q1.GetSafeHwnd()) {

        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q2.GetSafeHwnd()) {

        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q3.GetSafeHwnd()) {

        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q4.GetSafeHwnd()) {

        //    m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q5.GetSafeHwnd()) {

    //        m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q6.GetSafeHwnd()) {

    //        m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q7.GetSafeHwnd()) {

    //        m_EyeLoc.SetIcon(m_hBlank);
            SetCursor(m_hHandCursor);

    }

    }

    CDialog::OnMouseMove(nFlags, point);
}

void CColorCopDlg::GetScreenBitmap(CPoint point)
{
    if (hBitmap)    // delete the old bitmap, right before we get a new one
    {
        ::DeleteObject(hBitmap);
        hBitmap = NULL;
    }
    if (hZoomBitmap)
    {
        ::DeleteObject(hZoomBitmap);
        hZoomBitmap = NULL;
    }
            hdc = ::GetDC(NULL);        // dEVICEcONEXT to the whole desktop

    hdcMem = ::CreateCompatibleDC(hdc);
    hdcZoomMem = ::CreateCompatibleDC(hdc);

    hBitmap = CreateCompatibleBitmap(hdc, magrect.Width(), magrect.Height());
    hZoomBitmap = CreateCompatibleBitmap(hdc, magrect.Width(), magrect.Height());

    ::SelectObject(hdcMem, hBitmap);
    ::SelectObject(hdcZoomMem, hZoomBitmap);
    ::SetStretchBltMode(hdc, COLORONCOLOR);


    int magwidth = 19;        // default heights
    int magheight = 15;



    if (m_MagLevel <= 0)
        m_MagLevel = 1;
    else if (m_MagLevel >= 17)
        m_MagLevel = 16;
    magwidth = magrect.Width() / m_MagLevel;
    magheight = magrect.Height() / m_MagLevel;



    ::BitBlt (hdcZoomMem,            // destination DC
              0, 0,                // destination upper left (always 0,0)
              magrect.Width(), magrect.Height(),    // w x h of destination
              hdc,                // source DC
              point.x - (magrect.Width() / 2), // x coordinate of source
              point.y - (magrect.Height() / 2), // y coordinate of source
                 SRCCOPY);            // raster mode

    ::StretchBlt (hdcMem,            // destination DC
               0, 0,                // destination upper left (always 0,0)
              magrect.Width(), magrect.Height(),    // w x h of destination
              hdc,                // source DC
              point.x - (magwidth / 2), // x coordinate of source
              point.y - (magheight / 2), // y coordinate of source
              magwidth, magheight,            // width of source
              SRCCOPY);            // raster mode

    //      magrect.Width()/2, magrect.Height()/2,    // w x h of destination

    ::DeleteDC(hdcMem);
    ::DeleteDC(hdcZoomMem);
    ::ReleaseDC(::GetForegroundWindow(), hdc);        // free up memory
    return;

}


void CColorCopDlg::GetHistoryColor(int Cindex)
{
    // this function sets the current color
    // to the history color which was clicked on

    if (RGB(m_Reddec,m_Greendec,m_Bluedec) != ColorHistory[Cindex]) {
        // the color is different.  get it

        m_Reddec   = GetRValue(ColorHistory[Cindex]);
        m_Greendec = GetGValue(ColorHistory[Cindex]);
        m_Bluedec  = GetBValue(ColorHistory[Cindex]);

        UpdateData(false);    // update vars
        OnconvertRGB();
        CalcColorPal();
        OnCopytoclip();
    }

}

void CColorCopDlg::AdvanceColorHistory()
{
    // this function will advance the colors in the color history..
    // the 7th color is lost, the first color is the current color
    // which is soon to be the older color.

    // if the new color is the same as the last color, don't switch.
    if (ColorHistory[0] ==  (COLORREF) RGB(m_Reddec, m_Greendec, m_Bluedec))
        return;

    for (int sk = 6; sk > 0; sk--)        // swap
    {
        ColorHistory[sk] = ColorHistory[sk-1];
    }

    ColorHistory[0] = RGB(m_Reddec, m_Greendec, m_Bluedec);
}

void CColorCopDlg::FloatPrecisionUp() {
    if (m_Appflags & RGBFLOAT) {
        if (m_FloatPrecision < 6)
            m_FloatPrecision++;
        SetStatusBarText(IDS_FLOATUP, 0);
        OnconvertRGB();
        OnCopytoclip();

    }
    return;
}
void CColorCopDlg::FloatPrecisionDown() {
    if (m_Appflags & RGBFLOAT) {
        if (m_FloatPrecision > 1)
            m_FloatPrecision--;
        SetStatusBarText(IDS_FLOATDOWN, 0);
        OnconvertRGB();
        OnCopytoclip();

    }
    return;
}

BOOL CColorCopDlg::PreTranslateMessage(MSG* pMsg)
{
    //HCURSOR hCurs1, hCurs2;    // cursor handles
    POINT pt;                  // cursor location
    //RECT rc;                   // client area coordinates
    static int repeat = 1;     // repeat key counter

    // pass a mouse message to the tool tip control for processing
    m_ToolTip.RelayEvent(pMsg);

    if (m_hAcceleratorTable) {        // use the Accelerator resource

        if(::TranslateAccelerator(m_hWnd, m_hAcceleratorTable, pMsg)) {
            return TRUE;    // escape
        }
    }

    // test to see if F1 is being pressed to bring up help
    if (pMsg->message == 0x4d) {
        if (GetKeyState(VK_SHIFT) >= 0)    {
            ::HtmlHelp(NULL, _T("ColorCop.chm"), HH_DISPLAY_TOPIC, 0);
            return TRUE;
        }
    }

    /************************************************
    * ESC key Check
    * - hitting the escape key, should only stop the
    *   eyedropper or magnifing glass and should not
    *   exit the app
    ************************************************/

    if (pMsg->message == WM_KEYDOWN)
        if (pMsg->wParam == VK_ESCAPE) {
            if (m_isEyedropping || m_isMagnifying)
            {
                // dropper or magnifier in use
                // This stops people from getting smart and executing
                // keyboard shortcuts while eyedropping or magnifying
                StopCapture();
                m_isEyedropping = m_isMagnifying = FALSE;

            }
            if (bRelativePosition) {
                // invert the old one back
                 bRelativePosition = FALSE;
            }
            return TRUE;    // return control rather than letting it
                            // exit the app
        } else if (m_isEyedropping || m_isMagnifying) {

              CString strStatus;
            CWinApp* pApp;
            switch(pMsg->wParam) {
            case VK_CONTROL:
                //repeat=5;
                if (m_isEyedropping && GetCursorPos(&RelativePoint)) {
                    RelativePointEnd = RelativePoint;
                    strStatus.LoadString(IDS_RELATIVE_POS);
                    strStatus.Format(strStatus, 0, 0,0,90);
                    SetStatusBarText(strStatus);

                    if (!(m_Appflags & USECROSSHAIR)) {
                        // set the cross hair cursor if they app isn't already using it
                        pApp = AfxGetApp();
                        m_hEyeCursor = pApp->LoadCursor(IDC_MYCROSS);
                        SetCursor(m_hEyeCursor);
                    }
                    bRelativePosition = TRUE;        // Start Relative
                }
                break;

            case VK_RIGHT:                // right arrow
                if (GetCursorPos(&pt))
                {
                    SetCursorPos(pt.x+repeat, pt.y);
                }
                break;
            case VK_LEFT:              // left arrow
                if (GetCursorPos(&pt))
                {
                    SetCursorPos(pt.x-repeat, pt.y);
                }
                break;
            case VK_UP:                    // up arrow
                if (GetCursorPos(&pt))
                {
                    SetCursorPos(pt.x, pt.y-repeat);
                }
                break;
            case VK_DOWN:              // down arrow
                if (GetCursorPos(&pt))
                {
                    SetCursorPos(pt.x, pt.y+repeat);
                }
                break;

            }
            //if (!bRelativePosition)
        }

    return CDialog::PreTranslateMessage(pMsg);
}



void CColorCopDlg::OnChangeHexcolor()
{
    // the user is typing in the hex edit control
    // or they pasted a hex code.
    // OK - parse out the RGB values and convert

    int hexrange,offset;

    UpdateData(1);    // save from control to m_Hexcolor
    int hexsize=0;
    hexsize=m_Hexcolor.GetLength();

    if (m_Appflags & ModeHTML)
    {
        if (m_Hexcolor.Left(1) =='#')
        {
            hexrange=5;
            offset=1;
        } else {
            hexrange=6;
             offset=0;
        }

        while (hexsize>hexrange)
        {
            m_Hexcolor.Delete(6+offset);
            hexsize--;
        }
        m_Reddec=m_Greendec=m_Bluedec=0;
        ParseHTML(m_Hexcolor);

    } else if (m_Appflags & ModeClarion)
    {
                // Clarion Starts with 0s
        if (m_Hexcolor.Left(1) =='0')
        {
            hexrange=7;
            offset=1;
            ParseClarion(m_Hexcolor);
        }

    } else //Delphi hex change
    {
        if (m_Hexcolor.Left(1) =='$')
        {
            hexrange=7;
            offset=1;
        } else
        {
            hexrange=8;
             offset=0;
        }

        while (hexsize>hexrange)
        {
            m_Hexcolor.Delete(8+offset);
            hexsize--;
        }
        m_Reddec=m_Greendec=m_Bluedec=0;
        ParseDelphi(m_Hexcolor);
    }
}



void CColorCopDlg::ParseDelphi(CString inst)
{
    // they are typing in delphi hex codes
    // determine the RGB values

    if (inst.GetLength() < 3)
    {
        m_Reddec=m_Greendec=m_Bluedec=0;
        UpdateData(false);
        return;
    }

    if (inst.Left(1)=='$')
        inst.Delete(0);
    inst.Delete(0);
    inst.Delete(0);
    m_Bluedec = strtoul( inst.Left(2), NULL, 16);
    inst.Delete(0);
    inst.Delete(0);
    m_Greendec = strtoul( inst.Left(2), NULL, 16);
    inst.Delete(0);
    inst.Delete(0);
    m_Reddec = strtoul( inst.Left(2), NULL, 16);

    UpdateData(0);

    inst.FreeExtra();
    OnconvertHEX();
    return;
}


void CColorCopDlg::ParseClarion(CString inst)
{
    // they are typing in clarion hex codes
    // determine the RGB values

    if (inst.GetLength() < 2)
    {
        m_Reddec=m_Greendec=m_Bluedec=0;
        UpdateData(false);
        return;
    }

    if (inst.Left(1)=='0')
        inst.Delete(0);
    m_Bluedec = strtoul( inst.Left(2), NULL, 16);
    inst.Delete(0);
    inst.Delete(0);
    m_Greendec = strtoul( inst.Left(2), NULL, 16);
    inst.Delete(0);
    inst.Delete(0);
    m_Reddec = strtoul( inst.Left(2), NULL, 16);

    UpdateData(0);

    inst.FreeExtra();
    OnconvertHEX();
    return;
}


void CColorCopDlg::ParseHTML(CString inst)
{
    // the user is typing in HTML hex codes
    // get the RGB values

    if (inst.Left(1) == '#')
        inst.Delete(0);

    m_Reddec = strtoul( inst.Left(2), NULL, 16);

    inst.Delete(0);
    inst.Delete(0);
    m_Greendec = strtoul( inst.Left(2), NULL, 16);
    inst.Delete(0);
    inst.Delete(0);
    m_Bluedec = strtoul( inst.Left(2), NULL, 16);
    UpdateData(0);

    OnconvertHEX();
    return;
}

BOOL CColorCopDlg::GetShellFolderPath(char* pShellFolder, char* pShellPath)
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

CString CColorCopDlg::GetTempFolder()
{
    CString strTmpPath;
//    DWORD dwL;

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

void CColorCopDlg::OnDestroy()
{
    //
    // The app is about to close, save the variables


    // save the location
    tagRECT *winSize = new tagRECT;
    GetWindowRect(winSize);
    WinLocX = winSize->left;        // Store x,y
    WinLocY = winSize->top;        // in variables


    // save the bitmap
//    TCHAR szPath[MAX_PATH];

    CString strBMPFile = GetTempFolder();

//    strBMPFile.Append(BMP_FILE);

    strBMPFile +=BMP_FILE_DIR;

    strBMPFile +=BMP_FILE;

    HWND curwindowhwnd = ::GetForegroundWindow();
/*
    //HBITMAP hBitmap, hBitmapClip, hZoomBitmap;
    if(0!=GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath)))
    {
        LPTSTR BmpName = _tcsrchr(szPath, '\\');
        if(!BmpName)
            BmpName = szPath;
        //_tcscpy_s(BmpName,MAX_PATH,"\\ColorCop5.bmp");
        _tcscpy(BmpName,"\\ColorCop5.bmp");
    }
*/
    if (hBitmap) {
        PBITMAPINFO MagBmpInfo = CreateBitmapInfoStruct( curwindowhwnd,hBitmap);
//        CreateBMPFile(curwindowhwnd, (LPTSTR)strBMPFile.Tostring ,MagBmpInfo,hBitmap,::GetDC(NULL));

        // working
        //CreateBMPFile(curwindowhwnd, szPath ,MagBmpInfo,hBitmap,::GetDC(NULL));

        CreateBMPFile(curwindowhwnd, strBMPFile.GetBuffer(MAX_PATH) ,MagBmpInfo,hBitmap,::GetDC(NULL));

        strBMPFile.ReleaseBuffer();


    }

    CDialog::OnDestroy();
    return;
}

void CColorCopDlg::OnFileExit()
{
    EndDialog(IDOK);
}

void CColorCopDlg::OnOptionsAlwaysontop()
{
    m_Appflags ^= AlwaysOnTop;
    ToggleOnTop(true);
}


void CColorCopDlg::ToggleOnTop(bool bSetStatusbartext)
{
    if (m_Appflags & AlwaysOnTop) // Make Always on Top
    {
        if (bSetStatusbartext)
        {
            SetStatusBarText(IDS_ALWAYSOTOP, 1);
        }
        //mfc call
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);
    } else        // Not always on top, NORMAL
    {
        if (bSetStatusbartext)
        {
        SetStatusBarText(IDS_ALWAYSOTOP, 2);
        }
        //win32 api call
        ::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    // Make sure the checkbox for always on top on the system menu
    // is the same as the checkbox in the dialog menu
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        if (m_Appflags & AlwaysOnTop)
            pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_CHECKED);        // check the menu item
        else
            pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_UNCHECKED);        // uncheck the item
    }


}


void CColorCopDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
    // this function is called when the top level
    // menu items are selected.
    UpdateMenu(pMenu);
}

void CColorCopDlg::UpdateMenu(CMenu* pMenu)
{
    // this function loops through each menu item and updates the checkboxes and radio buttons

    CCmdUI cmdUI;
    for (UINT n = 0; n < pMenu->GetMenuItemCount(); ++n)
    {
        CMenu* pSubMenu = pMenu->GetSubMenu(n);
        if (pSubMenu == NULL)
        {
            cmdUI.m_nIndexMax = pMenu->GetMenuItemCount();
            for (UINT i = 0; i < cmdUI.m_nIndexMax; ++i)
            {
                cmdUI.m_nIndex = i;
                cmdUI.m_nID = pMenu->GetMenuItemID(i);
                cmdUI.m_pMenu = pMenu;
                cmdUI.DoUpdate(this, FALSE);
            }
        }
    }
}


void CColorCopDlg::OnUpdateOptionsAutocopytoclipboard(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & AutoCopytoClip);
}

void CColorCopDlg::OnUpdateOptionsAlwaysontop(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & AlwaysOnTop);
}

void CColorCopDlg::OnOptionsAutocopytoclipboard()
{
    m_Appflags ^= AutoCopytoClip;
    OnCopytoclip();
}

void CColorCopDlg::OnColorRandom()
{
    // Generates a random color and updates
    // current decimal value MOD 256 - make a random value from 0 to 255
    // Thread-safe random number generation
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(RGB_MIN, RGB_MAX);

    m_Reddec   = distribution(generator);
    m_Greendec = distribution(generator);
    m_Bluedec  = distribution(generator);

    SetStatusBarText(IDS_RANDOMCOLOR, 0);
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnColorReverse()
{
    // Reverse the current colors.
    // ABS (current decimal value - 255)  then update

    SetStatusBarText(IDS_REVERSECOLOR,0);
    m_Reddec   = abs(m_Reddec - 255);
    m_Greendec = abs(m_Greendec - 255);
    m_Bluedec  = abs(m_Bluedec  - 255);
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}


void CColorCopDlg::OnPopupColorConverttograyscale()
{
    uint16_t L = 0, Min = 0, Max = 0;

    CString strStatus;

    // Converts the current color to grayscale
    if ((m_Reddec == m_Greendec) && (m_Greendec == m_Bluedec)) {

        strStatus.LoadString(IDS_COLOR_GRAY);
        SetStatusBarText(strStatus);
    } else {

        strStatus.LoadString(IDS_COLOR_CONVERT_GRAY);
        SetStatusBarText(strStatus);
        // grayscale values have identical R, G, B values.
        //
        // Actually, it uses an approximation to this idea;
        // it chooses the point on the neutral axis determined by computing , which corresponds
        // to the definition of lightness. After applying Desaturate, the image remains in RGB space
        // and continues to have three color channels, but now the channels have identical values,
        // which is why the image appears as a grayscale.

        // formula: L = {Max(R,G,B) + Min(R,G,B)}/2;
        Max = __max(m_Reddec,m_Greendec);
        Max = __max(Max,m_Bluedec);

        Min = __min(m_Reddec,m_Greendec);
        Min = __min(Min,m_Bluedec);
        L = (int) (Min + Max) / 2;

        m_Reddec   = L;
        m_Greendec = L;
        m_Bluedec  = L;
        UpdateData(false);
        OnconvertRGB();
        OnCopytoclip();
    }
    return;
}

void CColorCopDlg::OnFileAbout()
{
    CAboutDlg dlg;
    dlg.DoModal();
}

void CColorCopDlg::OnColorSnaptowebsafe()
{
    m_Appflags ^= SnaptoWebsafe;


    if (m_Appflags & SnaptoWebsafe) {
        m_Appflags &= ~DetectWebsafeColors; // shut off Websafe Detection
        SetStatusBarText(IDS_SNAPTOWEBSAFE, 1);
    } else {
        SetStatusBarText(IDS_SNAPTOWEBSAFE, 2);
    }

    if ((m_Appflags ^ SnaptoWebsafe) && (bOldClrExist))
    {
        // the user switched snap to websafe off, which means it was previously on
        // revert back to the saved colors
        m_Reddec   = m_OldRed;
        m_Greendec = m_OldGreen;
        m_Bluedec  = m_OldBlue;

    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnUpdateColorSnaptowebsafe(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & SnaptoWebsafe);
}

void CColorCopDlg::TestForWebsafe()
{
    // websafe colors are multiples of 51
    if (m_Appflags & SnaptoWebsafe)
    {
        bOldClrExist = true;    // let the other function know we have an old color
        m_OldRed   = m_Reddec;    // backup the actual values incase they unsnap
        m_OldGreen = m_Greendec;
        m_OldBlue  = m_Bluedec;

        m_Reddec   = DecimaltoWebsafe(m_Reddec);
        m_Greendec = DecimaltoWebsafe(m_Greendec);
        m_Bluedec  = DecimaltoWebsafe(m_Bluedec);
    } //else  do nothing
}

int CColorCopDlg::DecimaltoWebsafe(int originalDec)
{
    // this function takes an int and converts
    // it to the closest web safe int
    //
    int offset = originalDec % WEBSAFE_STEP;

    if (offset == 0) {
        // already a web safe int
        return (originalDec);
    } else if (offset < 25) {
        // go down one
        return (originalDec - offset);
    } else {
        // jump up one
        return (originalDec + (WEBSAFE_STEP - offset));
    }
}

void CColorCopDlg::OnOptionsOmitsymbol()
{
    m_Appflags ^= OmitPound;
    if (m_Appflags & OmitPound)
        SetStatusBarText(IDS_OMITSYMBOL, 1);
    else
        SetStatusBarText(IDS_OMITSYMBOL, 2);

    FigurePound();
}

void CColorCopDlg::FigurePound()
{
    // This function adds or removes characters from the hex edit control

    if (m_Appflags & OmitPound)
    {
        if (m_Appflags & ModeHTML) {
            if (m_Hexcolor.Left(1) == '#')            // remove the # is it exists
                m_Hexcolor.Delete(0);
        } else if (m_Appflags & ModeDelphi) {
            if (m_Hexcolor.Left(1) == '$')
                m_Hexcolor.Delete(0);
        }
    } else
    {
        if (m_Appflags & ModeHTML) {
            if (m_Hexcolor.Left(1) != '#')
                m_Hexcolor = "#" + m_Hexcolor;        // add the # if it exsits
        } else if (m_Appflags & ModeDelphi) {
            if (m_Hexcolor.Left(1) != '$')
                m_Hexcolor = "$" + m_Hexcolor;
        }
    }
    UpdateData(false);
    // update changes to the edit control (m_Hexcolor)
}

void CColorCopDlg::OnUpdateOptionsOmitsymbol(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck((m_Appflags & OmitPound) ? 1 : 0);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // setup the hyperlinks

    m_link.SetLink(TRUE)
        .SetTextColor(RGB(0,0,255))
        .SetFontUnderline(TRUE)
        .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDPOINTER));

    m_maillink.SetLink(TRUE)
        .SetTextColor(RGB(0,0,255))
        .SetFontUnderline(TRUE)
        .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HANDPOINTER));

    return TRUE;
}

void CColorCopDlg::OnExpandDialog()
{
    m_Appflags ^= ExpandedDialog;

    if (m_Appflags & ExpandedDialog) {
        SetStatusBarText(IDS_EXPANDEDDIALOG, 1);
    }
    // No need to set it as off because the user can't see it anyway
    /*else {
        SetStatusBarText(IDS_EXPANDEDDIALOG, 2);
    }*/

    TestForExpand();
}

void CColorCopDlg::TestForExpand()
{
    RECT currect;
    GetWindowRect(&currect);

    // usr small for both
    //    smWidth = lgWidth;
    //    smHeight = lgHeight;

    if (m_Appflags & ExpandedDialog) {
        currect.right = currect.left + lgWidth;
        currect.bottom = currect.top + lgHeight;

        m_ExpandDialog.SetWindowText( _T("&<<") );

        m_Magnifier.ShowWindow(TRUE);

        MoveWindow(&currect);

    } else {
        // use smWidth and smHeight
        m_Magnifier.ShowWindow(FALSE);
        currect.right  = currect.left + smWidth;
        currect.bottom = currect.top + smHeight;
        MoveWindow(&currect);

        m_ExpandDialog.SetWindowText(_T("&>>"));
    }
    InvalidateRect(&currect, false); // go redraw... but don't erase or it will flicker
}

void CColorCopDlg::OnOptionsMinimizetosystray()
{
    m_Appflags ^= MimimizetoTray;

    if (m_Appflags & MimimizetoTray)
        SetStatusBarText(IDS_MINIMIZETOTRAY, 1);
    else
        SetStatusBarText(IDS_MINIMIZETOTRAY, 2);
    return;
}

void CColorCopDlg::OnUpdateOptionsMinimizetosystray(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & MimimizetoTray);
}

void CColorCopDlg::OnOptionsUppercasehex()
{
    m_Appflags ^= UpperCaseHex;

    if (m_Appflags & UpperCaseHex)
        SetStatusBarText(IDS_UPPERHEX, 1);
    else
        SetStatusBarText(IDS_UPPERHEX, 2);

    TestForUpperHex();
}

void CColorCopDlg::OnUpdateOptionsUppercasehex(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & UpperCaseHex);
}

void CColorCopDlg::TestForUpperHex()
{
    // fixes the current hex value to the correct case

    if (m_Appflags & UpperCaseHex)
    {
        // make currenthex uppercase
        m_Hexcolor.Replace('a','A');
        m_Hexcolor.Replace('b','B');
        m_Hexcolor.Replace('c','C');
        m_Hexcolor.Replace('d','D');
        m_Hexcolor.Replace('e','E');
        m_Hexcolor.Replace('f','F');
    } else {
        m_Hexcolor.Replace('A','a');
        m_Hexcolor.Replace('B','b');
        m_Hexcolor.Replace('C','c');
        m_Hexcolor.Replace('D','d');
        m_Hexcolor.Replace('E','e');
        m_Hexcolor.Replace('F','f');
    }
    UpdateData(false);    // update control
}

void CColorCopDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    CWnd* pWnd = ChildWindowFromPoint(point);

    if (m_isMagnifying)    {            // right clicked while magnifying
        if (m_MagLevel != 16) {
            m_MagLevel++;
        }
        return;
    } else if (m_isEyedropping)    {    // right clicked while eydropping
        AdvanceColorHistory();        // push color to color history
        return;
    } else if (pWnd && pWnd->GetSafeHwnd() == m_ColorPalette.GetSafeHwnd())    {
        AdvanceColorHistory();        // push color to color history
        return;
    }



    CMenu tempMenu;
    tempMenu.LoadMenu(IDR_COPMENU);

    ClientToScreen(&point);
    CMenu *pPopup = tempMenu.GetSubMenu(0);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN    | TPM_RIGHTBUTTON,
                           point.x, point.y,
                           this, NULL);


    CDialog::OnRButtonDown(nFlags, point);
}

void CColorCopDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // right mouse button came up, watch out for capture
    CDialog::OnRButtonUp(nFlags, point);
}

void CColorCopDlg::ChangeTo1pixelSampling()
{
    if (m_Appflags ^ Sampling1) {
        m_Appflags |= Sampling1;
        m_Appflags &= ~Sampling3x3;
        m_Appflags &= ~Sampling5x5;
        m_Appflags &= ~SamplingMULTI;
    }
    SetStatusBarText(IDS_1PIXEL, 0);

    CWinApp* pApp = AfxGetApp();
    m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));
    return;
}

void CColorCopDlg::ChangeTo3x3Sampling()
{
    if (m_Appflags ^ Sampling3x3) {
        m_Appflags &= ~Sampling1;
        m_Appflags |= Sampling3x3;
        m_Appflags &= ~Sampling5x5;
        m_Appflags &= ~SamplingMULTI;
    }
    m_iSamplingOffset=1;
    SetStatusBarText(IDS_3PIXEL, 0);

    CWinApp* pApp = AfxGetApp();
    m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_3X3));
    return;
}

void CColorCopDlg::ChangeTo5x5Sampling()
{
    if (m_Appflags ^ Sampling5x5) {
        m_Appflags &= ~Sampling1;
        m_Appflags &= ~Sampling3x3;
        m_Appflags |= Sampling5x5;
        m_Appflags &= ~SamplingMULTI;
    }
    m_iSamplingOffset=2;
    SetStatusBarText(IDS_5PIXEL, 0);

    CWinApp* pApp = AfxGetApp();
    m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_5X5));
    return;
}

void CColorCopDlg::OnUpdatePopupSampling1pixel(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & Sampling1);
}

void CColorCopDlg::OnUpdatePopupSampling5by5average(CCmdUI* pCmdUI)
{

    pCmdUI->SetRadio(m_Appflags & Sampling5x5);
}

void CColorCopDlg::OnUpdatePopupSampling3by3average(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & Sampling3x3);
}

bool CColorCopDlg::AveragePixelArea(HDC hdc, int* m_R, int* m_G, int* m_B, CPoint point)
{
    // this function averages a matrix of pixels.
    // either a 3 by 3 or a 5 by 5 average of pixels.

    // we want to modify this so it can be any range.
    // to do this lets store m_iSampleOffset;



    int reddec = 0, greendec = 0, bluedec = 0;        // temp variables to add up the values
    int offset = 0, elements = 0, xrel, yrel;
    COLORREF crefxy;


    /*if (m_Appflags & Sampling3x3)
    {
        offset = 1;
        elements = 9;        // 3x3 - add up and divide by 9

    } else { // (m_Appflags & Sampling5x5)

        offset = 2;
        elements = 25;        // 5x5 - add up and divide by 25
    }*/

    offset = m_iSamplingOffset;
    elements = (m_iSamplingOffset*2+1)*(m_iSamplingOffset*2+1);

    for (xrel = point.x - offset; xrel <= point.x + offset; xrel++) {
        for (yrel = point.y - offset; yrel <= point.y + offset; yrel++) {
            crefxy=::GetPixel(hdc, xrel, yrel);
            reddec += GetRValue(crefxy);
            greendec += GetGValue(crefxy);
            bluedec += GetBValue(crefxy);
        }
    }
    reddec = (int) reddec / elements; // average
    greendec = (int) greendec / elements; // average
    bluedec = (int) bluedec / elements; // average

    if (RGB(reddec,greendec,bluedec) != RGB(m_Reddec, m_Greendec, m_Bluedec)) {

        *m_R = reddec;
        *m_G = greendec;
        *m_B = bluedec;

        return false;    // different.  don't skip

    } else {
        return true;    // color is the same
    }
}

void CColorCopDlg::OnPopupApplicationExpandeddialog()
{
    OnExpandDialog();
}

void CColorCopDlg::OnUpdatePopupApplicationExpandeddialog(CCmdUI* pCmdUI)
{
        pCmdUI->SetCheck(m_Appflags & ExpandedDialog);
}

void CColorCopDlg::OnUpdateViewHtmlhexmode(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModeHTML);
}
void CColorCopDlg::OnUpdateOptionsDelphimode(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModeDelphi);
}
void CColorCopDlg::OnUpdatePopupHexmodePowerbuilder(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModePowerBuilder);
}
void CColorCopDlg::OnUpdatePopupModeVisualbasichex(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModeVisualBasic);
}
void CColorCopDlg::OnUpdatePopupModeVisualchex(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModeVisualC);
}
void CColorCopDlg::OnUpdatePopupModeRgbfloat(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & RGBFLOAT);
}
void CColorCopDlg::OnUpdatePopupModeRgbint(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & RGBINT);
}

void CColorCopDlg::OnUpdatePopupModeClarionhex(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & ModeClarion);
}

void CColorCopDlg::OnPopupModeRgbfloat()
{
    SetStatusBarText(IDS_MODE_RGBFLOAT, 1);
    if (m_Appflags ^ ModeHTML) {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags |= RGBFLOAT;    // rgb float
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;
    }
    OnconvertRGB();
    OnCopytoclip();


}


void CColorCopDlg::OnPopupModeRgbint()
{
    SetStatusBarText(IDS_MODE_RGBINT, 1);
    if (m_Appflags ^ ModeHTML)
    {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags |= RGBINT;    // rgb int
        m_Appflags &= ~ModeClarion;

    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnViewHtmlhexmode()
{
    SetStatusBarText(IDS_MODE_HTML, 1);
    if (m_Appflags ^ ModeHTML)
    {
        m_Appflags |= ModeHTML;            // HTML ON
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;

    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnOptionsDelphimode()
{
    SetStatusBarText(IDS_MODE_DELPHI, 1);
    if (m_Appflags ^ ModeDelphi) {
        m_Appflags &= ~ModeHTML;
        m_Appflags |= ModeDelphi;        // Delphi ON
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;
    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupHexmodePowerbuilder()
{
    SetStatusBarText(IDS_MODE_POWERBUILDER, 1);
    if (m_Appflags ^ ModePowerBuilder)
    {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags |= ModePowerBuilder;        // PowerBuilder ON
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;
    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeVisualbasichex()
{
    SetStatusBarText(IDS_MODE_VISUALB, 1);
    {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags |= ModeVisualBasic;        // VisualBasic ON
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;
    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeVisualchex()
{
    SetStatusBarText(IDS_MODE_VISUALC, 1);
    if (m_Appflags ^ ModeVisualC)
    {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags |= ModeVisualC;            // VisualC ON
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags &= ~ModeClarion;
    }
    OnconvertRGB();
    OnCopytoclip();
}


void CColorCopDlg::OnPopupModeClarionhex()
{
    SetStatusBarText(IDS_MODE_CLARION, 1);
    if (m_Appflags ^ ModeClarion)
    {
        m_Appflags &= ~ModeHTML;
        m_Appflags &= ~ModeDelphi;
        m_Appflags &= ~ModePowerBuilder;
        m_Appflags &= ~ModeVisualBasic;
        m_Appflags &= ~ModeVisualC;
        m_Appflags &= ~RGBFLOAT;
        m_Appflags &= ~RGBINT;
        m_Appflags |= ModeClarion;            // Clarion ON
    }
    OnconvertRGB();
    OnCopytoclip();
}


void CColorCopDlg::OnPopupRestore()
{
    m_bvisible = true;

    ShowWindow(SW_RESTORE);        // user wants to restore from systray
    bMinimized = false;            // remove the systray icon
    SetupTrayIcon();
    SetupTaskBarButton();        // add taskbar button
}

void CColorCopDlg::OnPopupExit()
{
    bMinimized = false;            // user wants to exit color cop
    SetupTrayIcon();            // remove the systray icon
    EndDialog(IDOK);
}

void CColorCopDlg::SetupStatusBar()
{

    int nTotWide;        // total width of status bar

    CRect rect;
       this->GetWindowRect(&rect);
    rect.top = rect.bottom -25;

    if (!m_StatBar.Create(CCS_NODIVIDER | WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
                          rect, this, IDC_STATUSBAR))
    {
        AfxMessageBox("Failed to create status bar.");
    }

    //
    // get size of window, use to configure the status bar
    //
    CRect rWin;
       this->GetWindowRect(&rWin);
       nTotWide = rWin.right-rWin.left;

    //
    // Make each part 1/4 of the total width of the window.
    //
    int m_Widths[1];

    m_Widths[0] = nTotWide;

    m_StatBar.SetMinHeight(25);
    m_StatBar.SetParts( 1, m_Widths);

    SetStatusBarText(IDS_RIGHTCLICK_MENU,0);

    return;
}

void CColorCopDlg::SetStatusBarText(LPCTSTR statusText)
{
    m_StatBar.SetText(statusText, 0,0);
    return;
}

void CColorCopDlg::SetStatusBarText(UINT strResource, int toggleVal) {

    CString strTemp;
    strTemp.LoadString(strResource);
    CString strOn;

    switch (toggleVal) {
        case 0:
            // Load String and set it
            m_StatBar.SetText(_T(strTemp), 0,0);
            strTemp.FreeExtra();
        break;

        case 1:
            // Load string and ON
            strOn.LoadString(IDS_ON);
            strTemp.Insert(strTemp.GetLength(), _T(" "));
            strTemp.Insert(strTemp.GetLength(), strOn);
            strTemp.FreeExtra();
            m_StatBar.SetText(_T(strTemp), 0,0);

        break;

        case 2:
            // Load string and OFF
            strOn.LoadString(IDS_OFF);
            strTemp.Insert(strTemp.GetLength(), _T(" "));
            strTemp.Insert(strTemp.GetLength(), strOn);
            strTemp.FreeExtra();
            m_StatBar.SetText(_T(strTemp), 0,0);
        break;
    }

    return;
}

BOOL CColorCopDlg::OnMouseWheel(UINT nFlags, int16_t zDelta, CPoint pt)
{
    // first check if we are magnifying...

    if (m_isEyedropping) {
        return TRUE;
    } else if (m_isMagnifying) {

        m_MagLevel += zDelta/WHEEL_DELTA;

        if (m_MagLevel <= 0)
            m_MagLevel = 1;
        else if (m_MagLevel >= 17)
            m_MagLevel = 16;
        return TRUE;
    }

    // determine which edit control has the focus??

    CWnd* curfocus = GetFocus();    // handle to the window with focus

    if (curfocus == NULL)    // There is nothing with the focus
        return TRUE;        // jump out

    int offset = 1;                    // default

    if (nFlags == MK_CONTROL)        // jump by 5 if control is down
        offset = 5;
    else if (nFlags == MK_SHIFT)    // jump by 2 if shift is down
        offset = 2;

    // if bSnaptoWebsafe is on then incrementing by 1, 5 or 2 won't do any good
    // because it will just snap lower.

    if (m_Appflags & SnaptoWebsafe)        // therefore, increment or decrement by 51
        offset = WEBSAFE_STEP;

    if (curfocus == GetDlgItem(IDC_RED)) {            // red has focus
        m_Reddec+=zDelta/WHEEL_DELTA * offset;
        m_Reddec = RangeCheck(m_Reddec);

    } else if (curfocus == GetDlgItem(IDC_GREEN)) {    // green has focus
        m_Greendec+=zDelta/WHEEL_DELTA * offset;
        m_Greendec = RangeCheck(m_Greendec);

    } else if (curfocus == GetDlgItem(IDC_BLUE)) {    // blue has focus
        m_Bluedec+=zDelta/WHEEL_DELTA * offset;
        m_Bluedec = RangeCheck(m_Bluedec);

    } else {            // there is focus, but it's not on either the Red,
        return TRUE;    // Green, or Blue edit controls -> jump out
    }


    UpdateData(false);
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

int CColorCopDlg::RangeCheck(int icolorval)
{
    // this function ensures that the user doesn't use the mouse wheel to
    // make a color decimal < 0 or > 255

    if (icolorval > 255) {
        // go right around to 0 or 1
        return (icolorval % 256);
    } else if (icolorval < 0) {
        // roll around to 255 or 254
        return (256 - abs(icolorval));
    } else {
        // it's valid -- leave it alone
        return (icolorval);
    }
}

void CColorCopDlg::OnTimer(UINT nIDEvent)
{
    // this allows animations to be magnified and eyedropped

    CString strStatus;

    switch(nIDEvent)
    {
    case 1:

        if ((m_isMagnifying) || (m_isEyedropping))
        {
                POINT point;

                if (GetCursorPos(&point))
                {

                    ScreenToClient(&point);
                    PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM (point.x, point.y));
                    //PostMessage(
                }
        }
        break;
    case 2:

        if (m_isMagMinusDown)
        {

            if (m_MagLevel != 1) {
                m_MagLevel--;
            }

            strStatus.LoadString(IDS_MAG_DECREASED);
            strStatus.Format(strStatus, m_MagLevel);
            SetStatusBarText(strStatus);

            RecalcZoom();

        } else if (m_isMagPlusDown) {

            if (m_MagLevel != 16) {
                 m_MagLevel++;
            }

            strStatus.LoadString(IDS_MAG_INCREASED);
            strStatus.Format(strStatus, m_MagLevel);
            SetStatusBarText(strStatus);

            RecalcZoom();
        }

    break;
    }

    CDialog::OnTimer(nIDEvent);
}

void CColorCopDlg::OnPopupApplicationHelp()
{
    ::HtmlHelp(NULL, _T("ColorCop.chm"), HH_DISPLAY_TOPIC, 0);
}

void CColorCopDlg::OnPopupColorDetectwebsafe()
{
    m_Appflags ^= DetectWebsafeColors;
    if (m_Appflags & DetectWebsafeColors) {
        m_Appflags &= ~SnaptoWebsafe;        // shut off snap to websafe
        SetStatusBarText(IDS_DETECTWEBSAFE,1);
    } else {
        SetStatusBarText(IDS_DETECTWEBSAFE,2);
    }
}

void CColorCopDlg::OnUpdatePopupColorDetectwebsafe(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & DetectWebsafeColors);
}

void CColorCopDlg::OnPopupApplicationEasymove()
{
    m_Appflags ^= EasyMove;
    if (m_Appflags & EasyMove) {
        SetStatusBarText(IDS_EASYMOVE,1);
    } else {
        SetStatusBarText(IDS_EASYMOVE,2);
    }
}

void CColorCopDlg::OnUpdatePopupApplicationEasymove(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & EasyMove);
}

void CColorCopDlg::OnPopupApplicationMinimizetosystemtrayonstart()
{
    m_Appflags ^= MinimizeonStart;

    if (m_Appflags & MinimizeonStart)
        SetStatusBarText(IDS_MINIMIZEONSTART, 1);
    else
        SetStatusBarText(IDS_MINIMIZEONSTART, 2);
}

void CColorCopDlg::OnUpdatePopupApplicationMinimizetosystemtrayonstart(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & MinimizeonStart);
}


void CColorCopDlg::OnPopupApplicationAllowmultipleinstances()
{
    m_Appflags ^= MultipleInstances;
    SetStatusBarText(IDS_RESTART_COLORCOP,0);
    return;
}

void CColorCopDlg::OnUpdatePopupApplicationAllowmultipleinstances(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & MultipleInstances);
}

void CColorCopDlg::OnPopupOptionsMagnifywhileeyedropping()
{
        m_Appflags ^= MAGWHILEEYEDROP;
}

void CColorCopDlg::OnUpdatePopupOptionsMagnifywhileeyedropping(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & MAGWHILEEYEDROP);

}

void CColorCopDlg::OnPopupOptionsUsecrosshaircursor()
{
    CWinApp* pApp = AfxGetApp();    // get a pointer to the one and only CWinApp
    m_Appflags ^= USECROSSHAIR;

    if (m_Appflags & USECROSSHAIR) {
        m_hEyeCursor = pApp->LoadCursor(IDC_MYCROSS);
        m_EyeLoc.SetIcon(m_hEyeCursor);
    } else {
        m_hEyeCursor = pApp->LoadCursor(IDC_EYEDROPPER);
        m_EyeLoc.SetIcon(m_hEyeCursor);
    }
    return;
}

void CColorCopDlg::OnUpdatePopupOptionsUsecrosshaircursor(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & USECROSSHAIR);
}


HCURSOR CColorCopDlg::OnQueryDragIcon()
{
    // The system calls this to obtain the cursor to display while the user drags
    //  the minimized window.

    return (HCURSOR) m_hIcon;
}

void CColorCopDlg::OnCaptureChanged(CWnd *pWnd) {
    // This is called when the app has lost capture

    if ((m_isEyedropping) || (m_isMagnifying)) {

        StopCapture();
    }
    CDialog::OnCaptureChanged(pWnd);
}

void CColorCopDlg::FireOptionMenu() {

    // Pop up the system menu when a user hits SHIFT + F10 (right click equivalent)
    // suggested by that dude from PC Mag

    CPoint targetp = (0,0);

    CMenu tempMenu;
    tempMenu.LoadMenu(IDR_COPMENU);

    ClientToScreen(&targetp);
    CMenu *pPopup = tempMenu.GetSubMenu(0);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           targetp.x, targetp.y, this, NULL);
}


PBITMAPINFO CColorCopDlg::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
        AfxMessageBox("Unable to receive bitmap information");

    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else
        cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)

     if (cClrBits != 24)
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER) +
                    sizeof(RGBQUAD) * (1<< cClrBits));

     // There is no RGBQUAD array for the 24-bit-per-pixel format.

     else
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure.

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    // For Windows NT, the width must be DWORD aligned unless
    // the bitmap is RLE compressed. This example shows this.
    // For Windows 95/98/Me, the width must be WORD aligned unless the
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the
    // device colors are important.
     pbmi->bmiHeader.biClrImportant = 0;
     return pbmi;
 }
void CColorCopDlg::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
 {

    HANDLE hf;                 // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    // TODO(j4y): error handling
    //if (!lpBits)
    //    ;

    // Retrieve the color table (RGBQUAD array) and the bits
    // (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,
        DIB_RGB_COLORS))
    {
        AfxMessageBox("GetDIBits failed");
        return;
    }

    // Create the .BMP file.
    hf = CreateFile(pszFile,
                   GENERIC_READ | GENERIC_WRITE,
                   (DWORD) 0,
                   NULL,
                   CREATE_ALWAYS,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox("Could not create BMP file");
        return;
    }

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"


    // Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
                 pbih->biSize + pbih->biClrUsed
                 * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + pbih->biClrUsed
                    * sizeof (RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),
        (LPDWORD) &dwTmp,  NULL))
    {
        AfxMessageBox("WriteFile failed");
        return;
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                  + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp, ( NULL)) )
                  {
                    AfxMessageBox("WriteFile failed");
                    return;
                  }

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
        AfxMessageBox("WriteFile failed");
    }
    // Close the .BMP file.
    if (!CloseHandle(hf))
    {
        AfxMessageBox("Could not close BMP file");
    }
    // Free memory.
    GlobalFree((HGLOBAL)lpBits);
}

bool CColorCopDlg::isWebsafeColor(int R, int G, int B) {
    // WebSafe colors have each channel as a multiple of WEBSAFE_STEP (51)
    return (R % WEBSAFE_STEP == 0) &&
           (G % WEBSAFE_STEP == 0) &&
           (B % WEBSAFE_STEP == 0);
}

void CColorCopDlg::OnUpdatePopupOptionsStartcursoroneyedropper(CCmdUI* pCmdUI)
{
        pCmdUI->SetCheck(m_Appflags & SETCURSORONEYEDROP);

}

void CColorCopDlg::OnPopupOptionsStartcursoroneyedropper()
{
    m_Appflags ^= SETCURSORONEYEDROP;
    SetStatusBarText(IDS_RESTART_COLORCOP,0);
    return;
}

void CColorCopDlg::OnPopupSamplingDecreasemultipixelaverage()
{
    // force to sampling multi.  TODO: refactor this into a method:
    m_Appflags &= ~Sampling1;
    m_Appflags &= ~Sampling3x3;
    m_Appflags &= ~Sampling5x5;
    m_Appflags |= SamplingMULTI;

    CString strStatus="";
    strStatus.LoadString(IDS_MULTIPIX_SET);

    if (m_iSamplingOffset>MULTIPIX_MIN) {
        m_iSamplingOffset--;
        strStatus.Format(strStatus, m_iSamplingOffset*2+1,m_iSamplingOffset*2+1);
    } else {
        strStatus.LoadString(IDS_MULTIPIX_LIMIT);
        strStatus.Format(strStatus, MULTIPIX_MIN*2+1,MULTIPIX_MAX*2+1);

    }
    SetStatusBarText(strStatus);
}

void CColorCopDlg::OnPopupSamplingIncreasemultipixelaverage()
{
    // force to sampling multi.  TODO: refactor this into a method:
    m_Appflags &= ~Sampling1;
    m_Appflags &= ~Sampling3x3;
    m_Appflags &= ~Sampling5x5;
    m_Appflags |= SamplingMULTI;

    CString strStatus="";
    strStatus.LoadString(IDS_MULTIPIX_SET);

    if (m_iSamplingOffset<MULTIPIX_MAX) {
        m_iSamplingOffset++;
        strStatus.Format(strStatus, m_iSamplingOffset*2+1,m_iSamplingOffset*2+1);
    } else {
        strStatus.LoadString(IDS_MULTIPIX_LIMIT);
        strStatus.Format(strStatus, MULTIPIX_MIN*2+1,MULTIPIX_MAX*2+1);

    }
    SetStatusBarText(strStatus);

}

void CColorCopDlg::OnPopupSamplingMultipixel()
{

    if (m_Appflags ^ SamplingMULTI) {
        m_Appflags &= ~Sampling1;
        m_Appflags &= ~Sampling3x3;
        m_Appflags &= ~Sampling5x5;
        m_Appflags |= SamplingMULTI;
    }
    SetStatusBarText(IDS_MULTIPIX, 0);

}

void CColorCopDlg::OnUpdatePopupSamplingMultipixel(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_Appflags & SamplingMULTI);
}

void CColorCopDlg::OnPopupSpaceRgb()
{
    if (m_Appflags ^ SpaceRGB) {
        m_Appflags &= ~SpaceCMYK;
        m_Appflags |= SpaceRGB;
    }
    ChangeColorSpace(true);
    SetStatusBarText(IDS_SPACE_RGB, 1);
}

void CColorCopDlg::OnUpdatePopupSpaceRgb(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & SpaceRGB);
}

void CColorCopDlg::OnPopupSpaceCmyk()
{
    if (m_Appflags ^ SpaceCMYK) {
        m_Appflags &= ~SpaceRGB;
        m_Appflags |= SpaceCMYK;
    }
    ChangeColorSpace(false);
    SetStatusBarText(IDS_SPACE_CMYK, 1);
}

void CColorCopDlg::OnUpdatePopupSpaceCmyk(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_Appflags & SpaceCMYK);

}

void CColorCopDlg::ChangeColorSpace(bool bRGB)
{
    CString txt;

    if(bRGB)
    {

        // Show Red
        GetDlgItem(IDC_RED)->ShowWindow(SW_SHOW);
        // Show Green
        GetDlgItem(IDC_GREEN)->ShowWindow(SW_SHOW);
        // Show Blue
        GetDlgItem(IDC_BLUE)->ShowWindow(SW_SHOW);
        // R
        txt.LoadString(IDS_R);
        m_Lbl1.SetWindowText(txt);
        // G
        txt.LoadString(IDS_G);
        m_Lbl2.SetWindowText(txt);
        // B
        txt.LoadString(IDS_B);
        m_Lbl3.SetWindowText(txt);
        // Hide 4th Label
        m_Lbl4.ShowWindow(SW_HIDE);
        // Hide Cyan
        GetDlgItem(IDC_CYAN)->ShowWindow(SW_HIDE);
        // Hide Magenta
        GetDlgItem(IDC_MAGENTA)->ShowWindow(SW_HIDE);
        // Hide Yellow
        GetDlgItem(IDC_YELLOW)->ShowWindow(SW_HIDE);
        // Hide Black
        GetDlgItem(IDC_BLACK)->ShowWindow(SW_HIDE);

    } else {

        // Show Cyan
        GetDlgItem(IDC_CYAN)->ShowWindow(SW_SHOW);
        // Show Magenta
        GetDlgItem(IDC_MAGENTA)->ShowWindow(SW_SHOW);
        // Show Yellow
        GetDlgItem(IDC_YELLOW)->ShowWindow(SW_SHOW);
        // Show Black
        GetDlgItem(IDC_BLACK)->ShowWindow(SW_SHOW);

        // C
        txt.LoadString(IDS_C);
        m_Lbl1.SetWindowText(txt);
        // M
        txt.LoadString(IDS_M);
        m_Lbl2.SetWindowText(txt);

        // Y
        txt.LoadString(IDS_Y);
        m_Lbl3.SetWindowText(txt);

        // K
        txt.LoadString(IDS_K);
        m_Lbl4.SetWindowText(txt);

        m_Lbl4.ShowWindow(SW_SHOW);

        // Hide Red
        GetDlgItem(IDC_RED)->ShowWindow(SW_HIDE);
        // Hide Green
        GetDlgItem(IDC_GREEN)->ShowWindow(SW_HIDE);
        // Hide Blue
        GetDlgItem(IDC_BLUE)->ShowWindow(SW_HIDE);
    }
    txt.FreeExtra();
    UpdateWindow();
}
