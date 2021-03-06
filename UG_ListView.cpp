//---------------------------------------------------------------------------
// Processes to populate and handle the List Views
//---------------------------------------------------------------------------
//#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1


#include <windows.h>
#include <cstring>
#include <Commctrl.h>
#include <strsafe.h>
//****************** TEMP TEMP DELETE *************
#include "TEMPDEBUG.h"
//****************** TEMP TEMP DELETE *************
using namespace std;
#define  UG_ListView
#include "CWindow.h"
#include "CID3Tag.h"
#include "MP3TagSet.h"
#include "UG_ListView.h" 
#include "Resources\resource.h" 
//---------------------------------------------------------------------------



void G100_Setup_Header_Details(void) {
//******************************************************************************
//*
//******************************************************************************
    HINSTANCE hProgramInstance;
    int nCol;
/*------------------------------------*/
    hProgramInstance = CMainWin.Program_Instance();

    for (nCol = 0; nCol < NUMCOLS; nCol++)     {
        LoadString(hProgramInstance, IDS_FIRSTCOLUMN + nCol, gColumn[nCol].Title, COLTEXTWIDTH); // Load the names of the column headings from the string resources.
    }

    gColumn[0].Width = min(50, CID3Tag::nMaxFileNameLen);
    gColumn[1].Width = min(50, CID3Tag::nMaxTitleLen);
    gColumn[2].Width = CID3Tag::nMaxArtistLen;
    gColumn[3].Width = CID3Tag::nMaxAlbumLen;
    gColumn[4].Width = 10;
}


/*
HWND G100_Create_LV_For_Current(HWND hParent) {
//******************************************************************************
//*
//******************************************************************************
    RECT    rect;
    HINSTANCE hProgramInstance;
    HWND    hListWindow;
    HWND    hWndParent;
    DWORD   WindowStyle, ExtendedWindowStyle;
/*------------------------------------*/
/*
    GetClientRect(hParent, &rect);
    hWndParent = CMainWin.Handle();
    GetWindowRect(hWndParent, &rect);
//        SetWindowPos(hDialog, HWND_TOP, rect.left, rect.top, rect.right, (rect.bottom / 2), SWP_SHOWWINDOW);

    hProgramInstance = CMainWin.Program_Instance();
    ExtendedWindowStyle = WS_EX_WINDOWEDGE|WS_EX_NOPARENTNOTIFY;
    WindowStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_EDITLABELS;

    hListWindow = CreateWindowEx(ExtendedWindowStyle, WC_LISTVIEW, NULL, WindowStyle,
                                CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, (rect.bottom - rect.top)/2,
                                hParent, NULL, hProgramInstance, NULL);

    CLVCurrent.Handle_Store(hListWindow);
    G200_Create_LV_ColumnHeaders(hListWindow, 100);

    return hListWindow;
}
*/


HWND G100_Create_LVControl_For_Proposed(HWND hParent) {
//******************************************************************************
//*
//******************************************************************************
    RECT rect;
    HINSTANCE hProgramInstance;
    HWND hListWindow;
    int nHeight;
/*------------------------------------*/
    GetClientRect(hParent, &rect);
    nHeight = (rect.bottom - rect.top)/2;
    hProgramInstance = CMainWin.Program_Instance();

    hListWindow = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY,
                    WC_LISTVIEW,
                    NULL,
                    WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS
                    |LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_EDITLABELS, 
                    //CW_USEDEFAULT, CW_USEDEFAULT,
                    0, nHeight, rect.right - rect.left, nHeight,
                    hParent,
                    NULL,                         //(HMENU) ID_LISTVIEW,
                    hProgramInstance, NULL);

    CLVProposed.Handle_Store(hListWindow);
 
    return hListWindow;
}



void G200_Create_LV_ColumnHeaders(HWND hListWindow, int nCharWidth) {
//******************************************************************************
//*
//******************************************************************************
    LVCOLUMN lvcol;
    int nCol = 0;
//    int nColWidth = 0;
//    int nMaxColWidth = 50;
/*------------------------------------*/
    lvcol.mask = LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
    lvcol.cchTextMax = COLTEXTWIDTH;

    for (nCol = 0; nCol < NUMCOLS; nCol++)     {
        lvcol.iSubItem = nCol;
        lvcol.cx = gColumn[nCol].Width * nCharWidth;               // Width of column in pixels.
        lvcol.pszText = gColumn[nCol].Title;

        ListView_InsertColumn(hListWindow, nCol, &lvcol);
    }
}



void G300_Insert_LV_Current_Data(HWND hListWindow) {
//******************************************************************************
//* NOTE: have to insert ALL the  items before can update the sub-items
//******************************************************************************
    char    szText[MAX_PATH];     // Temporary buffer.
    LVITEM  LvItem;
    CID3Tag gCurrTag; 
    int nRow = 0, nCol = 0;
    LRESULT RC;
    int size = 0;
    int nLen = 0;
    COLORREF colours;
/*------------------------------------*/
    size = vTag.size();
    ListView_SetItemCount(hListWindow, vTag.size());

    for (ID3ind = vTag.begin(); ID3ind != vTag.end(); ID3ind++) {
        memset(&LvItem, 0, sizeof(LvItem));     // Clear down struct's Members

        LvItem.iItem        = nRow++;           // Row we are inserting  
        LvItem.mask         = LVIF_TEXT;        // Text Style
        LvItem.cchTextMax   = MAX_PATH;         // Max size of test
        gCurrTag            = *(ID3ind);

//=================================
        LvItem.iSubItem     = 0;
        strcpy_s(szText, MAX_PATH, gCurrTag.File_Name()); 
        LvItem.pszText = szText;
        RC = SendMessage(hListWindow, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Insert Item into Listview

//=================================
        LvItem.iSubItem     = 1;
        strcpy_s(szText, MAX_PATH, gCurrTag.Title());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 2;
        strcpy_s(szText, MAX_PATH, gCurrTag.Artist());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 3;
        strcpy_s(szText, MAX_PATH, gCurrTag.Album());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 4;
        strcpy_s(szText, MAX_PATH, gCurrTag.Track());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems
    }
}



void G400_Insert_LV_Proposed_Data(HWND hListWindow) {
//******************************************************************************
//* NOTE: have to insert ALL the  items before can update the sub-items
//******************************************************************************
    char    szText[MAX_PATH];     // Temporary buffer.
    LVITEM  LvItem;
    CProposedValues gProposed; 
    int nRow = 0, nCol = 0;
    LRESULT RC;
    int size = 0;
    int nLen = 0;
    COLORREF colours;
/*------------------------------------*/
    size = vProposed.size();
    ListView_SetItemCount(hListWindow, vProposed.size());

    for (PVind = vProposed.begin(); PVind != vProposed.end(); PVind++) {
        memset(&LvItem, 0, sizeof(LvItem));     // Clear down struct's Members

        LvItem.iItem        = nRow++;           // Row we are inserting  
        LvItem.mask         = LVIF_TEXT;        // Text Style
        LvItem.cchTextMax   = MAX_PATH;         // Max size of test
        gProposed            = *(PVind);

//=================================
        LvItem.iSubItem     = 0;
        strcpy_s(szText, MAX_PATH, gProposed.File_Name()); 
        LvItem.pszText = szText;
        RC = SendMessage(hListWindow, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Insert Item into Listview

//=================================
        LvItem.iSubItem     = 1;
        strcpy_s(szText, MAX_PATH, gProposed.Title());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 2;
        strcpy_s(szText, MAX_PATH, gProposed.Artist());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 3;
        strcpy_s(szText, MAX_PATH, gProposed.Album());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems

//=================================
        LvItem.iSubItem     = 4;
        strcpy_s(szText, MAX_PATH, gProposed.Track());
        LvItem.pszText = szText;
        SendMessage(hListWindow, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems
    }
}
