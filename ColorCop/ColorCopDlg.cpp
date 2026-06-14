// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCopDlg.cpp — Core UI implementation for the ColorCop application.
// Provides dialog setup, control wiring, color sampling and conversion logic,
// magnifier rendering, system‑tray behavior, and loading/saving of user settings.

// Precompiled header
#include "pch.h"

// Project headers
#include "ColorCop.h"
#include "ColorCopDlg.h"
#include "Label.h"       // used for the links in the AboutDlg
#include "SystemTray.h"  // used to minimize to the systray
#include "GdiHelpers.h"  // RAII wrapper for window device contexts

// Windows SDK headers (explicit APIs used in this file)
#include <commctrl.h>

// C++ standard library headers
#include <algorithm>  // std::min, std::max
#include <random>     // std::mt19937, std::uniform_int_distribution
#include <cstdlib>    // strtoul, abs
#include <cstring>    // memcpy

constexpr int WEBSAFE_STEP = 51;
constexpr int RGB_MIN = 0;
constexpr int RGB_MAX = 255;
constexpr int CMYK_MIN = 0;
constexpr int CMYK_MAX = 100;

// Minimum zoom level for the magnifier (1x)
constexpr int kMinZoom = 1;
// Maximum zoom level for the magnifier (16x)
constexpr int kMaxZoom = 16;
// Magnifier button size in pixels (square)
constexpr int kMagButtonSize = 11;

static const UINT kStatusIndicators[] = {
    ID_SEPARATOR  // single pane
};


#include "AboutDlg.h"

// Dark mode API function pointers (undocumented)
using ShouldAppsUseDarkModeFn = bool(WINAPI)();
using AllowDarkModeForWindowFn = bool(WINAPI)(HWND, bool);
using SetPreferredAppModeFn = void(WINAPI)(int);

using ShouldAppsUseDarkMode_t = ShouldAppsUseDarkModeFn*;
using AllowDarkModeForWindow_t = AllowDarkModeForWindowFn*;
using SetPreferredAppMode_t = SetPreferredAppModeFn*;

// Function pointers
static ShouldAppsUseDarkMode_t _ShouldAppsUseDarkMode = nullptr;
static AllowDarkModeForWindow_t _AllowDarkModeForWindow = nullptr;
static SetPreferredAppMode_t _SetPreferredAppMode = nullptr;

// Constants

const TCHAR* kpcTrayNotificationMsg_ = _T("color cop tray notification");

CColorCopDlg::CColorCopDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CColorCopDlg::IDD, pParent),
      m_Greendec(0),
      m_Bluedec(0),
      m_Reddec(0),
      m_Hexcolor(_T("")),
      r(0),
      g(0),
      b(0),
      m_bvisible(false),
      m_Black(0),
      m_Cyan(0),
      m_Magenta(0),
      m_Yellow(0),
      ColorHistory{},   // array
      CustColorBank{},  // array
      m_Appflags(0),
      m_iSamplingOffset(0),
      WinLocX(0),
      WinLocY(0),
      hBitmap(nullptr),
      hBitmapClip(nullptr),
      hZoomBitmap(nullptr),
      m_hIcon(nullptr),
      m_hBlank(nullptr),
      m_hEye(nullptr),
      m_hAcceleratorTable(nullptr),
      m_oldColorSaved(false),
      m_isEyedropping(false),
      m_isMagnifying(false),
      bMinimized(false),
      m_isMagPlusDown(false),
      m_isMagMinusDown(false),
      m_InitialMove(false),
      m_bCalcColorPal(false),
      bRelativePosition(false),
      m_MagDrop(false),
      smHeight(0),
      smWidth(0),
      lgHeight(0),
      lgWidth(0),
      m_nwide(0),
      m_ntall(0),
      Hue(0.0),
      Sat(0.0),
      Light(0.0),
      Swatch{},      // array of structs
      OrigSwatch{},  // struct
      m_HSV_H(0.0),
      m_HSV_S(0.0),
      m_HSV_V(0.0),
      ColorPal{},  // 2D array
      m_hEyeCursor(nullptr),
      m_hMagCursor(nullptr),
      m_hStandardCursor(nullptr),
      m_hMoveCursor(nullptr),
      m_hHandCursor(nullptr),
      hIcon_(nullptr),
      bMinimized_(false),
      pTrayIcon_(nullptr),
      nTrayNotificationMsg_(0) {

    m_brDarkMode.CreateSolidBrush(RGB(32, 32, 32));
    m_clrText = RGB(220, 220, 220);
}

void CColorCopDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorCopDlg) // NOLINT(whitespace/comments)
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
    //}}AFX_DATA_MAP // NOLINT(whitespace/comments)
}

BEGIN_MESSAGE_MAP(CColorCopDlg, CDialog)

    ON_COMMAND(ID_OPTIONMENU, FireOptionMenu)
    ON_COMMAND(ID_FLOATUP, FloatPrecisionUp)
    ON_COMMAND(ID_FLOATDOWN, FloatPrecisionDown)

    //{{AFX_MSG_MAP(CColorCopDlg) // NOLINT(whitespace/comments)
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
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_DRAWITEM()
    ON_COMMAND(ID_THEME_SYSTEM, OnThemeSystem)
    ON_COMMAND(ID_THEME_LIGHT, OnThemeLight)
    ON_COMMAND(ID_THEME_DARK, OnThemeDark)

    ON_UPDATE_COMMAND_UI(ID_THEME_SYSTEM, OnUpdateThemeSystem)
    ON_UPDATE_COMMAND_UI(ID_THEME_LIGHT, OnUpdateThemeLight)
    ON_UPDATE_COMMAND_UI(ID_THEME_DARK, OnUpdateThemeDark)

    //}}AFX_MSG_MAP // NOLINT(whitespace/comments)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCopDlg message handlers

BOOL CColorCopDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    SetupSystemMenu();  // add about and always on top to the system menu

    LoadPersistentVariables();
    ToggleOnTop(false);  // make always on top, unless save file said not to

    SetupWindowRects();
    TestForExpand();  // do not call this before SetupWindowRects();

    if (!m_ToolTip.Create(this)) {
        TRACE(_T("Unable to create a tool tip obj"));
    } else {
        m_ToolTip.AddTool(&m_ExpandDialog, IDS_EXPANDEDDIALOG);
        m_ToolTip.AddTool(&m_ColorPick, IDS_CUSTOM_COLOR);
        m_ToolTip.AddTool(this, _T(""), &buttonrect, 1);
        m_ToolTip.SetDelayTime(TTDT_INITIAL, 200);
        m_ToolTip.Activate(TRUE);
        m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 300);
    }

    nTrayNotificationMsg_ = RegisterWindowMessage(kpcTrayNotificationMsg_);

    // application variables
    m_isMagPlusDown = m_isMagMinusDown = m_oldColorSaved = FALSE;
    m_bCalcColorPal = m_isEyedropping = m_isMagnifying = FALSE;
    bMinimized_ = false;
    pTrayIcon_ = nullptr;

    // small icon
    hIcon_ = (HICON)::LoadImage(
        AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);

    // large icon
    m_hIcon = (HICON)::LoadImage(
        AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
        GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);

    SetIcon(hIcon_, FALSE);  // small icon 16x16
    SetIcon(m_hIcon, TRUE);  // big icon 32x32

    bRelativePosition = false;
    m_MagDrop = false;

    //
    //  -------- Centralized pApp handling --------
    //
    if (CWinApp* pApp = AfxGetApp()) {
        // Load common cursors/icons once
        m_hMagCursor = pApp->LoadCursor(IDC_MEDIUM_MAGNIFY);
        m_hHandCursor = pApp->LoadCursor(IDC_HANDPOINTER);
        m_hMoveCursor = pApp->LoadCursor(IDC_CURMOVE);
        m_hBlank = pApp->LoadIcon(IDI_BLANK);
        m_hStandardCursor = pApp->LoadStandardCursor(IDC_ARROW);

        m_Magnifier.SetIcon(m_hMagCursor);

        //
        // Determine the correct eyedropper cursor/icon once
        //
        HCURSOR hEyeIcon = nullptr;

        if (m_Appflags & USECROSSHAIR) {
            // Crosshair mode
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));  // visible icon
            m_hEyeCursor = pApp->LoadCursor(IDC_MYCROSS);        // actual cursor
            hEyeIcon = m_hEyeCursor;
        } else {
            // Standard eyedropper mode
            m_hEyeCursor = pApp->LoadCursor(IDC_EYEDROPPER);

            if (m_Appflags & Sampling5x5)
                hEyeIcon = pApp->LoadCursor(IDC_EYEDROPPER_5X5);
            else if (m_Appflags & Sampling3x3)
                hEyeIcon = pApp->LoadCursor(IDC_EYEDROPPER_3X3);
            else
                hEyeIcon = m_hEyeCursor;
        }

        // Apply final chosen icon
        m_EyeLoc.SetIcon(hEyeIcon);
    }

    //
    // -------------------------------------------
    //

    // upgrade case -- make sure there is a color space set
    if ((!(m_Appflags & SpaceCMYK)) && (!(m_Appflags & SpaceRGB))) {
        m_Appflags |= SpaceRGB;
    } else if (m_Appflags & SpaceCMYK) {
        ChangeColorSpace(false);
    } else {
        ChangeColorSpace(true);
    }
    OnconvertRGB();
    CalcColorPal();
    OnCopytoclip();

    // set focus on the eyedropper so there is no cursor
    m_EyeLoc.SetFocus();

    // user wants to minimize on app start
    if (m_Appflags & MinimizeOnStart) {
        m_bvisible = true;
        bMinimized = true;
        ShowWindow(SW_MINIMIZE);

        SetupTaskBarButton();
        SetupTrayIcon();
    } else if (m_Appflags & SETCURSORONEYEDROP) {
        m_bvisible = true;

        CRect eyerect;
        m_EyeLoc.GetWindowRect(&eyerect);
        SetCursorPos(eyerect.CenterPoint().x, eyerect.CenterPoint().y);
    }

    InitDarkModeAPIs();

    m_StatusBar.SubclassDlgItem(IDC_STATUSBAR, this);
    m_StatusBar.SetIndicators(kStatusIndicators, _countof(kStatusIndicators));

    ApplyTheme();

    // Load accelerator resource once MFC instance handle and window are valid
    if (m_hAcceleratorTable == nullptr) {
        m_hAcceleratorTable =
            ::LoadAccelerators(AfxGetInstanceHandle(),
                                MAKEINTRESOURCE(IDR_COLORCOP_ACCEL));
    }

    return FALSE;  // return TRUE unless you set the focus to a control
}


void CColorCopDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) {
    if (nIDCtl != IDC_ColorPick && nIDCtl != IDC_EXPAND_DIALOG) {
        CDialog::OnDrawItem(nIDCtl, lpDIS);
        return;
    }

    CDC dc;
    dc.Attach(lpDIS->hDC);

    CRect rc(lpDIS->rcItem);

    const bool pressed = (lpDIS->itemState & ODS_SELECTED);
    const bool disabled = (lpDIS->itemState & ODS_DISABLED);

    const bool dark = (m_Appflags & DarkMode);

    COLORREF bg;
    COLORREF fg;

    if (dark) {
        bg = pressed ? RGB(55, 55, 55) : RGB(32, 32, 32);
        fg = disabled ? RGB(120, 120, 120) : RGB(220, 220, 220);
    } else {
        bg = pressed ? RGB(200, 200, 200) : RGB(240, 240, 240);
        fg = disabled ? RGB(160, 160, 160) : RGB(0, 0, 0);
    }

    dc.FillSolidRect(rc, bg);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(fg);

    CString text;
    GetDlgItem(nIDCtl)->GetWindowText(text);
    dc.DrawText(text, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    dc.Detach();
}

void CColorCopDlg::SetupSystemMenu() {
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        CString text;

        // About
        text.LoadString(IDS_ABOUTBOX);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_ABOUTBOX, text);

        // Options
        text.LoadString(IDS_OPTIONMENU);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_OPTIONMENU, text);

        // Always on top
        text.LoadString(IDS_ALWAYSONTOP);
        pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_ALWAYSONTOP, text);

        // Separator before built‑in items
        pSysMenu->InsertMenu(2, MF_BYPOSITION | MF_SEPARATOR, 0);

        // Disable maximize and sizing
        pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
        pSysMenu->EnableMenuItem(SC_SIZE, MF_BYCOMMAND | MF_GRAYED);
    }
}


bool CColorCopDlg::LoadPersistentVariables() {
    TRACE(_T("Portable mode: %d\n"), m_PortableMode);

    // Build full bitmap path safely
    auto* pApp = static_cast<CColorCopApp*>(AfxGetApp());
    CString strBMPFile = pApp->GetSettingsFolder();
    strBMPFile += BMP_FILE;

    TRACE(_T("BMP path: %s\n"), strBMPFile.GetString());

    // Load bitmap from file
    hBitmap = reinterpret_cast<HBITMAP>(
        LoadImage(AfxGetApp()->m_hInstance,
                  strBMPFile,
                  IMAGE_BITMAP,
                  0, 0,
                  LR_LOADFROMFILE));

    if (hBitmap) {
        hZoomBitmap = hBitmap;
    } else {
        TRACE(_T("Warning: Could not load bitmap: %s\n"), strBMPFile.GetString());
    }

    //
    // Off‑screen recovery: If the saved window position is off-screen, reset to defaults.
    // Use the full virtual desktop (multi‑monitor safe)
    //
    RECT virtualBounds;
    virtualBounds.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    virtualBounds.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    virtualBounds.right = virtualBounds.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
    virtualBounds.bottom = virtualBounds.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);

    constexpr int kVisibilityMargin = 100;  // px

    bool offscreen =
        (WinLocX > virtualBounds.right) ||
        (WinLocY > virtualBounds.bottom) ||
        (WinLocX + kVisibilityMargin < virtualBounds.left) ||
        (WinLocY + kVisibilityMargin < virtualBounds.top);

    if (offscreen) {
        WinLocX = kDefaultWinLocX;
        WinLocY = kDefaultWinLocY;
    }

    // Restore window position (no size change)
    ::SetWindowPos(GetSafeHwnd(),
                   (m_Appflags & AlwaysOnTop) ? HWND_TOPMOST : HWND_NOTOPMOST,
                   WinLocX, WinLocY,
                   0, 0,
                   SWP_NOSIZE);
    // Ensure zoom level is valid after loading settings
    m_MagLevel = std::clamp<int>(m_MagLevel, kMinZoom, kMaxZoom);

    return true;
}

void CColorCopDlg::SetupWindowRects() {
    // setup color window rect based on the Static box
    HWND temphandle;
    CWnd::GetDlgItem(IDC_CPreview, &temphandle);
    ::GetWindowRect(temphandle, &buttonrect);
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
    CWnd::GetDlgItem(IDC_MagWindow, &maghand);
    ::GetWindowRect(maghand, &magrect);
    CWnd::ScreenToClient(&magrect);

    RECT exprect;
    HWND expandbutton;
    CWnd::GetDlgItem(IDC_EXPAND_DIALOG, &expandbutton);
    ::GetWindowRect(expandbutton, &exprect);
    CWnd::ScreenToClient(&exprect);

    // Get a rect of the entire window.
    CWnd::GetWindowRect(&CCopRect);
    CWnd::ScreenToClient(&CCopRect);

    // large sizes
    lgWidth = CCopRect.right - CCopRect.left;
    lgHeight = CCopRect.bottom - CCopRect.top;

    int collapsedClientWidth = exprect.right;
    int collapsedClientHeight = exprect.bottom;

    RECT collapsedRect = {0, 0, collapsedClientWidth, collapsedClientHeight};
    AdjustWindowRectEx(&collapsedRect, GetStyle(), FALSE, GetExStyle());

    smWidth = collapsedRect.right - collapsedRect.left + 14;
    smHeight = collapsedRect.bottom - collapsedRect.top + 14;

    // Setup magnify plus level rect
    CWnd::GetDlgItem(IDC_MAG_PLUS, &temphandle);
    ::GetWindowRect(temphandle, &magplus);
    CWnd::ScreenToClient(&magplus);

    magplus.right = magplus.left + kMagButtonSize;
    magplus.bottom = magplus.top + kMagButtonSize;

    // Setup magnify minus level rect
    CWnd::GetDlgItem(IDC_MAG_MINUS, &temphandle);
    ::GetWindowRect(temphandle, &magminus);
    CWnd::ScreenToClient(&magminus);

    magminus.right = magminus.left + kMagButtonSize;
    magminus.bottom = magminus.top + kMagButtonSize;


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
}

void CColorCopDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    // Handle system‑menu commands (About, Options, Always‑on‑Top, minimize/restore)
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else if ((nID & 0xFFF0) == IDM_OPTIONMENU) {
        FireOptionMenu();
    } else if ((nID & 0xFFF0) == IDM_ALWAYSONTOP) {
        m_Appflags ^= AlwaysOnTop;
        ToggleOnTop(true);

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL) {
            if (m_Appflags & AlwaysOnTop) {
                // check the menu item
                pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_CHECKED);
            } else {
                pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_UNCHECKED);
                // uncheck the item
            }
        }
    } else {
        BOOL bOldMin = bMinimized;  // remember previous state

        if (nID == SC_MINIMIZE) {
            bMinimized = true;
        } else if (nID == SC_RESTORE) {
            bMinimized = false;
        }

        CDialog::OnSysCommand(nID, lParam);

        if (bOldMin != bMinimized) {
            SetupTrayIcon();
            SetupTaskBarButton();
        }
    }
}

void CColorCopDlg::SetupTaskBarButton() {
    if ((bMinimized) && (m_Appflags & MinimizetoTray)) {
        // hide the tray icon if it belongs in the system tray
        ShowWindow(SW_HIDE);
        m_bvisible = false;

    } else {
        ShowWindow(SW_SHOW);
        m_bvisible = true;
    }
}

void CColorCopDlg::SetupTrayIcon() {
    if (bMinimized && (pTrayIcon_ == 0) && (m_Appflags & MinimizetoTray)) {
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

void CColorCopDlg::OnPaint() {
    if (IsIconic()) {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialog::OnPaint();
        // Custom magnifier rendering and color preview drawing
        HWND hWndMain = GetSafeHwnd();

        WindowDC hdc(hWndMain);  // RAII — auto‑releases
        if (hBitmap) {
            HDC hdcMem = ::CreateCompatibleDC(hdc);

            ::SelectObject(hdcMem, hBitmap);

            if (m_Appflags & ExpandedDialog) {
                ::BitBlt(hdc,
                         magrect.TopLeft().x + 2, magrect.TopLeft().y + 2,
                         magrect.Width() - 4, magrect.Height() - 4,
                         hdcMem,
                         0, 0,
                         SRCCOPY);
            }
            ::DeleteDC(hdcMem);
        }

        if (m_Appflags & ExpandedDialog) {
            ::DrawEdge(hdc, &magrect, EDGE_SUNKEN, BF_RECT);
        }

        DisplayColor();  // keep the color window showing
    }
}

void CColorCopDlg::ComputeHSV(int red, int green, int blue,
                              double& outH, double& outS, double& outV) {
    double rf = red / RGB_MAX_D;
    double gf = green / RGB_MAX_D;
    double bf = blue / RGB_MAX_D;

    double maxv = std::max({rf, gf, bf});
    double minv = std::min({rf, gf, bf});
    double delta = maxv - minv;

    // Value (0–100)
    outV = maxv * 100.0;

    // Saturation (0–100)
    outS = (maxv == 0.0) ? 0.0 : (delta / maxv) * 100.0;

    // Hue (0–360)
    if (delta <= HSL_EPSILON) {
        outH = 0.0;
    } else if (maxv == rf) {
        outH = 60.0 * fmod(((gf - bf) / delta), 6.0);
    } else if (maxv == gf) {
        outH = 60.0 * (((bf - rf) / delta) + HUE_SECTOR_GREEN);
    } else {
        outH = 60.0 * (((rf - gf) / delta) + HUE_SECTOR_BLUE);
    }

    if (outH < 0.0)
        outH += 360.0;
}

void CColorCopDlg::RecalcZoom() {
    ASSERT(m_MagLevel >= kMinZoom && m_MagLevel <= kMaxZoom);

    int magwidth = magrect.Width() / m_MagLevel;
    int magheight = magrect.Height() / m_MagLevel;

    HWND hwnd = GetSafeHwnd();

    if (hZoomBitmap) {
        HDC hdc = ::GetDC(hwnd);
        if (!hdc)
            return;

        HDC hdcMem = ::CreateCompatibleDC(hdc);
        HDC hdcZoomMem = ::CreateCompatibleDC(hdc);

        if (hdcMem && hdcZoomMem) {
            HGDIOBJ oldMemBmp = ::SelectObject(hdcMem, hBitmap);
            HGDIOBJ oldZoomBmp = ::SelectObject(hdcZoomMem, hZoomBitmap);

            ::StretchBlt(
                hdcMem,
                0, 0,
                magrect.Width(), magrect.Height(),
                hdcZoomMem,
                ((magrect.Width()) / 2) - (magwidth / 2),
                ((magrect.Height()) / 2) - (magheight / 2),
                magwidth, magheight,
                SRCCOPY);

            ::SelectObject(hdcMem, oldMemBmp);
            ::SelectObject(hdcZoomMem, oldZoomBmp);
        }

        if (hdcZoomMem)
            ::DeleteDC(hdcZoomMem);
        if (hdcMem)
            ::DeleteDC(hdcMem);

        ::ReleaseDC(hwnd, hdc);
    }

    InvalidateRect(&buttonrect, FALSE);
}

void CColorCopDlg::OnconvertRGB() {
    TestForWebsafe();  // before conversion

    if (m_Appflags & ModeHTML) {
        m_Hexcolor.Format(_T("#%.2x%.2x%.2x"), m_Reddec, m_Greendec, m_Bluedec);
        if (m_Appflags & OmitPound) {
            if (m_Hexcolor.Left(1) == _T("#")) {
                m_Hexcolor.Delete(0);
            }
        }
    } else if (m_Appflags & ModeDelphi) {
        m_Hexcolor.Format(_T("$00%.2x%.2x%.2x"), m_Bluedec, m_Greendec, m_Reddec);
        if (m_Appflags & OmitPound) {
            if (m_Hexcolor.Left(1) == _T("$")) {
                m_Hexcolor.Delete(0);
            }
        }
    } else if (m_Appflags & ModePowerBuilder) {
        // PowerBuilder decimal: B*65536 + G*256 + R
        m_Hexcolor.Format(_T("%d"), (65536 * m_Bluedec) + (256 * m_Greendec) + (m_Reddec));
    } else if (m_Appflags & ModeVisualBasic) {
        // &HBBGGRR
        m_Hexcolor.Format(_T("&H%.2x%.2x%.2x"), m_Bluedec, m_Greendec, m_Reddec);
    } else if (m_Appflags & ModeClarion) {
        // 0BBGGRRH
        m_Hexcolor.Format(_T("0%.2x%.2x%.2xH"), m_Bluedec, m_Greendec, m_Reddec);
    } else if (m_Appflags & RGBINT) {
        m_Hexcolor.Format(_T("%d,%d,%d"), m_Reddec, m_Greendec, m_Bluedec);
    } else if (m_Appflags & RGBFLOAT) {
        r = static_cast<float>(m_Reddec) / RGB_MAX_D;
        g = static_cast<float>(m_Greendec) / RGB_MAX_D;
        b = static_cast<float>(m_Bluedec) / RGB_MAX_D;
        m_Hexcolor.Format(_T("%0.*f,%0.*f,%0.*f"), m_FloatPrecision, r, m_FloatPrecision, g, m_FloatPrecision, b);
    } else if (m_Appflags & ModeVisualC) {
        m_Hexcolor.Format(_T("0x00%.2x%.2x%.2x"), m_Bluedec, m_Greendec, m_Reddec);
    }

    ComputeHSV(m_Reddec,
               m_Greendec,
               m_Bluedec,
               m_HSV_H,
               m_HSV_S,
               m_HSV_V);
    CString hsvTip;
    hsvTip.Format(_T("H: %.0f\xB0\nS: %.0f%%\nV: %.0f%%"),
                  m_HSV_H, m_HSV_S, m_HSV_V);

    // defensive: if the color preview control exists, update its tooltip with the new HSV values
    if (m_ColorPreview.GetSafeHwnd()) {
        m_ToolTip.UpdateTipText(hsvTip, this, 1);
    }

    // Only apply uppercase formatting when NOT in Visual C++ hex mode.
    if ((m_Appflags & ModeVisualC) == 0) {
        TestForUpperHex();
    }

    // Only update the hex edit control
    SetDlgItemText(IDC_HEXCOLOR, m_Hexcolor);

    if ((m_isEyedropping) && (m_bCalcColorPal)) {
        CalcColorPal();  // Re-Calculate color palette
    }
    Invalidate(FALSE);  // Call WM_PAINT, but don't erase background
}

void CColorCopDlg::OnconvertHEX() {
    TestForWebsafe();  // before

    // --- Format hex string ---
    if (m_Appflags & ModeHTML) {
        // Always emit full 6‑digit HTML hex
        m_Hexcolor.Format(_T("#%.2x%.2x%.2x"),
                          m_Reddec,
                          m_Greendec,
                          m_Bluedec);
    } else {
        // Delphi format: $00BBGGRR
        m_Hexcolor.Format(_T("$00%.2x%.2x%.2x"),
                          m_Bluedec,
                          m_Greendec,
                          m_Reddec);
    }

    // --- Optional prefix removal ---
    if (m_Appflags & OmitPound) {
        if ((m_Appflags & ModeHTML) && m_Hexcolor.Left(1) == _T("#")) {
            m_Hexcolor.Delete(0);
        }
        if ((m_Appflags & ModeDelphi) && m_Hexcolor.Left(1) == _T("$")) {
            m_Hexcolor.Delete(0);
        }
    }

    OnCopytoclip();
    Invalidate(FALSE);  // Call WM_PAINT, but don't erase background
}

void CColorCopDlg::CalcColorPal() {
    // generate a new color palette from the current color
    setSeedColor();  // step 1. - set the RGB color
                     //         - calc the Hue, Sat, Light
                     //         - create the swatch
    palcol = 0;
    handleShifts();
}

double CColorCopDlg::plusValue(double num) {
    num += SAT_LIGHT_SHIFT;
    if (num > HSL_MAX)
        num = HSL_MAX;

    return num;
}

double CColorCopDlg::minusValue(double num) {
    num -= SAT_LIGHT_SHIFT;
    if (num < HSL_MIN)
        num += HSL_MAX;

    return num;
}


void CColorCopDlg::handleShifts() {
    int i;
    setupSwatches();
    printSwatch();

    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = plusValue(Swatch[i].B);
        Swatch[i].C = plusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = plusValue(Swatch[i].B);
    }
    printSwatch();

    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = plusValue(Swatch[i].B);
        Swatch[i].C = minusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = minusValue(Swatch[i].B);
        Swatch[i].C = plusValue(Swatch[i].C);
    }
    printSwatch();
    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = minusValue(Swatch[i].B);
    }
    printSwatch();
    setupSwatches();
    for (i = 0; i < NUM_SWATCHES; i++) {
        Swatch[i].B = minusValue(Swatch[i].B);
        Swatch[i].C = minusValue(Swatch[i].C);
    }
    printSwatch();
}

void CColorCopDlg::printSwatch() {
    for (int i = 0; i < NUM_SWATCHES; i++) {
        HSLtoRGB(Swatch[i].A, Swatch[i].B, Swatch[i].C);
        Swatch[i].A = r;
        Swatch[i].B = g;
        Swatch[i].C = b;

        ColorPal[i][palcol] = RGB(Swatch[i].A, Swatch[i].B, Swatch[i].C);
    }
    palcol++;
}

void CColorCopDlg::setupSwatches() {
    Swatch[0].A = OrigSwatch.A;
    Swatch[0].B = OrigSwatch.B;
    Swatch[0].C = OrigSwatch.C;

    for (int i = 1; i < NUM_SWATCHES; i++) {
        Swatch[i].A = shiftHue(Swatch[i - 1].A);
        Swatch[i].B = Swatch[i - 1].B;
        Swatch[i].C = Swatch[i - 1].C;
    }
}

double CColorCopDlg::shiftHue(double hue) {
    double rethue = hue + HUE_ROTATION_STEP;
    if (rethue >= HSL_MAX)
        rethue -= HSL_MAX;
    return rethue;
}

void CColorCopDlg::setSeedColor() {
    // here we set the RGB color
    OrigSwatch.A = m_Reddec;
    OrigSwatch.B = m_Greendec;
    OrigSwatch.C = m_Bluedec;

    // convert the color to HSL
    RGBtoHSL(static_cast<int>(OrigSwatch.A), static_cast<int>(OrigSwatch.B), static_cast<int>(OrigSwatch.C));

    // set the swatch to the HSL values
    OrigSwatch.A = Hue;
    OrigSwatch.B = Sat;
    OrigSwatch.C = Light;
}

void CColorCopDlg::HSLtoRGB(double H, double S, double L) {
    if (S == 0) {
        r = g = b = static_cast<int>(L * RGB_MAX_D);
    } else {
        double h = (H - static_cast<int>(H)) * 6.0;
        int caseH = static_cast<int>(h);

        double f = h - static_cast<int>(h);
        double p = L * (1.0 - S);
        double q = L * (1.0 - S * f);
        double t = L * (1.0 - (S * (1.0 - f)));

        switch (caseH) {
        case 0:
            r = static_cast<int>((L) * RGB_MAX_D);
            g = static_cast<int>((t) * RGB_MAX_D);
            b = static_cast<int>((p) * RGB_MAX_D);
            break;
        case 1:
            r = static_cast<int>((q) * RGB_MAX_D);
            g = static_cast<int>((L) * RGB_MAX_D);
            b = static_cast<int>((p) * RGB_MAX_D);
            break;
        case 2:
            r = static_cast<int>((p) * RGB_MAX_D);
            g = static_cast<int>((L) * RGB_MAX_D);
            b = static_cast<int>((t) * RGB_MAX_D);
            break;
        case 3:
            r = static_cast<int>((p) * RGB_MAX_D);
            g = static_cast<int>((q) * RGB_MAX_D);
            b = static_cast<int>((L) * RGB_MAX_D);
            break;
        case 4:
            r = static_cast<int>((t) * RGB_MAX_D);
            g = static_cast<int>((p) * RGB_MAX_D);
            b = static_cast<int>((L) * RGB_MAX_D);
            break;
        case 5:
            r = static_cast<int>((L) * RGB_MAX_D);
            g = static_cast<int>((p) * RGB_MAX_D);
            b = static_cast<int>((q) * RGB_MAX_D);
            break;
        }
    }
}

void CColorCopDlg::UpdateCMYKFromRGB(int red, int green, int blue) {
    // Normalize RGB to [0,1]
    const double rNorm = static_cast<double>(red) / RGB_MAX_D;
    const double gNorm = static_cast<double>(green) / RGB_MAX_D;
    const double bNorm = static_cast<double>(blue) / RGB_MAX_D;

    // Compute K (black)
    const double maxRGB = std::max({rNorm, gNorm, bNorm});
    const double K = 1.0 - maxRGB;

    double C = 0.0;
    double M = 0.0;
    double Y = 0.0;

    if (K < 1.0) {
        const double denom = 1.0 - K;
        C = (1.0 - rNorm - K) / denom;
        M = (1.0 - gNorm - K) / denom;
        Y = (1.0 - bNorm - K) / denom;
    }

    // Convert CMYK to 0–100 integer percentage range

    m_Cyan = static_cast<int>(std::round(C * CMYK_MAX));
    m_Magenta = static_cast<int>(std::round(M * CMYK_MAX));
    m_Yellow = static_cast<int>(std::round(Y * CMYK_MAX));
    m_Black = static_cast<int>(std::round(K * CMYK_MAX));
}

void CColorCopDlg::RGBtoHSL(double R, double G, double B) {
    // the function converts the RGB model to the HSL model.

    double MinNum, MaxNum, Diff;
    double R_Dist, G_Dist, B_Dist;

    /////////////////////////////////////
    // get min, max, and diff
    //
    MinNum = std::min({R, G, B});
    MaxNum = std::max({R, G, B});
    Diff = (MaxNum - MinNum);

    // compute Light early (needed for grayscale complement)
    Light = MaxNum / RGB_MAX_D;

    // detect grayscale
    const bool isGray = (Diff <= HSL_EPSILON);

    if (isGray) {
        // grayscale has no hue; neutral-axis complement computed but not stored
        // (maps L in [0..1] back into the 0..255 RGB domain)
        const uint16_t inv = static_cast<uint16_t>(RGB_MAX - (Light * RGB_MAX_D));
        (void)inv;  // TODO(j4y): expose complement color in UI if a future feature requires it

        // grayscale HSL values
        Sat = 0.0;
        Hue = 0.0;

        return;
    }


    // find the Saturation
    if ((MaxNum == RGB_MAX) || (MinNum == 0)) {
        Sat = 1.0;
    }

    if (MaxNum != 0) {
        Sat = Diff / MaxNum;
    }

    // find the Hue
    R_Dist = (MaxNum - R) / Diff;
    G_Dist = (MaxNum - G) / Diff;
    B_Dist = (MaxNum - B) / Diff;

    if (R == MaxNum) {
        Hue = B_Dist - G_Dist;
    } else if (G == MaxNum) {
        Hue = HUE_SECTOR_GREEN + R_Dist - B_Dist;
    } else {  // B == MaxNum
        Hue = HUE_SECTOR_BLUE + G_Dist - R_Dist;
    }
    Hue = Hue / HUE_CYCLE;

    if (Hue < 0.0) {
        Hue = Hue + HSL_MAX;
    }
}

void CColorCopDlg::DisplayColor() {
    CDC* pDC = GetDC();

    CBrush blackbrush;  // create a black brush
    blackbrush.CreateSolidBrush(0x00000000);

    CBrush greybrush;  // create a grey brush
    greybrush.CreateSolidBrush(0x00808080);

    pDC->FillSolidRect(buttonrect, RGB(m_Reddec, m_Greendec, m_Bluedec));
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

        m_nwide = static_cast<int>((palWidth) / 6.0);
        m_ntall = static_cast<int>((palHeight) / 7.0);

        CRect insiderect = colorpalrect;
        insiderect.right = insiderect.left + m_nwide;
        insiderect.bottom = insiderect.top + m_ntall;

        for (int col = 0; col < 7; col++) {
            if (col) {
                // not the top row

                insiderect.top += m_ntall;
                insiderect.bottom += m_ntall;
            }
            for (int row = 0; row < 6; row++) {
                pDC->FillSolidRect(insiderect, ColorPal[row][col]);

                insiderect.right += m_nwide;
                insiderect.left += m_nwide;
            }

            insiderect.right -= m_nwide * 6;
            insiderect.left -= m_nwide * 6;
        }

        // calculate the rect for FrameRect
        colorpalrect.right = colorpalrect.left + m_nwide * 6;
        colorpalrect.bottom = colorpalrect.top + m_ntall * 7;
        colorpalrect.InflateRect(1, 1, 1, 1);

        pDC->FrameRect(colorpalrect, &blackbrush);

        colorpalrect.DeflateRect(1, 1, 1, 1);

        pDC->FrameRect(magplus, &greybrush);
        pDC->MoveTo(magplus.left + 2, magplus.top + (magplus.bottom - magplus.top) / 2);
        pDC->LineTo(magplus.right - 2, magplus.top + (magplus.bottom - magplus.top) / 2);

        pDC->MoveTo(magplus.left + (magplus.right - magplus.left) / 2, magplus.top + 2);
        pDC->LineTo(magplus.left + (magplus.right - magplus.left) / 2, magplus.bottom - 2);

        pDC->FrameRect(magminus, &greybrush);
        pDC->MoveTo(magminus.left + 2, magminus.top + (magminus.bottom - magminus.top) / 2);
        pDC->LineTo(magminus.right - 2, magminus.top + (magminus.bottom - magminus.top) / 2);

        if ((m_Appflags & MAGWHILEEYEDROP) && (m_isEyedropping) && (!m_MagDrop)) {
            insiderect = magrect;
            insiderect.DeflateRect(magrect.Width() / 2, magrect.Height() / 2);
            insiderect.InflateRect(3, 3);  // mag 4
            insiderect.left += 4;
            insiderect.right += 4;
            insiderect.bottom += 4;
            insiderect.top += 4;


            if ((m_Appflags & Sampling3x3) ||
                (m_Appflags & Sampling5x5) ||
                ((m_Appflags & SamplingMULTI) && (m_iSamplingOffset <= 10))) {
                insiderect.InflateRect(4 * m_iSamplingOffset, 4 * m_iSamplingOffset);
            }

            pDC->FrameRect(&insiderect, &blackbrush);
            // show the point that the eyedropper is on
        }
    }

    ReleaseDC(pDC);  // free memory
    return;
}

void CColorCopDlg::OnAbout() {
    CAboutDlg dlg;
    dlg.DoModal();
}

void CColorCopDlg::OnChangeGreen() {
    m_Greendec = GetDlgItemInt(IDC_GREEN);
    m_Greendec = std::clamp(m_Greendec, RGB_MIN, RGB_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeBlue() {
    m_Bluedec = GetDlgItemInt(IDC_BLUE);
    m_Bluedec = std::clamp(m_Bluedec, RGB_MIN, RGB_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}


void CColorCopDlg::OnChangeRed() {
    m_Reddec = GetDlgItemInt(IDC_RED);
    m_Reddec = std::clamp(m_Reddec, RGB_MIN, RGB_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeBlack() {
    m_Black = GetDlgItemInt(IDC_BLACK);
    m_Black = std::clamp(m_Black, CMYK_MIN, CMYK_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeCyan() {
    m_Cyan = GetDlgItemInt(IDC_CYAN);
    m_Cyan = std::clamp(m_Cyan, CMYK_MIN, CMYK_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeMagenta() {
    m_Magenta = GetDlgItemInt(IDC_MAGENTA);
    m_Magenta = std::clamp(m_Magenta, CMYK_MIN, CMYK_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnChangeYellow() {
    m_Yellow = GetDlgItemInt(IDC_YELLOW);
    m_Yellow = std::clamp(m_Yellow, CMYK_MIN, CMYK_MAX);

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnColorPick() {
    // set up the common windows color dialog
    COLORREF temp;
    CColorDialog dlgcolor;
    dlgcolor.m_cc.rgbResult = RGB(m_Reddec, m_Greendec, m_Bluedec);
    dlgcolor.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN;
    dlgcolor.m_cc.lpCustColors = CustColorBank;  // pointer to custom colors aray

    // show the dialog
    if (dlgcolor.DoModal() == IDOK) {
        // if they pressed OK
        temp = dlgcolor.GetColor();

        m_Reddec = GetRValue(temp);
        m_Greendec = GetGValue(temp);
        m_Bluedec = GetBValue(temp);

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

void CColorCopDlg::OnCopytoclip() {
    if (m_Appflags & AutoCopytoClip) {
        HWND hWndOwner = ::GetForegroundWindow();

        if (::OpenClipboard(hWndOwner)) {
            ::EmptyClipboard();

            // CString is UTF‑16 in Unicode builds, so copy its raw buffer.
            const int len = m_Hexcolor.GetLength();
            const SIZE_T bytes = (len + 1) * sizeof(wchar_t);

            HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, bytes);
            if (hMem) {
                void* pMem = ::GlobalLock(hMem);
                if (pMem) {
                    std::memcpy(pMem, m_Hexcolor.GetString(), bytes);
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

void CColorCopDlg::StopCapture() {
    // we don't want to capture anymore, they let up the left mouse button, or hit ESC
    m_isMagnifying = m_isEyedropping = FALSE;
    SetCursor(m_hStandardCursor);  // put standard cursor back on

    ReleaseCapture();  // let go of the mouse

    CWinApp* pApp = AfxGetApp();

    // put the icons back in their holders
    if (m_Appflags & USECROSSHAIR) {
        m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_MYCROSS));  // location cursor
    } else {
        if (m_Appflags & Sampling5x5) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_5X5));
        } else if (m_Appflags & Sampling3x3) {
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_3X3));
        } else {                                                 // 1x1 or multi
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));  // location cursor
        }
    }
    m_Magnifier.SetIcon(m_hMagCursor);

    bRelativePosition = false;
    // stop the timer
    KillTimer(1);

    return;
}

void CColorCopDlg::OnLButtonDown(UINT nFlags, CPoint point) {
    CDialog::OnLButtonDown(nFlags, point);
    CString strStatus = "";

    // the left mouse button was pressed, lets find out where

    CWnd* pWnd = ChildWindowFromPoint(point);
    if (pWnd && pWnd->GetSafeHwnd() == m_EyeLoc.GetSafeHwnd()) {
        // left mouse button down on the eyedropper
        m_isEyedropping = TRUE;
        m_InitialMove = TRUE;
        m_bCalcColorPal = TRUE;  // ReCalculate Color Palette

        SetCapture();
        SetTimer(1, 100, NULL);  // install the timer

        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Magnifier.GetSafeHwnd()) {
        //  left mouse button down on the magnifier

        SetCursor(m_hMagCursor);
        m_isMagnifying = true;
        SetCapture();
        SetTimer(1, 100, NULL);  // install the timer

        m_Magnifier.SetIcon(m_hBlank);

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagWindow.GetSafeHwnd()) {
        // left mouse button down on magnification window
        // we should get the color, because we can.
        // it's in the client window.


        if (m_Appflags & MAGWHILEEYEDROP) {
            m_MagDrop = true;
        }


        m_isEyedropping = TRUE;
        m_InitialMove = TRUE;    // Push color to color history
        m_bCalcColorPal = TRUE;  // ReCalculate Color Palette
        SetCapture();
        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM(point.x, point.y));
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_ColorPalette.GetSafeHwnd()) {
        m_isEyedropping = TRUE;
        m_InitialMove = FALSE;    // Push color to color history
        m_bCalcColorPal = FALSE;  // ReCalculate Color Palette
        SetCapture();
        m_EyeLoc.SetIcon(m_hBlank);
        SetCursor(m_hEyeCursor);
        PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM(point.x, point.y));
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagPlus.GetSafeHwnd()) {
        if (m_MagLevel != kMaxZoom) {
            m_MagLevel++;
        }

        strStatus.LoadString(IDS_MAG_INCREASED);
        strStatus.Format(strStatus, m_MagLevel);
        SetStatusBarText(strStatus);

        RecalcZoom();

        m_isMagPlusDown = TRUE;
        SetTimer(2, 145, NULL);  // install the timer
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagMinus.GetSafeHwnd()) {
        if (m_MagLevel != kMinZoom) {
            m_MagLevel--;
        }

        strStatus.LoadString(IDS_MAG_DECREASED);
        strStatus.Format(strStatus, m_MagLevel);
        SetStatusBarText(strStatus);

        RecalcZoom();

        m_isMagMinusDown = TRUE;
        SetTimer(2, 145, NULL);  // install the timer
        return;

    } else if (pWnd && pWnd->GetSafeHwnd() == m_Q1.GetSafeHwnd()) {
        GetHistoryColor(0);  // calls on copy clip
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

        if (m_Appflags & EasyMove) {  // one click move is enabled
            PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
            SetCursor(m_hMoveCursor);
        }
    }
    return;
}

void CColorCopDlg::OnLButtonDblClk(UINT nFlags, CPoint point) {
    HWND CCopHWND = AfxGetApp()->GetMainWnd()->m_hWnd;

    CWnd* pWnd = ChildWindowFromPoint(point);

    if (pWnd && pWnd->GetSafeHwnd() == m_ColorPreview.GetSafeHwnd()) {
        // User double-clicked the color preview: open the custom color dialog
        OnColorPick();
        return;
    } else if (pWnd && pWnd->GetSafeHwnd() == m_MagWindow.GetSafeHwnd()) {
        if (hBitmap) {
            CString strQuestion;
            strQuestion.LoadString(IDS_COPY_BITMAP);

            if (IDYES == AfxMessageBox(strQuestion, MB_YESNO)) {
                if (::OpenClipboard(CCopHWND)) {
                    ::EmptyClipboard();

                    hBitmapClip = CopyBitmap(hBitmap);
                    SetClipboardData(CF_BITMAP, hBitmapClip);
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
        SetCursor(m_hStandardCursor);  // put standard cursor back on
        return;
    }
    CDialog::OnLButtonDblClk(nFlags, point);
}

void CColorCopDlg::OnThemeSystem() {
    m_Appflags &= ~DarkMode;    // clear dark bit
    m_Appflags &= ~ForceLight;  // optional future bit
    ApplyTheme();
}

void CColorCopDlg::OnThemeLight() {
    m_Appflags &= ~DarkMode;
    m_Appflags |= ForceLight;
    ApplyTheme();
}

void CColorCopDlg::OnThemeDark() {
    m_Appflags |= DarkMode;
    m_Appflags &= ~ForceLight;
    ApplyTheme();
}

void CColorCopDlg::ApplyTheme() {
    BOOL useDark = (m_Appflags & DarkMode);

    if (_SetPreferredAppMode)
        _SetPreferredAppMode(useDark ? 1 : 0);

    BOOL dark = useDark ? TRUE : FALSE;
    DwmSetWindowAttribute(GetSafeHwnd(),
                          DWMWA_USE_IMMERSIVE_DARK_MODE,
                          &dark,
                          sizeof(dark));
    // Tell the status bar to update its colors
    m_StatusBar.SetTheme(useDark);

    Invalidate(TRUE);
    UpdateWindow();
}

void CColorCopDlg::OnUpdateThemeSystem(CCmdUI* pCmdUI) {
    BOOL isChecked = static_cast<BOOL>(
        ((m_Appflags & DarkMode) == 0) &&
        ((m_Appflags & ForceLight) == 0));
    pCmdUI->SetCheck(isChecked);
}

void CColorCopDlg::OnUpdateThemeLight(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(static_cast<BOOL>((m_Appflags & ForceLight) != 0));
}

void CColorCopDlg::OnUpdateThemeDark(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(static_cast<BOOL>((m_Appflags & DarkMode) != 0));
}

HBITMAP CColorCopDlg::CopyBitmap(HBITMAP hBitmapSrc) {
    BITMAP bitmap;
    HBITMAP hBitmapDst;
    HDC hdcSrc, hdcDst;

    ::GetObject(hBitmapSrc, sizeof(BITMAP), &bitmap);
    hBitmapDst = ::CreateBitmapIndirect(&bitmap);

    hdcSrc = ::CreateCompatibleDC(NULL);
    hdcDst = ::CreateCompatibleDC(NULL);

    ::SelectObject(hdcSrc, hBitmapSrc);
    ::SelectObject(hdcDst, hBitmapDst);

    ::BitBlt(hdcDst, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
             hdcSrc, 0, 0, SRCCOPY);

    ::DeleteDC(hdcSrc);
    ::DeleteDC(hdcDst);

    return hBitmapDst;
}

void CColorCopDlg::OnLButtonUp(UINT nFlags, CPoint point) {
    // Stop eyedropper or magnifier mode
    if (m_isEyedropping || m_isMagnifying) {
        OnCopytoclip();

        if (bRelativePosition) {
            bRelativePosition = FALSE;
        }
        Invalidate(TRUE);
        StopCapture();
    }

    // Reset magnifier drop state
    m_MagDrop = false;

    // Stop auto-repeat zoom timers
    if (m_isMagMinusDown) {
        m_isMagMinusDown = FALSE;
        KillTimer(2);
    }
    if (m_isMagPlusDown) {
        m_isMagPlusDown = FALSE;
        KillTimer(2);
    }

    // Restore cursor if EasyMove was active
    if (m_Appflags & EasyMove) {
        SetCursor(m_hStandardCursor);
    }

    CDialog::OnLButtonUp(nFlags, point);
}

void CColorCopDlg::OnMouseMove(UINT nFlags, CPoint point) {
    CString strStatus;

    //
    // Handle capture for eyedropper or magnifier
    //
    if (m_isEyedropping || m_isMagnifying) {
        if (::GetCapture() == nullptr) {
            SetCapture();
        } else {
            ClientToScreen(&point);  // convert to screen coordinates
        }
    }

    //
    // Eyedropper mode
    //
    if (m_isEyedropping) {
        bool skipColor = false;

        if (m_InitialMove) {
            AdvanceColorHistory();
            m_InitialMove = FALSE;
        }

        WindowDC hdc(nullptr);  // RAII DC

        if (m_Appflags & Sampling1) {
            const COLORREF crefxy = ::GetPixel(hdc, point.x, point.y);

            if (crefxy != CLR_INVALID) {
                const COLORREF current = RGB(m_Reddec, m_Greendec, m_Bluedec);

                if (current != crefxy) {
                    m_Reddec = GetRValue(crefxy);
                    m_Greendec = GetGValue(crefxy);
                    m_Bluedec = GetBValue(crefxy);

                    UpdateCMYKFromRGB(m_Reddec, m_Greendec, m_Bluedec);
                } else {
                    skipColor = true;
                }
            }
        } else {
            // 3×3, 5×5, or multi‑pixel sampling
            skipColor = AveragePixelArea(hdc, &m_Reddec, &m_Greendec, &m_Bluedec, point);
            UpdateCMYKFromRGB(m_Reddec, m_Greendec, m_Bluedec);
        }

        //
        // Relative‑position mode
        //
        if (bRelativePosition) {
            RelativePointEnd = point;

            int dx = point.x - RelativePoint.x;
            int dy = point.y - RelativePoint.y;

            const double w = static_cast<double>(dx + 1);
            const double h = static_cast<double>(dy + 1);

            const double length = std::sqrt(w * w + h * h);
            const double angle = std::atan2(h, w) * (180.0 / kPi);

            strStatus.LoadString(IDS_RELATIVE_POS);
            strStatus.Format(strStatus, dx, dy, length, angle);
        } else if ((m_Appflags & Sampling3x3) ||
                   (m_Appflags & Sampling5x5) ||
                   (m_Appflags & SamplingMULTI)) {
            strStatus.LoadString(IDS_EYEDROPPING);
            strStatus += ", %dx%d";
            strStatus.Format(strStatus,
                             point.x,
                             point.y,
                             m_iSamplingOffset * 2 + 1,
                             m_iSamplingOffset * 2 + 1);
        } else {
            strStatus.LoadString(IDS_EYEDROPPING);

            if (m_Appflags & DetectWebsafeColors) {
                CString strWebSafe;
                strWebSafe.LoadString(
                    isWebsafeColor(m_Reddec, m_Greendec, m_Bluedec)
                        ? IDS_WEBSAFE
                        : IDS_NOT_WEBSAFE);
                strStatus += strWebSafe;
            }

            strStatus.Format(CString(strStatus), point.x, point.y);
        }

        //
        // Magnify‑while‑eyedropping
        //
        if (m_Appflags & MAGWHILEEYEDROP) {
            if (!m_MagDrop) {
                m_MagLevel = 4;
                GetScreenBitmap(point);
                InvalidateRect(&magrect, FALSE);
            }
        }

        SetStatusBarText(strStatus);

        if (!skipColor) {
            OnconvertRGB();
        }

        return;
    }

    //
    // Magnifier mode
    //
    if (m_isMagnifying) {
        strStatus.LoadString(IDS_MAGNIFYING);
        strStatus.Format(strStatus, point.x, point.y, m_MagLevel);

        SetStatusBarText(strStatus);
        GetScreenBitmap(point);
        InvalidateRect(magrect, FALSE);
        return;
    }

    //
    // Normal mode — cursor changes
    //
    if (CWnd* pWnd = ChildWindowFromPoint(point)) {
        const HWND h = pWnd->GetSafeHwnd();

        if (h == m_EyeLoc.GetSafeHwnd() ||
            h == m_Magnifier.GetSafeHwnd() ||
            h == m_MagPlus.GetSafeHwnd() ||
            h == m_MagMinus.GetSafeHwnd() ||
            h == m_Q1.GetSafeHwnd() ||
            h == m_Q2.GetSafeHwnd() ||
            h == m_Q3.GetSafeHwnd() ||
            h == m_Q4.GetSafeHwnd() ||
            h == m_Q5.GetSafeHwnd() ||
            h == m_Q6.GetSafeHwnd() ||
            h == m_Q7.GetSafeHwnd()) {
            SetCursor(m_hHandCursor);
        } else if (h == m_MagWindow.GetSafeHwnd() ||
                   h == m_ColorPalette.GetSafeHwnd()) {
            SetCursor(m_hEyeCursor);
        }
    }

    CDialog::OnMouseMove(nFlags, point);
}

void CColorCopDlg::GetScreenBitmap(CPoint point) {
    if (hBitmap) {  // delete the old bitmap, right before we get a new one
        ::DeleteObject(hBitmap);
        hBitmap = NULL;
    }
    if (hZoomBitmap) {
        ::DeleteObject(hZoomBitmap);
        hZoomBitmap = NULL;
    }

    WindowDC hdc(NULL);  // device context to the whole desktop (RAII)

    HDC hdcMem = ::CreateCompatibleDC(hdc);
    HDC hdcZoomMem = ::CreateCompatibleDC(hdc);

    hBitmap = CreateCompatibleBitmap(hdc, magrect.Width(), magrect.Height());
    hZoomBitmap = CreateCompatibleBitmap(hdc, magrect.Width(), magrect.Height());

    ::SelectObject(hdcMem, hBitmap);
    ::SelectObject(hdcZoomMem, hZoomBitmap);
    ::SetStretchBltMode(hdc, COLORONCOLOR);

    m_MagLevel = std::clamp<int>(m_MagLevel, kMinZoom, kMaxZoom);

    const int level = m_MagLevel;
    const int magwidth = magrect.Width() / level;
    const int magheight = magrect.Height() / level;

    ::BitBlt(hdcZoomMem,                         // destination DC
             0, 0,                               // destination upper left (always 0, 0)
             magrect.Width(), magrect.Height(),  // w x h of destination
             hdc,                                // source DC
             point.x - (magrect.Width() / 2),    // x coordinate of source
             point.y - (magrect.Height() / 2),   // y coordinate of source
             SRCCOPY);                           // raster mode

    ::StretchBlt(hdcMem,                             // destination DC
                 0, 0,                               // destination upper left (always 0, 0)
                 magrect.Width(), magrect.Height(),  // w x h of destination
                 hdc,                                // source DC
                 point.x - (magwidth / 2),           // x coordinate of source
                 point.y - (magheight / 2),          // y coordinate of source
                 magwidth, magheight,                // width of source
                 SRCCOPY);                           // raster mode

    ::DeleteDC(hdcMem);
    ::DeleteDC(hdcZoomMem);
    // hdc auto‑releases via RAII
    return;
}

void CColorCopDlg::GetHistoryColor(int Cindex) {
    // this function sets the current color
    // to the history color which was clicked on

    if (RGB(m_Reddec, m_Greendec, m_Bluedec) != ColorHistory[Cindex]) {
        // the color is different.  get it

        m_Reddec = GetRValue(ColorHistory[Cindex]);
        m_Greendec = GetGValue(ColorHistory[Cindex]);
        m_Bluedec = GetBValue(ColorHistory[Cindex]);

        UpdateData(false);  // update vars
        OnconvertRGB();
        CalcColorPal();
        OnCopytoclip();
    }
}

void CColorCopDlg::AdvanceColorHistory() {
    // this function will advance the colors in the color history..
    // the 7th color is lost, the first color is the current color
    // which is soon to be the older color.

    // if the new color is the same as the last color, don't switch.
    if (ColorHistory[0] == (COLORREF) RGB(m_Reddec, m_Greendec, m_Bluedec))
        return;

    for (int sk = 6; sk > 0; sk--) {
        ColorHistory[sk] = ColorHistory[sk - 1];
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

BOOL CColorCopDlg::PreTranslateMessage(MSG* pMsg) {
    POINT pt;               // cursor location
    static int repeat = 1;  // repeat key counter

    // Forward mouse messages to tooltip control
    if (m_ToolTip.GetSafeHwnd() &&
        ::IsWindow(m_ToolTip.GetSafeHwnd())) {
        m_ToolTip.RelayEvent(pMsg);
    }

    // Accelerator handling
    if (m_hAcceleratorTable && m_hWnd != NULL && ::IsWindow(m_hWnd)) {
        if (::TranslateAccelerator(m_hWnd, m_hAcceleratorTable, pMsg)) {
            return TRUE;
        }
    }

    /************************************************
    * ESC key Check
    * - hitting the escape key should only stop the
    *   eyedropper or magnifier and should not exit
    ************************************************/
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_ESCAPE) {
            if (m_isEyedropping || m_isMagnifying) {
                StopCapture();
                m_isEyedropping = m_isMagnifying = FALSE;
            }
            if (bRelativePosition) {
                bRelativePosition = FALSE;
            }
            return TRUE;
        } else if (m_isEyedropping || m_isMagnifying) {
            CString strStatus;
            CWinApp* pApp;

            switch (pMsg->wParam) {
            case VK_CONTROL:
                if (m_isEyedropping && GetCursorPos(&RelativePoint)) {
                    RelativePointEnd = RelativePoint;
                    strStatus.LoadString(IDS_RELATIVE_POS);
                    strStatus.Format(strStatus, 0, 0, 0, 90);
                    SetStatusBarText(strStatus);

                    if (!(m_Appflags & USECROSSHAIR)) {
                        pApp = AfxGetApp();
                        m_hEyeCursor = pApp->LoadCursor(IDC_MYCROSS);
                        SetCursor(m_hEyeCursor);
                    }
                    bRelativePosition = TRUE;
                }
                break;

            case VK_RIGHT:
                if (GetCursorPos(&pt))
                    SetCursorPos(pt.x + repeat, pt.y);
                break;
            case VK_LEFT:
                if (GetCursorPos(&pt))
                    SetCursorPos(pt.x - repeat, pt.y);
                break;
            case VK_UP:
                if (GetCursorPos(&pt))
                    SetCursorPos(pt.x, pt.y - repeat);
                break;
            case VK_DOWN:
                if (GetCursorPos(&pt))
                    SetCursorPos(pt.x, pt.y + repeat);
                break;
            }
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CColorCopDlg::OnChangeHexcolor() {
    // the user is typing in the hex edit control
    // or they pasted a hex code.
    // OK - parse out the RGB values and convert

    int hexrange, offset;

    UpdateData(1);  // save from control to m_Hexcolor
    int hexsize = 0;
    hexsize = m_Hexcolor.GetLength();

    if (m_Appflags & ModeHTML) {
        if (m_Hexcolor.Left(1) == _T("#")) {
            hexrange = 5;
            offset = 1;
        } else {
            hexrange = 6;
            offset = 0;
        }
        while (hexsize > hexrange) {
            m_Hexcolor.Delete(6 + offset);
            hexsize--;
        }
        m_Reddec = m_Greendec = m_Bluedec = 0;
        ParseHTML(m_Hexcolor);

    } else if (m_Appflags & ModeClarion) {
        // Clarion Starts with 0s
        if (m_Hexcolor.Left(1) == _T("0")) {
            hexrange = 7;
            offset = 1;
            ParseClarion(m_Hexcolor);
        }
    } else {  // Delphi hex change
        if (m_Hexcolor.Left(1) == _T("$")) {
            hexrange = 7;
            offset = 1;
        } else {
            hexrange = 8;
            offset = 0;
        }

        while (hexsize > hexrange) {
            m_Hexcolor.Delete(8 + offset);
            hexsize--;
        }
        m_Reddec = m_Greendec = m_Bluedec = 0;
        ParseDelphi(m_Hexcolor);
    }
}

void CColorCopDlg::ParseDelphi(CString inst) {
    // they are typing in delphi hex codes
    // determine the RGB values

    if (inst.GetLength() < 3) {
        m_Reddec = m_Greendec = m_Bluedec = 0;
        UpdateData(false);
        return;
    }

    if (inst.Left(1) == _T('$')) {
        inst.Delete(0);
    }
    inst.Delete(0);
    inst.Delete(0);
    m_Bluedec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    inst.Delete(0);
    inst.Delete(0);
    m_Greendec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    inst.Delete(0);
    inst.Delete(0);
    m_Reddec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    UpdateData(FALSE);

    inst.FreeExtra();
    OnconvertHEX();
    return;
}


void CColorCopDlg::ParseClarion(CString inst) {
    // they are typing in clarion hex codes
    // determine the RGB values
    if (inst.GetLength() < 2) {
        m_Reddec = m_Greendec = m_Bluedec = 0;
        UpdateData(false);
        return;
    }

    if (inst.Left(1) == _T("0")) {
        inst.Delete(0);
    }

    m_Bluedec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));
    inst.Delete(0);
    inst.Delete(0);

    m_Greendec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));
    inst.Delete(0);
    inst.Delete(0);

    m_Reddec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    UpdateData(FALSE);

    inst.FreeExtra();
    OnconvertHEX();
    return;
}


void CColorCopDlg::ParseHTML(CString inst) {
    // the user is typing in HTML hex codes
    // get the RGB values
    if (inst.Left(1) == _T("#")) {
        inst.Delete(0);
    }

    m_Reddec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    inst.Delete(0);
    inst.Delete(0);
    m_Greendec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    inst.Delete(0);
    inst.Delete(0);
    m_Bluedec = static_cast<int>(_tcstoul(inst.Left(2), nullptr, 16));

    UpdateData(FALSE);

    OnconvertHEX();

    return;
}

BOOL CColorCopDlg::GetShellFolderPath(LPCTSTR pShellFolder, LPTSTR pShellPath) {
    // pShellFolder can be one of the following
    // AppData, Cache, Cookies, Desktop, Favorites, Fonts, History, NetHood,
    // Personal, Printhood, Programs, Recent, SendTo, Start Menu, Startup,
    // Templates, ShellNew
    DWORD rc;
    DWORD length = MAX_PATH;
    DWORD type = REG_SZ;
    HKEY hkey;

    rc = RegOpenKeyEx(HKEY_CURRENT_USER,
                      _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
                      0, KEY_READ, &hkey);

    if (rc == ERROR_SUCCESS) {
        rc = RegQueryValueEx(hkey, pShellFolder, NULL, &type,
                             reinterpret_cast<BYTE*>(pShellPath), &length);
        RegCloseKey(hkey);
    }

    if (rc == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

void CColorCopDlg::OnDestroy() {
    // Capture and store the dialog’s last on-screen position so it can be
    // restored the next time the application starts.
    RECT winSize{};
    GetWindowRect(&winSize);
    WinLocX = winSize.left;
    WinLocY = winSize.top;

    // save the bitmap
    auto* pApp = static_cast<CColorCopApp*>(AfxGetApp());
    CString strBMPFile = pApp->GetSettingsFolder();
    strBMPFile += BMP_FILE;

    TRACE(_T("Portable mode: %d\n"), m_PortableMode);
    TRACE(_T("BMP path: %s\n"), strBMPFile.GetString());

    HWND curwindowhwnd = ::GetForegroundWindow();

    if (hBitmap) {
        PBITMAPINFO MagBmpInfo = CreateBitmapInfoStruct(curwindowhwnd, hBitmap);
        CreateBMPFile(curwindowhwnd, strBMPFile.GetBuffer(MAX_PATH), MagBmpInfo, hBitmap, ::GetDC(NULL));

        strBMPFile.ReleaseBuffer();
    }

    CDialog::OnDestroy();
    return;
}

void CColorCopDlg::OnFileExit() {
    EndDialog(IDOK);
}

void CColorCopDlg::OnOptionsAlwaysontop() {
    m_Appflags ^= AlwaysOnTop;
    ToggleOnTop(true);
}

void CColorCopDlg::ToggleOnTop(bool bSetStatusbartext) {
    const bool makeTopMost = (m_Appflags & AlwaysOnTop) != 0;

    if (bSetStatusbartext) {
        SetStatusBarText(IDS_ALWAYSOTOP, makeTopMost ? 1 : 2);
    }

    // Unified Win32 call — no MFC wndTopMost wrapper
    ::SetWindowPos(
        GetSafeHwnd(),
        makeTopMost ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);

    // Sync system menu checkbox
    if (CMenu* pSysMenu = GetSystemMenu(FALSE)) {
        pSysMenu->CheckMenuItem(
            IDM_ALWAYSONTOP,
            makeTopMost ? MF_CHECKED : MF_UNCHECKED);
    }
}

// Dialog-based MFC applications do NOT automatically update menu UI state
// (checkmarks, radio items, enable/disable) the way frame windows do.
// CFrameWnd has a full command routing loop that calls CCmdUI::DoUpdate()
// for each menu item, but CDialog does not.
//
// Because ColorCop is a dialog-based app with a menu, we must manually
// iterate the menu items and invoke DoUpdate() so that ON_UPDATE_COMMAND_UI
// handlers (e.g., for checkboxes and toggles) are actually called.
//
// Without this loop, menu items will NOT reflect the current application
// state when the user opens a menu.
void CColorCopDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu) {
    // Preserve default MFC behavior (system menu handling, owner-draw, etc.)
    CDialog::OnInitMenuPopup(pMenu, nIndex, bSysMenu);

    // Manually trigger UI updates for each menu item.
    // This restores the behavior normally provided by CFrameWnd.
    CCmdUI cmdUI;
    cmdUI.m_pMenu = pMenu;
    cmdUI.m_nIndexMax = pMenu->GetMenuItemCount();

    for (UINT i = 0; i < cmdUI.m_nIndexMax; ++i) {
        cmdUI.m_nIndex = i;
        cmdUI.m_nID = pMenu->GetMenuItemID(i);
        cmdUI.DoUpdate(this, FALSE);
    }
}

void CColorCopDlg::OnUpdateOptionsAutocopytoclipboard(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & AutoCopytoClip);
}

void CColorCopDlg::OnUpdateOptionsAlwaysontop(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & AlwaysOnTop);
}

void CColorCopDlg::OnOptionsAutocopytoclipboard() {
    m_Appflags ^= AutoCopytoClip;
    OnCopytoclip();
}

void CColorCopDlg::OnColorRandom() {
    // Generates a random color and updates
    // current decimal value MOD 256 - make a random value from 0 to 255
    // Thread-safe random number generation
    static thread_local std::mt19937 generator(std::random_device{}());  // NOLINT
    std::uniform_int_distribution<int> distribution(RGB_MIN, RGB_MAX);

    m_Reddec = distribution(generator);
    m_Greendec = distribution(generator);
    m_Bluedec = distribution(generator);

    SetStatusBarText(IDS_RANDOMCOLOR, 0);
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnColorReverse() {
    // Invert the current RGB color (per‑channel): new = 255 - old.
    // This produces the photographic "negative" of the selected color.
    SetStatusBarText(IDS_REVERSECOLOR, 0);

    m_Reddec = RGB_MAX - m_Reddec;
    m_Greendec = RGB_MAX - m_Greendec;
    m_Bluedec = RGB_MAX - m_Bluedec;

    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupColorConverttograyscale() {
    CString strStatus;

    // Converts the current color to grayscale.
    //
    // If R, G, and B are already equal, the color is already gray.
    if ((m_Reddec == m_Greendec) && (m_Greendec == m_Bluedec)) {
        strStatus.LoadString(IDS_COLOR_GRAY);
        SetStatusBarText(strStatus);
    } else {
        strStatus.LoadString(IDS_COLOR_CONVERT_GRAY);
        SetStatusBarText(strStatus);

        //
        // grayscale values have identical R, G, B values.
        //
        // This implementation uses the "lightness" definition from HSL:
        //     L = (max(R,G,B) + min(R,G,B)) / 2
        //
        // This picks the midpoint on the neutral axis between the darkest
        // and brightest channel. After this desaturation, the RGB channels
        // all receive the same value, producing a grayscale color.
        //

        // Compute channel extrema in int.
        const int Max = std::max({m_Reddec, m_Greendec, m_Bluedec});
        const int Min = std::min({m_Reddec, m_Greendec, m_Bluedec});

        // Compute grayscale lightness in int.
        const int L = (Min + Max) / 2;

        // Apply grayscale value to all channels.
        m_Reddec = L;
        m_Greendec = L;
        m_Bluedec = L;

        // Push updated values to UI and recalc dependent fields.
        UpdateData(false);
        OnconvertRGB();
        OnCopytoclip();
    }
    return;
}

void CColorCopDlg::OnFileAbout() {
    CAboutDlg dlg;
    dlg.DoModal();
}

void CColorCopDlg::OnColorSnaptowebsafe() {
    m_Appflags ^= SnaptoWebsafe;

    if (m_Appflags & SnaptoWebsafe) {
        m_Appflags &= ~DetectWebsafeColors;  // shut off Websafe Detection
        SetStatusBarText(IDS_SNAPTOWEBSAFE, 1);
    } else {
        SetStatusBarText(IDS_SNAPTOWEBSAFE, 2);
    }

    if (!(m_Appflags & SnaptoWebsafe) && m_oldColorSaved) {
        // Snap-to-Websafe was just turned OFF and we have a saved pre‑snap color.
        // Restore the original RGB values that were backed up when snapping was enabled.
        m_Reddec = m_OldRed;
        m_Greendec = m_OldGreen;
        m_Bluedec = m_OldBlue;
    }
    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnUpdateColorSnaptowebsafe(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & SnaptoWebsafe);
}

void CColorCopDlg::TestForWebsafe() {
    // When Snap‑to‑Websafe is enabled, preserve the user's original RGB values
    // so they can be restored if the user later disables snapping. Web‑safe
    // colors are defined as multiples of 51 (0, 51, 102, 153, 204, 255).
    if (m_Appflags & SnaptoWebsafe) {
        m_oldColorSaved = true;

        // Backup the actual RGB values before snapping
        m_OldRed = m_Reddec;
        m_OldGreen = m_Greendec;
        m_OldBlue = m_Bluedec;

        // Snap each component to the nearest web‑safe value
        m_Reddec = DecimaltoWebsafe(m_Reddec);
        m_Greendec = DecimaltoWebsafe(m_Greendec);
        m_Bluedec = DecimaltoWebsafe(m_Bluedec);
    }
}

int CColorCopDlg::DecimaltoWebsafe(int originalDec) {
    // Convert an RGB component (0–255) to the nearest web‑safe value.
    // Web‑safe colors use steps of 51: {0, 51, 102, 153, 204, 255}.
    // We compute how far 'originalDec' is from the nearest step and
    // round down if the offset is small, otherwise round up.
    //
    // Example: 128 → offset 128 % 51 = 26 → round up to 153.

    const int offset = originalDec % WEBSAFE_STEP;

    if (offset == 0) {
        // Already exactly on a web‑safe boundary
        return originalDec;
    }

    // If we're closer to the lower boundary (offset < 25), round down.
    // Otherwise, round up to the next 51‑multiple.
    return (offset < WEBSAFE_STEP / 2)
               ? (originalDec - offset)
               : (originalDec + (WEBSAFE_STEP - offset));
}

void CColorCopDlg::OnOptionsOmitsymbol() {
    m_Appflags ^= OmitPound;
    if (m_Appflags & OmitPound)
        SetStatusBarText(IDS_OMITSYMBOL, 1);
    else
        SetStatusBarText(IDS_OMITSYMBOL, 2);

    FigurePound();
}

void CColorCopDlg::FigurePound() {
    // This function adds or removes characters from the hex edit control
    if (m_Appflags & OmitPound) {
        if (m_Appflags & ModeHTML) {
            if (m_Hexcolor.Left(1) == _T("#"))  // remove the # is it exists
                m_Hexcolor.Delete(0);
        } else if (m_Appflags & ModeDelphi) {
            if (m_Hexcolor.Left(1) == _T("$"))
                m_Hexcolor.Delete(0);
        }
    } else {
        if (m_Appflags & ModeHTML) {
            // if the user is switching to omit the pound, we need to add it if it doesn't exist
            if (m_Hexcolor.Left(1) != _T("#")) {
                m_Hexcolor = _T("#") + m_Hexcolor;
            }
        } else if (m_Appflags & ModeDelphi) {
            if (m_Hexcolor.Left(1) != _T("$"))
                m_Hexcolor = _T("$") + m_Hexcolor;
        }
    }
    UpdateData(false);
    // update changes to the edit control (m_Hexcolor)
}

void CColorCopDlg::OnUpdateOptionsOmitsymbol(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck((m_Appflags & OmitPound) ? 1 : 0);
}

void CColorCopDlg::OnExpandDialog() {
    m_Appflags ^= ExpandedDialog;

    if (m_Appflags & ExpandedDialog) {
        SetStatusBarText(IDS_EXPANDEDDIALOG, 1);
    }

    TestForExpand();
}

void CColorCopDlg::TestForExpand() {
    RECT currect;
    GetWindowRect(&currect);

    if (m_Appflags & ExpandedDialog) {
        currect.right = currect.left + lgWidth;
        currect.bottom = currect.top + lgHeight;

        m_ExpandDialog.SetWindowText(_T("&<<"));

        m_Magnifier.ShowWindow(TRUE);

        MoveWindow(&currect);

    } else {
        // use smWidth and smHeight
        m_Magnifier.ShowWindow(FALSE);
        currect.right = currect.left + smWidth;
        currect.bottom = currect.top + smHeight;
        MoveWindow(&currect);

        m_ExpandDialog.SetWindowText(_T("&>>"));
    }
    InvalidateRect(&currect, false);  // go redraw... but don't erase or it will flicker
}

void CColorCopDlg::OnOptionsMinimizetosystray() {
    m_Appflags ^= MinimizetoTray;

    if (m_Appflags & MinimizetoTray)
        SetStatusBarText(IDS_MINIMIZETOTRAY, 1);
    else
        SetStatusBarText(IDS_MINIMIZETOTRAY, 2);
    return;
}

void CColorCopDlg::OnUpdateOptionsMinimizetosystray(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & MinimizetoTray);
}

void CColorCopDlg::OnOptionsUppercasehex() {
    m_Appflags ^= UpperCaseHex;

    if (m_Appflags & UpperCaseHex)
        SetStatusBarText(IDS_UPPERHEX, 1);
    else
        SetStatusBarText(IDS_UPPERHEX, 2);

    TestForUpperHex();
}

void CColorCopDlg::OnUpdateOptionsUppercasehex(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & UpperCaseHex);
}

void CColorCopDlg::TestForUpperHex() {
    // Number of prefix characters to skip before normalizing case.
    // Prefixes like 0x, #, and $ are not part of the hex digits.
    int start = 0;

    if (m_Hexcolor.Left(2).CompareNoCase(_T("0x")) == 0)
        start = 2;
    else if (m_Hexcolor.Left(1) == _T('#') || m_Hexcolor.Left(1) == _T('$'))
        start = 1;

    CString payload = m_Hexcolor.Mid(start);

    if (m_Appflags & UpperCaseHex)
        payload.MakeUpper();
    else
        payload.MakeLower();

    m_Hexcolor = m_Hexcolor.Left(start) + payload;

    SetDlgItemText(IDC_HEXCOLOR, m_Hexcolor);
}

void CColorCopDlg::OnRButtonDown(UINT nFlags, CPoint point) {
    CWnd* pWnd = ChildWindowFromPoint(point);

    if (m_isMagnifying) {  // right clicked while magnifying
        if (m_MagLevel != kMaxZoom) {
            m_MagLevel++;
        }
        return;
    } else if (m_isEyedropping) {  // right clicked while eydropping
        AdvanceColorHistory();     // push color to color history
        return;
    } else if (pWnd && pWnd->GetSafeHwnd() == m_ColorPalette.GetSafeHwnd()) {
        AdvanceColorHistory();  // push color to color history
        return;
    }

    CMenu tempMenu;
    tempMenu.LoadMenu(IDR_COPMENU);

    ClientToScreen(&point);
    CMenu* pPopup = tempMenu.GetSubMenu(0);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           point.x, point.y,
                           this, NULL);


    CDialog::OnRButtonDown(nFlags, point);
}

void CColorCopDlg::OnRButtonUp(UINT nFlags, CPoint point) {
    CDialog::OnRButtonUp(nFlags, point);
}

void CColorCopDlg::ChangeTo1pixelSampling() {
    // Only change if we're not already in 1‑pixel mode
    if ((m_Appflags & Sampling1) == 0) {
        // Clear all sampling modes, then enable 1‑pixel
        m_Appflags &= ~(Sampling3x3 | Sampling5x5 | SamplingMULTI);
        m_Appflags |= Sampling1;

        // Update cursor
        if (CWinApp* pApp = AfxGetApp())
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER));
    }

    SetStatusBarText(IDS_1PIXEL, 0);
}

void CColorCopDlg::ChangeTo3x3Sampling() {
    // Only change if we're not already in 3x3 mode
    if ((m_Appflags & Sampling3x3) == 0) {
        // Clear all sampling modes, then enable 3x3
        m_Appflags &= ~(Sampling1 | Sampling5x5 | SamplingMULTI);
        m_Appflags |= Sampling3x3;

        // Update cursor
        if (CWinApp* pApp = AfxGetApp())
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_3X3));
    }

    // 3×3 → offset = 1
    m_iSamplingOffset = 1;

    SetStatusBarText(IDS_3PIXEL, 0);
}

void CColorCopDlg::ChangeTo5x5Sampling() {
    // Only change if we're not already in 5x5 mode
    if ((m_Appflags & Sampling5x5) == 0) {
        // Clear all sampling modes, then enable 5x5
        m_Appflags &= ~(Sampling1 | Sampling3x3 | SamplingMULTI);
        m_Appflags |= Sampling5x5;

        // Update cursor
        if (CWinApp* pApp = AfxGetApp())
            m_EyeLoc.SetIcon(pApp->LoadCursor(IDC_EYEDROPPER_5X5));
    }

    // 5×5 → offset = 2
    m_iSamplingOffset = 2;

    SetStatusBarText(IDS_5PIXEL, 0);
}


void CColorCopDlg::OnUpdatePopupSampling1pixel(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & Sampling1);
}

void CColorCopDlg::OnUpdatePopupSampling5by5average(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & Sampling5x5);
}

void CColorCopDlg::OnUpdatePopupSampling3by3average(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & Sampling3x3);
}

// Compute the average RGB value of a square region centered on `point`.
bool CColorCopDlg::AveragePixelArea(HDC hdc, int* m_R, int* m_G, int* m_B, CPoint point) {
    // Compute the average RGB value of a square region centered on `point`.
    // The region size is (2 * m_iSamplingOffset + 1) on each side.
    int64_t reddec = 0, greendec = 0, bluedec = 0;  // 64-bit to safely accumulate many pixel values
    int offset = m_iSamplingOffset;
    int elements = 0;  // count only valid pixels

    COLORREF crefxy;
    int xrel, yrel;

    // Compute unclamped bounds
    int xmin = point.x - offset;
    int ymin = point.y - offset;

    // Clamp lower bounds manually (avoids std::max(int, uint16_t) ambiguity)
    if (xmin < 0)
        xmin = 0;
    if (ymin < 0)
        ymin = 0;

    // Walk the sampling region and accumulate RGB components.
    for (xrel = xmin; xrel <= point.x + offset; xrel++) {
        for (yrel = ymin; yrel <= point.y + offset; yrel++) {
            crefxy = ::GetPixel(hdc, xrel, yrel);

            // Skip invalid pixels; GetPixel can fail near screen edges or on invalid DCs.
            if (crefxy == CLR_INVALID)
                continue;

            reddec += GetRValue(crefxy);
            greendec += GetGValue(crefxy);
            bluedec += GetBValue(crefxy);
            ++elements;  // count only valid samples
        }
    }

    if (elements == 0) {
        // No valid pixels sampled; treat as unchanged.
        return true;
    }

    // Convert accumulated totals into average RGB values.
    reddec /= elements;
    greendec /= elements;
    bluedec /= elements;

    // Check whether the averaged color differs from the previously stored sample.
    // This avoids unnecessary updates when the sampled color hasn't changed.
    if (reddec != m_Reddec || greendec != m_Greendec || bluedec != m_Bluedec) {
        *m_R = static_cast<int>(reddec);
        *m_G = static_cast<int>(greendec);
        *m_B = static_cast<int>(bluedec);
        return false;  // color changed
    }

    return true;  // color unchanged
}

void CColorCopDlg::OnPopupApplicationExpandeddialog() {
    OnExpandDialog();
}

void CColorCopDlg::OnUpdatePopupApplicationExpandeddialog(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & ExpandedDialog);
}

void CColorCopDlg::OnUpdateViewHtmlhexmode(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModeHTML);
}

void CColorCopDlg::OnUpdateOptionsDelphimode(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModeDelphi);
}

void CColorCopDlg::OnUpdatePopupHexmodePowerbuilder(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModePowerBuilder);
}

void CColorCopDlg::OnUpdatePopupModeVisualbasichex(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModeVisualBasic);
}

void CColorCopDlg::OnUpdatePopupModeVisualchex(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModeVisualC);
}
void CColorCopDlg::OnUpdatePopupModeRgbfloat(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & RGBFLOAT);
}

void CColorCopDlg::OnUpdatePopupModeRgbint(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & RGBINT);
}

void CColorCopDlg::OnUpdatePopupModeClarionhex(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & ModeClarion);
}

void CColorCopDlg::OnPopupModeRgbfloat() {
    SetStatusBarText(IDS_MODE_RGBFLOAT, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set RGB float mode
    m_Appflags |= RGBFLOAT;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeRgbint() {
    SetStatusBarText(IDS_MODE_RGBINT, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set RGB integer mode
    m_Appflags |= RGBINT;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnViewHtmlhexmode() {
    SetStatusBarText(IDS_MODE_HTML, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set HTML mode
    m_Appflags |= ModeHTML;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnOptionsDelphimode() {
    SetStatusBarText(IDS_MODE_DELPHI, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set Delphi mode
    m_Appflags |= ModeDelphi;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupHexmodePowerbuilder() {
    SetStatusBarText(IDS_MODE_POWERBUILDER, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set Powerbuilder mode
    m_Appflags |= ModePowerBuilder;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeVisualbasichex() {
    SetStatusBarText(IDS_MODE_VISUALB, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set Visual Basic mode
    m_Appflags |= ModeVisualBasic;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeVisualchex() {
    SetStatusBarText(IDS_MODE_VISUALC, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set Visual C++ mode
    m_Appflags |= ModeVisualC;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupModeClarionhex() {
    SetStatusBarText(IDS_MODE_CLARION, 1);

    // Clear all other modes
    m_Appflags &= ~kAllColorModes;
    // Set Clarion mode
    m_Appflags |= ModeClarion;

    OnconvertRGB();
    OnCopytoclip();
}

void CColorCopDlg::OnPopupRestore() {
    m_bvisible = true;

    ShowWindow(SW_RESTORE);  // user wants to restore from systray
    bMinimized = false;      // remove the systray icon
    SetupTrayIcon();
    SetupTaskBarButton();  // add taskbar button
}

void CColorCopDlg::OnPopupExit() {
    bMinimized = false;  // user wants to exit color cop
    SetupTrayIcon();     // remove the systray icon
    EndDialog(IDOK);
}

void CColorCopDlg::SetStatusBarText(LPCTSTR statusText) {
    m_StatusBar.SetPaneText(0, statusText);
}

void CColorCopDlg::SetStatusBarText(UINT strResource, int toggleVal) {
    CString text;
    text.LoadString(strResource);

    if (toggleVal == 1) {
        CString on;
        on.LoadString(IDS_ON);
        text += _T(" ") + on;
    } else if (toggleVal == 2) {
        CString off;
        off.LoadString(IDS_OFF);
        text += _T(" ") + off;
    }

    m_StatusBar.SetPaneText(0, text);
}


BOOL CColorCopDlg::OnMouseWheel(UINT nFlags, int16_t zDelta, CPoint pt) {
    // first check if we are magnifying...

    if (m_isEyedropping) {
        return TRUE;
    } else if (m_isMagnifying) {
        m_MagLevel += zDelta / WHEEL_DELTA;
        m_MagLevel = std::clamp<int>(m_MagLevel, kMinZoom, kMaxZoom);
        return TRUE;
    }

    // determine which edit control has the focus??

    CWnd* curfocus = GetFocus();  // handle to the window with focus

    if (curfocus == NULL)  // There is nothing with the focus
        return TRUE;       // jump out

    int offset = 1;  // default

    if (nFlags == MK_CONTROL)  // jump by 5 if control is down
        offset = 5;
    else if (nFlags == MK_SHIFT)  // jump by 2 if shift is down
        offset = 2;

    // if bSnaptoWebsafe is on then incrementing by 1, 5 or 2 won't do any good
    // because it will just snap lower.

    if (m_Appflags & SnaptoWebsafe) {
        // therefore, increment or decrement by 51
        offset = WEBSAFE_STEP;
    }

    if (curfocus == GetDlgItem(IDC_RED)) {  // red has focus
        m_Reddec += zDelta / WHEEL_DELTA * offset;
        m_Reddec = std::clamp(m_Reddec, RGB_MIN, RGB_MAX);

    } else if (curfocus == GetDlgItem(IDC_GREEN)) {  // green has focus
        m_Greendec += zDelta / WHEEL_DELTA * offset;
        m_Greendec = std::clamp(m_Greendec, RGB_MIN, RGB_MAX);
    } else if (curfocus == GetDlgItem(IDC_BLUE)) {  // blue has focus
        m_Bluedec += zDelta / WHEEL_DELTA * offset;
        m_Bluedec = std::clamp(m_Bluedec, RGB_MIN, RGB_MAX);
    } else {          // there is focus, but it's not on either the Red,
        return TRUE;  // Green, or Blue edit controls -> jump out
    }

    UpdateData(false);
    CalcColorPal();
    OnconvertRGB();
    OnCopytoclip();

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CColorCopDlg::OnTimer(UINT nIDEvent) {
    // this allows animations to be magnified and eyedropped
    CString strStatus;
    switch (nIDEvent) {
    case 1:

        if ((m_isMagnifying) || (m_isEyedropping)) {
            POINT point;

            if (GetCursorPos(&point)) {
                ScreenToClient(&point);
                PostMessage(WM_MOUSEMOVE, HTCAPTION, MAKELPARAM(point.x, point.y));
            }
        }
        break;
    case 2:

        if (m_isMagMinusDown) {
            if (m_MagLevel != kMinZoom) {
                m_MagLevel--;
            }

            strStatus.LoadString(IDS_MAG_DECREASED);
            strStatus.Format(strStatus, m_MagLevel);
            SetStatusBarText(strStatus);

            RecalcZoom();

        } else if (m_isMagPlusDown) {
            if (m_MagLevel != kMaxZoom) {
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

void CColorCopDlg::OnPopupApplicationHelp() {
    CString helpPath;
    GetModuleFileName(NULL, helpPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
    helpPath.ReleaseBuffer();
    PathRemoveFileSpec(helpPath.GetBuffer());
    helpPath.ReleaseBuffer();
    helpPath += _T("\\ColorCop.chm");
    // Use ShellExecute with hh.exe instead of HtmlHelp:
    // - HtmlHelp is deprecated and unreliable on modern Windows
    // - Ensures the .chm opens correctly regardless of installation path
    ShellExecute(NULL, _T("open"), _T("hh.exe"), helpPath, NULL, SW_SHOWNORMAL);
}

void CColorCopDlg::OnPopupColorDetectwebsafe() {
    m_Appflags ^= DetectWebsafeColors;
    if (m_Appflags & DetectWebsafeColors) {
        m_Appflags &= ~SnaptoWebsafe;  // shut off snap to websafe
        SetStatusBarText(IDS_DETECTWEBSAFE, 1);
    } else {
        SetStatusBarText(IDS_DETECTWEBSAFE, 2);
    }
}

void CColorCopDlg::OnUpdatePopupColorDetectwebsafe(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & DetectWebsafeColors);
}

void CColorCopDlg::OnPopupApplicationEasymove() {
    m_Appflags ^= EasyMove;
    if (m_Appflags & EasyMove) {
        SetStatusBarText(IDS_EASYMOVE, 1);
    } else {
        SetStatusBarText(IDS_EASYMOVE, 2);
    }
}

void CColorCopDlg::OnUpdatePopupApplicationEasymove(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & EasyMove);
}

void CColorCopDlg::OnPopupApplicationMinimizetosystemtrayonstart() {
    m_Appflags ^= MinimizeOnStart;

    if (m_Appflags & MinimizeOnStart) {
        SetStatusBarText(IDS_MINIMIZEONSTART, 1);
    } else {
        SetStatusBarText(IDS_MINIMIZEONSTART, 2);
    }
}

void CColorCopDlg::OnUpdatePopupApplicationMinimizetosystemtrayonstart(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & MinimizeOnStart);
}


void CColorCopDlg::OnPopupApplicationAllowmultipleinstances() {
    m_Appflags ^= MultipleInstances;
    SetStatusBarText(IDS_RESTART_COLORCOP, 0);
    return;
}

void CColorCopDlg::OnUpdatePopupApplicationAllowmultipleinstances(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & MultipleInstances);
}

void CColorCopDlg::OnPopupOptionsMagnifywhileeyedropping() {
    m_Appflags ^= MAGWHILEEYEDROP;
}

void CColorCopDlg::OnUpdatePopupOptionsMagnifywhileeyedropping(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & MAGWHILEEYEDROP);
}

void CColorCopDlg::OnPopupOptionsUsecrosshaircursor() {
    CWinApp* pApp = AfxGetApp();  // get a pointer to the one and only CWinApp
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

void CColorCopDlg::OnUpdatePopupOptionsUsecrosshaircursor(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & USECROSSHAIR);
}


HCURSOR CColorCopDlg::OnQueryDragIcon() {
    // The system calls this to obtain the cursor to display while the user drags
    //  the minimized window.

    return (HCURSOR) m_hIcon;
}

void CColorCopDlg::OnCaptureChanged(CWnd* pWnd) {
    // This is called when the app has lost capture

    if ((m_isEyedropping) || (m_isMagnifying)) {
        StopCapture();
    }
    CDialog::OnCaptureChanged(pWnd);
}

void CColorCopDlg::FireOptionMenu() {
    // Pop up the system menu when a user hits SHIFT + F10 (right click equivalent)
    // suggested by that dude from PC Mag

    CPoint targetp = {0, 0};

    CMenu tempMenu;
    tempMenu.LoadMenu(IDR_COPMENU);

    ClientToScreen(&targetp);
    CMenu* pPopup = tempMenu.GetSubMenu(0);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           targetp.x, targetp.y, this, NULL);
}


PBITMAPINFO CColorCopDlg::CreateBitmapInfoStruct(HWND /*hwnd*/, HBITMAP hBmp) {
    BITMAP bmp = {};
    if (!GetObject(hBmp, sizeof(bmp), &bmp)) {
        AfxMessageBox(_T("Unable to receive bitmap information"));
        return nullptr;
    }

    // Compute effective color depth (clamped to standard buckets)
    WORD cClrBits = static_cast<WORD>(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits <= 1)
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

    // Compute palette size safely (avoid 1 << cClrBits overflow)
    DWORD clrUsed = 0;
    if (cClrBits < 24) {
        // max palette entries for <= 8bpp is 256; for 16/32bpp it's 0
        clrUsed = (cClrBits <= 8) ? (1u << cClrBits) : 0u;
    }

    // Compute allocation size
    const SIZE_T headerSize = sizeof(BITMAPINFOHEADER);
    const SIZE_T paletteSize = clrUsed * sizeof(RGBQUAD);

    // Overflow check (defensive)
    if (paletteSize / sizeof(RGBQUAD) != clrUsed) {
        return nullptr;  // overflow detected
    }

    const SIZE_T totalSize = headerSize + paletteSize;

    PBITMAPINFO pbmi = static_cast<PBITMAPINFO>(LocalAlloc(LPTR, totalSize));
    if (!pbmi) {
        return nullptr;
    }

    // Initialize BITMAPINFOHEADER
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiHeader.biClrUsed = clrUsed;

    // Compute image size (DWORD-aligned scanlines)
    const DWORD bitsPerLine = bmp.bmWidth * cClrBits;
    const DWORD alignedBits = (bitsPerLine + 31u) & ~31u;
    pbmi->bmiHeader.biSizeImage = (alignedBits / 8u) * bmp.bmHeight;

    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
}

void CColorCopDlg::CreateBMPFile(HWND /*hwnd*/, LPTSTR pszFile,
                                 PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) {
    HANDLE hf;               // file handle
    BITMAPFILEHEADER hdr;    // bitmap file-header
    PBITMAPINFOHEADER pbih;  // bitmap info-header
    LPBYTE lpBits;           // memory pointer
    DWORD dwTotal;           // total count of bytes
    DWORD cb;                // incremental count of bytes
    BYTE* hp;                // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) {
        AfxMessageBox(_T("GlobalAlloc failed"));
        return;
    }

    // Retrieve the color table (RGBQUAD array) and the bits
    // (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,
                   DIB_RGB_COLORS)) {
        AfxMessageBox(_T("GetDIBits failed"));
        return;
    }

    // Create the .BMP file.
    hf = CreateFile(pszFile,
                    GENERIC_READ | GENERIC_WRITE,
                    static_cast<DWORD>(0),
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE) {
        TRACE(_T("Warning: Could not create BMP file: %s (error=%lu)\n"), pszFile, GetLastError());
        return;
    }

    hdr.bfType = 0x4d42;  // 0x42 = "B" 0x4d = "M"

    // Compute the size of the entire file.
    hdr.bfSize = static_cast<DWORD>((sizeof(BITMAPFILEHEADER)) +
                                    pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.
    hdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) +
                    pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),
                   (LPDWORD) &dwTmp, NULL)) {
        AfxMessageBox(_T("WriteFile failed"));
        return;
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD),
                   (LPDWORD) &dwTmp, (NULL))) {
        AfxMessageBox(_T("WriteFile failed"));
        return;
    }

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, static_cast<int>(cb), (LPDWORD) &dwTmp, NULL)) {
        AfxMessageBox(_T("WriteFile failed"));
    }
    // Close the .BMP file.
    if (!CloseHandle(hf)) {
        AfxMessageBox(_T("Could not close BMP file"));
    }
    // Free memory.
    GlobalFree((HGLOBAL) lpBits);
}

void CColorCopDlg::InitDarkModeAPIs() {
    HMODULE hUxTheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!hUxTheme)
        return;

    _ShouldAppsUseDarkMode =
        reinterpret_cast<ShouldAppsUseDarkMode_t>(
            GetProcAddress(hUxTheme, MAKEINTRESOURCEA(132)));  // ordinal 132

    _AllowDarkModeForWindow =
        reinterpret_cast<AllowDarkModeForWindow_t>(
            GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133)));  // ordinal 133

    _SetPreferredAppMode =
        reinterpret_cast<SetPreferredAppMode_t>(
            GetProcAddress(hUxTheme, MAKEINTRESOURCEA(135)));  // ordinal 135
}


bool CColorCopDlg::isWebsafeColor(int R, int G, int B) {
    // WebSafe colors have each channel as a multiple of WEBSAFE_STEP (51)
    return (R % WEBSAFE_STEP == 0) &&
           (G % WEBSAFE_STEP == 0) &&
           (B % WEBSAFE_STEP == 0);
}

void CColorCopDlg::OnUpdatePopupOptionsStartcursoroneyedropper(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & SETCURSORONEYEDROP);
}

void CColorCopDlg::OnPopupOptionsStartcursoroneyedropper() {
    m_Appflags ^= SETCURSORONEYEDROP;
    SetStatusBarText(IDS_RESTART_COLORCOP, 0);
    return;
}

void CColorCopDlg::OnPopupSamplingDecreasemultipixelaverage() {
    m_Appflags &= ~Sampling1;
    m_Appflags &= ~Sampling3x3;
    m_Appflags &= ~Sampling5x5;
    m_Appflags |= SamplingMULTI;

    CString strStatus;

    (void) strStatus.LoadString(IDS_MULTIPIX_SET);

    if (m_iSamplingOffset > MULTIPIX_MIN) {
        m_iSamplingOffset--;
        strStatus.Format(strStatus, m_iSamplingOffset * 2 + 1,
                         m_iSamplingOffset * 2 + 1);
    } else {
        (void) strStatus.LoadString(IDS_MULTIPIX_LIMIT);
        strStatus.Format(strStatus, MULTIPIX_MIN * 2 + 1, MULTIPIX_MAX * 2 + 1);
    }

    SetStatusBarText(strStatus);
}

void CColorCopDlg::OnPopupSamplingIncreasemultipixelaverage() {
    m_Appflags &= ~Sampling1;
    m_Appflags &= ~Sampling3x3;
    m_Appflags &= ~Sampling5x5;
    m_Appflags |= SamplingMULTI;

    CString strStatus = "";
    strStatus.LoadString(IDS_MULTIPIX_SET);

    if (m_iSamplingOffset < MULTIPIX_MAX) {
        m_iSamplingOffset++;
        strStatus.Format(strStatus, m_iSamplingOffset * 2 + 1, m_iSamplingOffset * 2 + 1);
    } else {
        strStatus.LoadString(IDS_MULTIPIX_LIMIT);
        strStatus.Format(strStatus, MULTIPIX_MIN * 2 + 1, MULTIPIX_MAX * 2 + 1);
    }
    SetStatusBarText(strStatus);
}

void CColorCopDlg::OnPopupSamplingMultipixel() {
    if (m_Appflags ^ SamplingMULTI) {
        m_Appflags &= ~Sampling1;
        m_Appflags &= ~Sampling3x3;
        m_Appflags &= ~Sampling5x5;
        m_Appflags |= SamplingMULTI;
    }
    SetStatusBarText(IDS_MULTIPIX, 0);
}

void CColorCopDlg::OnUpdatePopupSamplingMultipixel(CCmdUI* pCmdUI) {
    pCmdUI->SetRadio(m_Appflags & SamplingMULTI);
}

void CColorCopDlg::OnPopupSpaceRgb() {
    if (m_Appflags ^ SpaceRGB) {
        m_Appflags &= ~SpaceCMYK;
        m_Appflags |= SpaceRGB;
    }
    ChangeColorSpace(true);
    SetStatusBarText(IDS_SPACE_RGB, 1);
}

void CColorCopDlg::OnUpdatePopupSpaceRgb(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & SpaceRGB);
}

void CColorCopDlg::OnPopupSpaceCmyk() {
    if (m_Appflags ^ SpaceCMYK) {
        m_Appflags &= ~SpaceRGB;
        m_Appflags |= SpaceCMYK;
    }
    ChangeColorSpace(false);
    SetStatusBarText(IDS_SPACE_CMYK, 1);
}

void CColorCopDlg::OnUpdatePopupSpaceCmyk(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_Appflags & SpaceCMYK);
}

void CColorCopDlg::ChangeColorSpace(bool bRGB) {
    CString txt;

    if (bRGB) {
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

HBRUSH CColorCopDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    if (!(m_Appflags & DarkMode))
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (!::IsWindow(pWnd->GetSafeHwnd()))
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (nCtlColor) {
    case CTLCOLOR_DLG:
    case CTLCOLOR_STATIC:
    case CTLCOLOR_EDIT:
        pDC->SetBkColor(RGB(32, 32, 32));
        pDC->SetTextColor(m_clrText);
        return (HBRUSH) m_brDarkMode.GetSafeHandle();
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


BOOL CColorCopDlg::OnEraseBkgnd(CDC* pDC) {
    if (m_Appflags & DarkMode) {
        CRect rc;
        GetClientRect(&rc);
        pDC->FillSolidRect(&rc, RGB(32, 32, 32));
        return TRUE;
    }

    return CDialog::OnEraseBkgnd(pDC);
}
