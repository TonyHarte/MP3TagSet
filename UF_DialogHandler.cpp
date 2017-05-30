//---------------------------------------------------------------------------
// Thus program sets the ID tags for the current directory - which is usually
// an albun of some sort
//---------------------------------------------------------------------------
//Gets latest version of ComCtl32.dll with extended ListView functionality
#define NTDDI_VERSION   0x05010300      //Windows XP with Service Pack 3
#define _WIN32_WINNT    0x0502          //Windows Server 2003 with SP1, Windows XP with SP2

//#define  WIN32_LEAN_AND_MEAN
//#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#include <windows.h>
#include <commctrl.h>
#include <strsafe.h>

//****************** TEMP TEMP DELETE *************
#include "TEMPDEBUG.h"
//****************** TEMP TEMP DELETE *************

#define  UF_DialogHandler
#include "UF_DialogHandler.h" 
#include "CWindow.h"
#include "MP3TagSet.h"
#include "UG_ListView.h" 
#include "UD_MainWindowHandler.h" 
#include "Resources\resource.h" 
//---------------------------------------------------------------------------



BOOL CALLBACK UF_Dialog_Handler(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//* This function is called by Windows 98 and is passed messages from the message queue.
//******************************************************************************
    RECT        rect;
    HWND        hListWindow = 0;
    HWND        hChildWindow;
    HINSTANCE   hProgramInstance; 
    HWND        hWndParent;
    int         nHeight;
    DWORD       LVExtendedStyle;
    UINT        WindowPositionFlags;
    HDC         hdc;
    TEXTMETRIC  tm;
    int     nCharAvgWidth, nCharMaxWidth, nCapsAvgWidth, nCharHeight;

/*------------------------------------*/
//TEMPDEBUG("***Doing UF_Dialog_Handler");
TEMPDEBUGNOTIFY("      Dialog Handler    ", message);

    switch(message) {
     case WM_INITDIALOG:
        hWndParent = CMainWin.Handle();
        hWndParent = GetParent(hDialog);

        hdc = GetDC(hDialog);
    //        hSourceDisplayFont = Define_Font(hdc, "Courier New", 9);
    //        hOldFont = (HFONT)SelectObject(hdc, hSourceDisplayFont);
        GetTextMetrics (hdc, &tm);                      // Extract font dimensions from the text metrics.
        nCharAvgWidth = tm.tmAveCharWidth;
        nCharMaxWidth = tm.tmMaxCharWidth;
        nCapsAvgWidth = (tm.tmPitchAndFamily & 1 ? 3 : 2) * nCharAvgWidth / 2;
        nCharHeight = tm.tmHeight + tm.tmExternalLeading;
        
        ReleaseDC (hDialog, hdc);

        GetClientRect(hWndParent, &rect);
        WindowPositionFlags = SWP_SHOWWINDOW | SWP_DEFERERASE | SWP_NOSENDCHANGING | SWP_DRAWFRAME;
        SetWindowPos(hDialog, HWND_TOP, rect.left, rect.top, rect.right, (rect.bottom / 2), WindowPositionFlags);
        hListWindow = GetDlgItem(hDialog, IDC_LIST);
        CLVCurrent.Handle_Store(hListWindow);

        G200_Create_LV_ColumnHeaders(hListWindow, nCharAvgWidth);

        if (Album.Present() == 'Y') {
            G300_Insert_LV_ID3Tag_Data(hListWindow);
        }

// Must use the following with WM_CLIPCHILDREN in the dialog
        LVExtendedStyle = ListView_GetExtendedListViewStyle(hListWindow); 
        LVExtendedStyle |= LVS_EX_DOUBLEBUFFER;
        ListView_SetExtendedListViewStyle(hListWindow, LVExtendedStyle);

        return TRUE;

     case WM_SIZE: 
        GetClientRect(hDialog, &rect);

        hChildWindow = GetDlgItem(hDialog, IDC_CURR_GROUPBOX);
        if  (hChildWindow != 0) {
            WindowPositionFlags = SWP_SHOWWINDOW | SWP_DEFERERASE |  SWP_NOCOPYBITS;
            SetWindowPos(hChildWindow, HWND_TOP, 0, 0, rect.right - 0, rect.bottom, WindowPositionFlags); //Needs to be offset to line up
        }

        hListWindow = GetDlgItem(hDialog, IDC_LIST);
        if  (hListWindow != 0) {
            WindowPositionFlags = SWP_SHOWWINDOW | SWP_DEFERERASE | SWP_NOSENDCHANGING |  SWP_NOCOPYBITS;
            SetWindowPos(hListWindow, HWND_TOP, 5, 20, rect.right - 10, rect.bottom - 28, WindowPositionFlags);
        }

        return TRUE;

     case WM_NOTIFY: 
        return F100_Handle_NOTIFY(hDialog, wParam, lParam);
    }    

    return FALSE;
}



BOOL F100_Handle_NOTIFY(HWND hDialog, WPARAM wParam, LPARAM lParam) {
//******************************************************************************
//*
//******************************************************************************
    HWND            hListWindow = 0;
    LPNMLISTVIEW    pnm;
    LPNMLVCUSTOMDRAW  lplvcd;
    NMLVCUSTOMDRAW  gLVCust;
/*------------------------------------*/
TEMPDEBUGNOTIFY("    NOTIFY sub command    ", ((LPNMHDR) lParam)->code);
    hListWindow = GetDlgItem (hDialog, IDC_LIST) ;
    pnm = (LPNMLISTVIEW)lParam;

    if (pnm->hdr.hwndFrom == hListWindow 
    &&  pnm->hdr.code == NM_CUSTOMDRAW)  {
        SetWindowLong(hDialog, DWL_MSGRESULT, (LONG)F150_NOTIFY_CustomDraw(lParam));
        return TRUE;
    }

    return FALSE;
}



LRESULT F150_NOTIFY_CustomDraw(LPARAM lParam) {
//******************************************************************************
//* This whole palaver is to colour the background of the lines.
//******************************************************************************
    LPNMLISTVIEW pnm;
    LPNMLVCUSTOMDRAW  lplvcd;
    NMLVCUSTOMDRAW gLVCust;
/*------------------------------------*/
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
                lplvcd->clrTextBk = RGB(220,220,240); 
            else {
                lplvcd->clrTextBk = RGB(230,230,255); 
            } 
        else {
            if (((lplvcd->nmcd.dwItemSpec) % 2) == 0) 
                lplvcd->clrTextBk = RGB(240,240,240); 
            else {
                lplvcd->clrTextBk = CLR_DEFAULT;
            }
        }
        return CDRF_NEWFONT;
    }
    return CDRF_DODEFAULT;     
}
