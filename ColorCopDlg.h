// Copyright (c) 2024 Jay Prall
// SPDX-License-Identifier: MIT

// ColorCopDlg.h : header file
//



#if !defined(AFX_HTMLCOPDLG_H__EC2A34E6_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_)
#define AFX_HTMLCOPDLG_H__EC2A34E6_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////////
// CColorCopDlg dialog

#define BMP_FILE "\\Color_Cop.bmp"
#define BMP_FILE_DIR "\\ColorCop"



class CSystemTray;

class CColorCopDlg : public CDialog
{
// Construction
public:
    explicit CColorCopDlg(CWnd* pParent = NULL);    // standard constructor
    COLORREF ColorHistory[7];
    COLORREF CustColorBank[16];
    int m_Appflags;
    int m_iSamplingOffset;

    int WinLocX, WinLocY;
    int16_t m_MagLevel;
    int16_t m_FloatPrecision;



    HBITMAP hBitmap, hBitmapClip, hZoomBitmap;

// Dialog Data
    //{{AFX_DATA(CColorCopDlg)
    enum { IDD = IDD_HTMLCOP_DIALOG };
    CButton    m_ColorPick;
    CStatic    m_Lbl4;
    CStatic    m_Lbl3;
    CStatic    m_Lbl2;
    CStatic    m_Lbl1;
    CStatic    m_Slide3;
    CStatic    m_Slide2;
    CStatic    m_Slide1;
    CStatic    m_MagPlus;
    CStatic    m_MagMinus;
    CStatic    m_ColorPalette;
    CStatic    m_MagWindow;
    CStatic    m_Q7;
    CStatic    m_Q6;
    CStatic    m_Q5;
    CStatic    m_Q4;
    CStatic    m_Q3;
    CStatic    m_Q2;
    CStatic    m_Q1;
    CStatic    m_ColorPreview;
    CButton    m_ExpandDialog;
    CStatic    m_Magnifier;
    CStatic    m_EyeLoc;
    int        m_Greendec;
    int        m_Bluedec;
    int        m_Reddec;
    CString    m_Hexcolor;
    int        m_Black;
    int        m_Cyan;
    int        m_Magenta;
    int        m_Yellow;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorCopDlg)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    bool m_bvisible;

    HICON m_hIcon, m_hBlank, m_hEye;

    HACCEL m_hAcceleratorTable;            // Accelerator var

    // Store for snapback from websafe,  unsigned char only takes 1 byte, 0 - 255
    unsigned char m_OldRed, m_OldBlue, m_OldGreen;
    BOOL bOldClrExist;

    BOOL m_isEyedropping;
    BOOL m_isMagnifying;
    BOOL bMinimized;
    BOOL m_isMagPlusDown;
    BOOL m_isMagMinusDown;
    BOOL m_InitialMove;
    BOOL m_bCalcColorPal;
    BOOL bRelativePosition;
    BOOL m_MagDrop;

    CStatusBarCtrl m_StatBar;
    CToolTipCtrl m_ToolTip;

    int smHeight, smWidth, lgHeight, lgWidth;
    unsigned int m_nwide, m_ntall;


    // color palette stuff
    double r,g,b;
    double Hue, Sat, Light;

    struct swatchStruct{
        double A;
        double B;
        double C;
    } Swatch[6];
    swatchStruct OrigSwatch;

    COLORREF ColorPal[6][7];
    int palcol;

    POINT RelativePoint;
    POINT RelativePointEnd;

    CMenu copmenu;
    HDC hdc, hdcMem, hdcZoomMem;

    CRect Q1rect, Q2rect, Q3rect, Q4rect, Q5rect, Q6rect, Q7rect;
    CRect buttonrect;                    // color window rect

    CRect magrect, colorpalrect;        // magnifier rect, based off color window rect
    CRect testrect;
    CRect magplus,magminus;
    RECT CCopRect, CCopsmRect;



    HCURSOR m_hEyeCursor;
    HCURSOR m_hMagCursor;
    HCURSOR m_hStandardCursor;
    HCURSOR m_hMoveCursor;
    HCURSOR m_hHandCursor;
    //HCURSOR m_hEyeCursor_3x3, m_hEyeCursor_5x5;

    ////  systray Internal data
    HICON hIcon_;
    bool bMinimized_;
    CSystemTray* pTrayIcon_;
    int nTrayNotificationMsg_;
    // end
    void CColorCopDlg::FloatPrecisionUp();
    void CColorCopDlg::FloatPrecisionDown();
    //void CColorCopDlg::BlockMeasure(POINT ptBeg, POINT ptEnd);
    void CColorCopDlg::GetScreenBitmap(CPoint point);

    void CColorCopDlg::StopCapture(void);
    void CColorCopDlg::DisplayColor();
    void CColorCopDlg::ParseHTML(CString inst);
    void CColorCopDlg::ParseDelphi(CString inst);
    void CColorCopDlg::ParseClarion(CString inst);
    void CColorCopDlg::FigurePound();
    void CColorCopDlg::TestForExpand();
    int CColorCopDlg::DecimaltoWebsafe(int originalDec);
    void CColorCopDlg::TestForUpperHex();
    void CColorCopDlg::OnconvertHEX();
    void CColorCopDlg::OnconvertRGB();
    void CColorCopDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
    void CColorCopDlg::UpdateMenu(CMenu* pMenu);
    void CColorCopDlg::ToggleOnTop(bool bSetStatusbartext);
    void CColorCopDlg::TestForWebsafe();
    void CColorCopDlg::SetupTaskBarButton();
    void CColorCopDlg::SetupTrayIcon();
    void CColorCopDlg::SetupSystemMenu();
    void CColorCopDlg::SetupWindowRects();
    bool CColorCopDlg::LoadPersistentVariables();
    bool CColorCopDlg::AveragePixelArea(HDC hdc, int* m_Reddec, int* m_Greendec, int* m_Bluedec, CPoint point);
    void CColorCopDlg::SetupStatusBar();
    void CColorCopDlg::SetStatusBarText(LPCTSTR statusText);
    int CColorCopDlg::RangeCheck(int icolorval);
    void CColorCopDlg::AdvanceColorHistory();
    void CColorCopDlg::GetHistoryColor(int Cindex);
    HBITMAP CColorCopDlg::CopyBitmap (HBITMAP hBitmapSrc);
    bool CColorCopDlg::isWebsafeColor(int R, int G, int B);
    CString CColorCopDlg::GetTempFolder();
    BOOL CColorCopDlg::GetShellFolderPath(char* pShellFolder, char* pShellPath);

    void CColorCopDlg::SetStatusBarText(UINT strResource, int toggleVal);

    void CColorCopDlg::CalcColorPal();
    double CColorCopDlg::shiftHue(double hue);
    void CColorCopDlg::setSeedColor();
    void CColorCopDlg::RGBtoHSL(double R, double G, double B);
    void CColorCopDlg::HSLtoRGB(double H, double S, double L);
    void CColorCopDlg::setupSwatches();
    void CColorCopDlg::handleShifts();
    void CColorCopDlg::printSwatch();
    double CColorCopDlg::plusValue(double num);
    double CColorCopDlg::minusValue(double num);
    void CColorCopDlg::RecalcZoom();
    void CColorCopDlg::FireOptionMenu();
    PBITMAPINFO CColorCopDlg::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
    void CColorCopDlg::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
    void CColorCopDlg::UpdateCMYKFromRGB(int red, int green, int blue);
    void CColorCopDlg::ChangeColorSpace(bool bRGB);

    // Generated message map functions
    //{{AFX_MSG(CColorCopDlg)
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
    afx_msg BOOL OnMouseWheel(UINT nFlags, int16_t zDelta, CPoint pt);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPopupApplicationHelp();
    afx_msg void OnPopupApplicationEasymove();
    afx_msg void OnUpdatePopupApplicationEasymove(CCmdUI* pCmdUI);
    afx_msg void OnPopupApplicationMinimizetosystemtrayonstart();
    afx_msg void OnUpdatePopupApplicationMinimizetosystemtrayonstart(CCmdUI* pCmdUI);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
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
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLCOPDLG_H__EC2A34E6_4FAA_11D3_81A0_A79013DBA62A__INCLUDED_)
