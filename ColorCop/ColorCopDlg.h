// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCopDlg.h : header file for the Color Cop dialog
//

#pragma once

#include <cstdint>

#include "Defaults.h"
#include "DarkStatusBar.h"

constexpr double HUE_ROTATION_STEP = 60.0 / 360.0;  // one-sixth of the color wheel
constexpr double SAT_LIGHT_SHIFT = 0.15;            // amount to adjust saturation/lightness
constexpr int NUM_SWATCHES = 6;                     // number of hue variants
constexpr int NUM_PALETTE_COLUMNS = 7;

constexpr double HSL_MIN = 0.0;   // minimum possible HSL component value
constexpr double HSL_MAX = 1.0;   // maximum possible HSL component value

// RGB domain (double precision version of 0–255 range)
// Used when normalizing integer RGB values into floating‑point color space.
constexpr double RGB_MAX_D = 255.0;

constexpr double HUE_SECTOR_GREEN = 2.0;  // hue offset when green is the max channel
constexpr double HUE_SECTOR_BLUE  = 4.0;  // hue offset when blue is the max channel

// Number of hue sectors in the HSL/HSV color wheel.
// Used to normalize hue into the 0–1 range after computing sector offsets.
constexpr double HUE_CYCLE = 6.0;

// Floating‑point epsilon for detecting grayscale (zero chroma).
// Prevents false negatives caused by tiny rounding errors when channels are equal.
constexpr double HSL_EPSILON = DBL_EPSILON;

#define BMP_FILE "\\Color_Cop.bmp"
#define BMP_FILE_DIR "\\ColorCop"

// Centralized list of all color output modes. Update this mask whenever a new
// mode is added so mode‑switch handlers remain consistent and future‑proof.
static constexpr uint32_t kAllColorModes =
    ModeHTML |
    ModeDelphi |
    ModePowerBuilder |
    ModeVisualBasic |
    ModeVisualC |
    ModeClarion |
    RGBFLOAT |
    RGBINT;

class CSystemTray;

class CColorCopDlg : public CDialog {
    // Construction
     public:
    explicit CColorCopDlg(CWnd* pParent = nullptr);  // standard constructor
    COLORREF ColorHistory[kHistoryCount];
    COLORREF CustColorBank[kCustomColorCount];
    // 64 bit application flags bitmask. Each bit represents a different user‑configurable setting or mode.
    uint64_t m_Appflags;
    int m_iSamplingOffset;

    int WinLocX;
    int WinLocY;
    int m_MagLevel = kDefaultMagLevel;
    int m_FloatPrecision = kDefaultFloatPrecision;

    HBITMAP hBitmap, hBitmapClip, hZoomBitmap;

    // Dialog Data
    //{{AFX_DATA(CColorCopDlg) // NOLINT(whitespace/comments)
    enum {
        IDD = IDD_HTMLCOP_DIALOG
    };
    CButton m_ColorPick;
    CButton m_ExpandDialog;

    CStatic m_Lbl4;
    CStatic m_Lbl3;
    CStatic m_Lbl2;
    CStatic m_Lbl1;
    CStatic m_Slide3;
    CStatic m_Slide2;
    CStatic m_Slide1;
    CStatic m_MagPlus;
    CStatic m_MagMinus;
    CStatic m_ColorPalette;
    CStatic m_MagWindow;
    CStatic m_Q7;
    CStatic m_Q6;
    CStatic m_Q5;
    CStatic m_Q4;
    CStatic m_Q3;
    CStatic m_Q2;
    CStatic m_Q1;
    CStatic m_ColorPreview;
    CStatic m_Magnifier;
    CStatic m_EyeLoc;
    CString m_Hexcolor;
    int m_Greendec;
    int m_Bluedec;
    int m_Reddec;
    int m_Black;
    int m_Cyan;
    int m_Magenta;
    int m_Yellow;
    //}}AFX_DATA // NOLINT(whitespace/comments)

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorCopDlg) // NOLINT(whitespace/comments)
     public:
    // Indicates whether ColorCop is running in portable mode. When enabled,
    // all settings are loaded from and saved to the executable directory
    // instead of the user's AppData folder. The flag is set by the app
    // during command‑line parsing and used by the dialog's persistence logic.
    bool m_PortableMode = false;
    virtual BOOL PreTranslateMessage(MSG* pMsg);

     protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
                                                      //}}AFX_VIRTUAL // NOLINT(whitespace/comments)

     protected:
    bool m_bvisible;

    HICON m_hIcon;
    HICON m_hBlank;
    HICON m_hEye;

    HACCEL m_hAcceleratorTable;

    int m_OldRed = 0;    // original red value before snapping to websafe
    int m_OldGreen = 0;  // original green value before snapping to websafe
    int m_OldBlue = 0;   // original blue value before snapping to websafe

    BOOL m_oldColorSaved;  // whether we have a saved pre‑snap color to restore from
    BOOL m_isEyedropping;
    BOOL m_isMagnifying;
    BOOL bMinimized;
    BOOL m_isMagPlusDown;
    BOOL m_isMagMinusDown;
    BOOL m_InitialMove;
    BOOL m_bCalcColorPal;
    BOOL bRelativePosition;
    BOOL m_MagDrop;

    CDarkStatusBar m_StatusBar;
    CToolTipCtrl m_ToolTip;

    CBrush m_brDarkMode;
    COLORREF m_clrText;

    int smHeight;
    int smWidth;
    int lgHeight;
    int lgWidth;
    int m_nwide;
    int m_ntall;

    // color palette stuff
    double r, g, b;
    double Hue, Sat, Light;

    struct swatchStruct {
        double A;
        double B;
        double C;
    } Swatch[NUM_SWATCHES];
    swatchStruct OrigSwatch;

    COLORREF ColorPal[NUM_SWATCHES][NUM_PALETTE_COLUMNS];
    int palcol{};

    POINT RelativePoint{};
    POINT RelativePointEnd{};

    CMenu copmenu;

    CRect Q1rect, Q2rect, Q3rect, Q4rect, Q5rect, Q6rect, Q7rect;
    CRect buttonrect;  // color window rect

    CRect magrect, colorpalrect;  // magnifier rect, based off color window rect
    CRect magplus, magminus;
    RECT CCopRect;

    HCURSOR m_hEyeCursor;
    HCURSOR m_hMagCursor;
    HCURSOR m_hStandardCursor;
    HCURSOR m_hMoveCursor;
    HCURSOR m_hHandCursor;

    double m_HSV_H;
    double m_HSV_S;
    double m_HSV_V;

    ////  systray Internal data
    HICON hIcon_;
    bool bMinimized_;
    CSystemTray* pTrayIcon_;
    int nTrayNotificationMsg_;
    // end
    void ComputeHSV(int r, int g, int b,
                    double& outH, double& outS, double& outV);

    void FloatPrecisionUp();
    void FloatPrecisionDown();
    void GetScreenBitmap(CPoint point);

    void StopCapture();
    void DisplayColor();
    void ParseHTML(CString inst);
    void ParseDelphi(CString inst);
    void ParseClarion(CString inst);
    void FigurePound();
    void TestForExpand();
    int DecimaltoWebsafe(int originalDec);
    void TestForUpperHex();
    void OnconvertHEX();
    void OnconvertRGB();
    void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
    void ToggleOnTop(bool bSetStatusbartext);
    void TestForWebsafe();
    void SetupTaskBarButton();
    void SetupTrayIcon();
    void SetupSystemMenu();
    void SetupWindowRects();
    bool LoadPersistentVariables();
    bool AveragePixelArea(HDC hdc, int* m_Reddec, int* m_Greendec, int* m_Bluedec, CPoint point);
    void SetStatusBarText(LPCTSTR statusText);
    void AdvanceColorHistory();
    void GetHistoryColor(int Cindex);
    HBITMAP CopyBitmap(HBITMAP hBitmapSrc);
    bool isWebsafeColor(int R, int G, int B);
    BOOL GetShellFolderPath(LPCTSTR pShellFolder, LPTSTR pShellPath);
    void SetStatusBarText(UINT strResource, int toggleVal);
    void CalcColorPal();
    double shiftHue(double hue);
    void setSeedColor();
    void RGBtoHSL(double R, double G, double B);
    void HSLtoRGB(double H, double S, double L);
    void setupSwatches();
    void handleShifts();
    void printSwatch();
    double plusValue(double num);
    double minusValue(double num);
    void RecalcZoom();
    void FireOptionMenu();
    PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
    void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
    void InitDarkModeAPIs();
    void UpdateCMYKFromRGB(int red, int green, int blue);
    void ChangeColorSpace(bool bRGB);

    // Generated message map functions
    //{{AFX_MSG(CColorCopDlg) // NOLINT(whitespace/comments)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnAbout();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnChangeGreen();
    afx_msg void OnChangeBlue();
    afx_msg void OnChangeRed();
    afx_msg void OnColorPick();
    afx_msg void OnCopytoclip();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnChangeHexcolor();
    afx_msg void OnDestroy();
    afx_msg void OnFileExit();
    afx_msg void OnColorReverse();
    afx_msg void OnOptionsAlwaysontop();
    afx_msg void OnUpdateOptionsAlwaysontop(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOptionsDelphimode(CCmdUI* pCmdUI);
    afx_msg void OnOptionsDelphimode();
    afx_msg void OnUpdateOptionsAutocopytoclipboard(CCmdUI* pCmdUI);
    afx_msg void OnOptionsAutocopytoclipboard();
    afx_msg void OnColorRandom();
    afx_msg void OnUpdateViewHtmlhexmode(CCmdUI* pCmdUI);
    afx_msg void OnExpandDialog();
    afx_msg void OnViewHtmlhexmode();
    afx_msg void OnFileAbout();
    afx_msg void OnColorSnaptowebsafe();
    afx_msg void OnOptionsOmitsymbol();
    afx_msg void OnUpdateOptionsOmitsymbol(CCmdUI* pCmdUI);
    afx_msg void OnUpdateColorSnaptowebsafe(CCmdUI* pCmdUI);
    afx_msg void OnOptionsMinimizetosystray();
    afx_msg void OnUpdateOptionsMinimizetosystray(CCmdUI* pCmdUI);
    afx_msg void OnOptionsUppercasehex();
    afx_msg void OnUpdateOptionsUppercasehex(CCmdUI* pCmdUI);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void ChangeTo1pixelSampling();
    afx_msg void OnUpdatePopupSampling1pixel(CCmdUI* pCmdUI);
    afx_msg void ChangeTo3x3Sampling();
    afx_msg void OnUpdatePopupSampling3by3average(CCmdUI* pCmdUI);
    afx_msg void ChangeTo5x5Sampling();
    afx_msg void OnUpdatePopupSampling5by5average(CCmdUI* pCmdUI);
    afx_msg void OnPopupApplicationExpandeddialog();
    afx_msg void OnUpdatePopupApplicationExpandeddialog(CCmdUI* pCmdUI);
    afx_msg void OnPopupHexmodePowerbuilder();
    afx_msg void OnUpdatePopupHexmodePowerbuilder(CCmdUI* pCmdUI);
    afx_msg void OnPopupModeVisualbasichex();
    afx_msg void OnUpdatePopupModeVisualbasichex(CCmdUI* pCmdUI);
    afx_msg void OnPopupModeVisualchex();
    afx_msg void OnUpdatePopupModeVisualchex(CCmdUI* pCmdUI);
    afx_msg void OnPopupRestore();
    afx_msg void OnPopupExit();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    void OnThemeSystem();
    void OnThemeLight();
    void OnThemeDark();
    void ApplyTheme();
    void OnUpdateThemeSystem(CCmdUI* pCmdUI);
    void OnUpdateThemeLight(CCmdUI* pCmdUI);
    void OnUpdateThemeDark(CCmdUI* pCmdUI);
    afx_msg BOOL OnMouseWheel(UINT nFlags, int16_t zDelta, CPoint pt);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPopupApplicationHelp();
    afx_msg void OnPopupApplicationEasymove();
    afx_msg void OnUpdatePopupApplicationEasymove(CCmdUI* pCmdUI);
    afx_msg void OnPopupApplicationMinimizetosystemtrayonstart();
    afx_msg void OnUpdatePopupApplicationMinimizetosystemtrayonstart(CCmdUI* pCmdUI);
    afx_msg void OnCaptureChanged(CWnd* pWnd);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
    afx_msg void OnPopupColorConverttograyscale();
    afx_msg void OnPopupApplicationAllowmultipleinstances();
    afx_msg void OnUpdatePopupApplicationAllowmultipleinstances(CCmdUI* pCmdUI);
    afx_msg void OnPopupColorDetectwebsafe();
    afx_msg void OnUpdatePopupColorDetectwebsafe(CCmdUI* pCmdUI);
    afx_msg void OnPopupModeRgbfloat();
    afx_msg void OnUpdatePopupModeRgbfloat(CCmdUI* pCmdUI);
    afx_msg void OnUpdatePopupModeRgbint(CCmdUI* pCmdUI);
    afx_msg void OnPopupModeRgbint();
    afx_msg void OnPopupOptionsMagnifywhileeyedropping();
    afx_msg void OnUpdatePopupOptionsMagnifywhileeyedropping(CCmdUI* pCmdUI);
    afx_msg void OnPopupOptionsUsecrosshaircursor();
    afx_msg void OnUpdatePopupOptionsUsecrosshaircursor(CCmdUI* pCmdUI);
    afx_msg void OnUpdatePopupOptionsStartcursoroneyedropper(CCmdUI* pCmdUI);
    afx_msg void OnPopupOptionsStartcursoroneyedropper();
    afx_msg void OnPopupSamplingDecreasemultipixelaverage();
    afx_msg void OnPopupSamplingIncreasemultipixelaverage();
    afx_msg void OnPopupSamplingMultipixel();
    afx_msg void OnUpdatePopupSamplingMultipixel(CCmdUI* pCmdUI);
    afx_msg void OnPopupSpaceRgb();
    afx_msg void OnUpdatePopupSpaceRgb(CCmdUI* pCmdUI);
    afx_msg void OnPopupSpaceCmyk();
    afx_msg void OnUpdatePopupSpaceCmyk(CCmdUI* pCmdUI);
    afx_msg void OnChangeBlack();
    afx_msg void OnChangeCyan();
    afx_msg void OnChangeMagenta();
    afx_msg void OnChangeYellow();
    afx_msg void OnPopupModeClarionhex();
    afx_msg void OnUpdatePopupModeClarionhex(CCmdUI* pCmdUI);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);


    //}}AFX_MSG // NOLINT(whitespace/comments)
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}} // NOLINT(whitespace/comments)
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
