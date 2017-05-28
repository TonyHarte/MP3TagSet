//---------------------------------------------------------------------------
// Thus program sets the ID tags for the current directory - which is usually
// an albun of some sort
//---------------------------------------------------------------------------
//Gets latest version of ComCtl32.dll with extended ListView functionality
#define NTDDI_VERSION   0x05010300      //Windows XP with Service Pack 3
#define _WIN32_WINNT    0x0502          //Windows Server 2003 with SP1, Windows XP with SP2

//#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE 


#include <windows.h>
#include <commctrl.h>
//#include <strsafe.h>
//#include <cstdio>
//#include <vector>
//using namespace std;
//****************** TEMP TEMP DELETE *************
#include "TEMPDEBUG.h"
//****************** TEMP TEMP DELETE *************

#define  UD_MainWindowHandler
#include "CWindow.h"
#include "MP3TagSet.h"
#include "UG_ListView.h" 
#include "UD_MainWindowHandler.h" 
#include "Resources\resource.h" 
//---------------------------------------------------------------------------



LRESULT CALLBACK UD_Main_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//* This function is called by Windows itself and is passed messages from the
//* message queue.
//******************************************************************************
TEMPDEBUGNOTIFY("    Main Handler    ", message);
    switch(message) {
     case WM_CREATE:
        D100_Handle_CREATE(hWnd, wParam, lParam);
        break;

     case WM_COMMAND:
        D200_Handle_COMMAND(hWnd, wParam, lParam);
        break;

     case WM_SIZE: 
        D300_Handle_SIZE(hWnd, wParam, lParam);
        break;

     case WM_SIZING: 
        break;

     case WM_NOTIFY: 
        return D400_Handle_NOTIFY(hWnd, wParam, lParam);
        break;
/*
     case WM_PAINT:
        D500_Handle_PAINT(hWnd);
        break;
*/
     case WM_DESTROY: 
        PostQuitMessage(0);
        break;
/*
     case WM_CTLCOLORBTN:
     case WM_CTLCOLORSTATIC:
        if(GetDlgItem(hWnd, IDC_PROPOSED_GROUPBOX) == (HWND)lParam) {
            HDC hDC = (HDC)wParam;		
            SetBkColor(hDC, RGB(0, 0xFF, 0));
// GetSysColor(COLOR_BTNFACE));
            SetTextColor(hDC, RGB(0, 0xFF, 0));
            SetBkMode(hDC, TRANSPARENT);	
            return (INT_PTR)CreateSolidBrush(GetSysColor(COLOR_BTNFACE));			
        }
        break;
*/
/*
     case WM_CTLCOLORSTATIC:
        if(GetWindowLong( (HWND)lParam, GWL_ID) == IDC_STATIC)
           return MakeControlRed( (HDC)wParam );
*/
     default:
        // Let Windows process the other messages
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}



void D100_Handle_CREATE(HWND hWnd, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//*
//******************************************************************************
    HWND    hListWindow;
    HDC     hdc;
    TEXTMETRIC tm;
    int     nCharAvgWidth, nCharMaxWidth, nCapsAvgWidth, nCharHeight;
    DWORD   LVExtendedStyle;
/*------------------------------------*/
//        G100_Create_LV_For_Current(hWnd); 
    hdc = GetDC(hWnd);
//        hSourceDisplayFont = Define_Font(hdc, "Courier New", 9);
//        hOldFont = (HFONT)SelectObject(hdc, hSourceDisplayFont);
    GetTextMetrics (hdc, &tm);                      // Extract font dimensions from the text metrics.
    nCharAvgWidth = tm.tmAveCharWidth;
    nCharMaxWidth = tm.tmMaxCharWidth;
    nCapsAvgWidth = (tm.tmPitchAndFamily & 1 ? 3 : 2) * nCharAvgWidth / 2;
    nCharHeight = tm.tmHeight + tm.tmExternalLeading;
    
    ReleaseDC (hWnd, hdc) ;

    D150_Create_Static_Controls(hWnd, wParam, lParam);

    hListWindow = G100_Create_LVControl_For_Proposed(hWnd); 
    D000_Extract_ID3_Data();
    hListWindow = CLVProposed.Handle();
    G100_Setup_Header_Details();
    G200_Create_LV_ColumnHeaders(hListWindow, nCharAvgWidth);
    G400_Insert_LV_Proposed_Data(hListWindow);

    LVExtendedStyle = ListView_GetExtendedListViewStyle(hListWindow); 
    LVExtendedStyle |= LVS_EX_DOUBLEBUFFER;
    ListView_SetExtendedListViewStyle(hListWindow, LVExtendedStyle);
}



void D150_Create_Static_Controls(HWND hWnd, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//*
//******************************************************************************
    HWND    hStaticWindow;
    HINSTANCE   hProgramInstance;

//    HDC     hdc;
//    TEXTMETRIC tm;
//    int     nCharAvgWidth, nCharMaxWidth, nCapsAvgWidth, nCharHeight;
//    DWORD   LVExtendedStyle;
    DWORD   dwError;
    char    cErrorNumber[6];
    char    cErrorMessage[512];
    DWORD   gGroupBoxStyles;
/*------------------------------------*/
    hProgramInstance = CMainWin.Program_Instance();
    gGroupBoxStyles = WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_CLIPSIBLINGS | BS_CENTER;

    hStaticWindow = CreateWindowEx(
                    WS_EX_TRANSPARENT | WS_EX_WINDOWEDGE, 
                    "Button", 
                    "Proposed Values",                  // title
                    gGroupBoxStyles,
                    0, 0, 0, 0,
                    hWnd,                               // parent window 
                    (HMENU) IDC_PROPOSED_GROUPBOX,                          // Child window ID
                    hProgramInstance,                   // handle of this instance of the program 
                    NULL                                // no additional arguments 
                    );

    dwError = GetLastError(); 
    sprintf(cErrorNumber, "%d", dwError);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, cErrorMessage, sizeof(cErrorMessage), NULL);


    CProposedStatic.Handle_Store(hStaticWindow);
}



void D200_Handle_COMMAND(HWND hWnd, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//*
//******************************************************************************
    int nResponse;
#define IDM_EXIT        100
#define IDM_HELP        101
/*------------------------------------*/
    switch(LOWORD(wParam)) {
     case IDM_EXIT:  
        nResponse = MessageBox(hWnd, "Quit the Program?", "Exit", MB_YESNO);

        if (nResponse == IDYES) {
            PostQuitMessage(0);
        }
        break;

     case IDM_HELP: 
        MessageBox(hWnd, "Try resizing the header.", "Help", MB_OK);
        break;
    }
}



void D300_Handle_SIZE(HWND hWnd, WPARAM wParam, LPARAM lParam) { 
//******************************************************************************
//* Resize the header control when its parent window changes size
//******************************************************************************
    RECT    rect;
    RECT    gStaticRect;
    HWND    hDialogWindow;
    HWND    hChildWindow;
    RECT    DiagRect, LVRect;
    int     nHeight;
//    int     cxClient, cyClient;
/*------------------------------------*/
    TEMPDEBUG("***FUNC*** D300_Handle_SIZE");
    GetClientRect(hWnd, &rect);
    nHeight = (rect.bottom - rect.top) / 2;

    hDialogWindow = CDialogCurrent.Handle();
    if  (hDialogWindow != 0) {
        SetWindowPos(hDialogWindow, HWND_TOP, 0, 0, rect.right, nHeight, SWP_NOZORDER);
    }

    hChildWindow = CProposedStatic.Handle();
    if  (hChildWindow != 0) {
        SetWindowPos(hChildWindow, HWND_TOP, 0, nHeight, rect.right, nHeight, SWP_SHOWWINDOW);
    }

    hChildWindow = CLVProposed.Handle();
    if  (hChildWindow != 0) {
        SetWindowPos(hChildWindow, HWND_TOP, 5, nHeight + 20, rect.right - 10, nHeight - 28, SWP_SHOWWINDOW);
//        ShowWindow(hChildWindow, SW_SHOW);
//        UpdateWindow(hChildWindow);
    }
}



LRESULT D400_Handle_NOTIFY(HWND hWnd, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//*
//******************************************************************************
    LPNMLISTVIEW pnm;
    NMLISTVIEW nm;
    LPNMLVCUSTOMDRAW  lplvcd;
    NMLVCUSTOMDRAW gLVCust;
/*------------------------------------*/
    pnm = (LPNMLISTVIEW) lParam;
    nm = *(LPNMLISTVIEW) lParam;
    lplvcd = (LPNMLVCUSTOMDRAW) lParam;
    gLVCust = *(LPNMLVCUSTOMDRAW) lParam;

TEMPDEBUGNOTIFY("    NOTIFY sub command    ", pnm->hdr.code);
    switch (pnm->hdr.code) {
     case NM_CUSTOMDRAW:
        lplvcd = (LPNMLVCUSTOMDRAW) lParam;

        switch (lplvcd->nmcd.dwDrawStage) {
         case CDDS_PREPAINT :
            return CDRF_NOTIFYITEMDRAW;

         case CDDS_ITEMPREPAINT:
            return CDRF_NOTIFYITEMDRAW;

         case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
            lplvcd->clrText = CLR_DEFAULT;
          
            if (lplvcd->iSubItem == 0) 
                if (((lplvcd->nmcd.dwItemSpec) % 2) == 0) 
                    lplvcd->clrTextBk = RGB(247,220,220); 
                else {
                    lplvcd->clrTextBk = RGB(255,230,230); 
                } 
            else {
                if (((lplvcd->nmcd.dwItemSpec) % 2) == 0) 
                    lplvcd->clrTextBk = RGB(244,240,240); 
                else {
                    lplvcd->clrTextBk = CLR_DEFAULT;
                }
            }
            return CDRF_NEWFONT;
        }
    }
    return CDRF_DODEFAULT;     
}



void D500_Handle_PAINT(HWND hWnd) {
//******************************************************************************
//*
//******************************************************************************
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    int nLineSpacing;
    int nEntry;
    SIZE size;
    NONCLIENTMETRICS gMetrics;

    int i, j, ColStart, chrs;
    char str[80];
/*------------------------------------*/
    TEMPDEBUG("***FUNC*** D500_Handle_PAINT");
    TEMPDEBUG("  ");

    hdc = BeginPaint(hWnd, &ps);
    
    gMetrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), (PVOID) &gMetrics, 0);
    SelectObject(hdc, CreateFontIndirect(&gMetrics.lfMenuFont));

/*    GetTextMetrics(hdc, &tm);
    nLineSpacing = tm.tmHeight + tm.tmInternalLeading;
*/

    EndPaint(hWnd, &ps);
}
