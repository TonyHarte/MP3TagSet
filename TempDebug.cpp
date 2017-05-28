#define _CRT_SECURE_NO_DEPRECATE 

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdarg.h>
#include "TEMPDEBUG.h"


char    fTurnOffDebugQuickly = 'Y';   //Set to 'Y' to quickly bypass
char    fDebugFileOpen = 'N';
char    Buffer[250];
static  DWORD   BufferLen;

//******************************************************************************
// USAGE: from your program call TEMPDEBUG with sprintf type details.  This
// function will add CRLF and write to DEBUG.TXT eg
// TEMPDEBUG("this is left %d, this is right %d", nLeft, nRight)
//******************************************************************************
char    DebugFileName[] = "DEBUG.TXT";
HANDLE  hDebugFile;
char    cDebugErrorDisplay[512];
char    cDebugDisplayMessage[512];


NOTIFICATIONS WMLookupTable[] = {
    {0x0000, "WM_NULL"},{0x0001, "WM_CREATE"},{0x0002, "WM_DESTROY"},{0x0003, "WM_MOVE"},
    {0x0005, "WM_SIZE"},{0x0006, "WM_ACTIVATE"},{0x0007, "WM_SETFOCUS"},{0x0008, "WM_KILLFOCUS"},
    {0x000A, "WM_ENABLE"},{0x000B, "WM_SETREDRAW"},{0x000C, "WM_SETTEXT"},{0x000D, "WM_GETTEXT"},
    {0x000E, "WM_GETTEXTLENGTH"},{0x000F, "WM_PAINT"},{0x0010, "WM_CLOSE"},{0x0011, "WM_QUERYENDSESSION"},
    {0x0012, "WM_QUIT"},{0x0013, "WM_QUERYOPEN"},{0x0014, "WM_ERASEBKGND"},{0x0015, "WM_SYSCOLORCHANGE"},
    {0x0016, "WM_ENDSESSION"},{0x0018, "WM_SHOWWINDOW"},{0x001A, "WM_WININICHANGE"},{WM_WININICHANGE, "WM_SETTINGCHANGE"},
    {0x001B, "WM_DEVMODECHANGE"},{0x001C, "WM_ACTIVATEAPP"},{0x001D, "WM_FONTCHANGE"},{0x001E, "WM_TIMECHANGE"},
    {0x001F, "WM_CANCELMODE"},{0x0020, "WM_SETCURSOR"},{0x0021, "WM_MOUSEACTIVATE"},{0x0022, "WM_CHILDACTIVATE"},
    {0x0023, "WM_QUEUESYNC"},{0x0024, "WM_GETMINMAXINFO"},{0x0026, "WM_PAINTICON"},{0x0027, "WM_ICONERASEBKGND"},
    {0x0028, "WM_NEXTDLGCTL"},{0x002A, "WM_SPOOLERSTATUS"},{0x002B, "WM_DRAWITEM"},{0x002C, "WM_MEASUREITEM"},
    {0x002D, "WM_DELETEITEM"},{0x002E, "WM_VKEYTOITEM"},{0x002F, "WM_CHARTOITEM"},{0x0030, "WM_SETFONT"},
    {0x0031, "WM_GETFONT"},{0x0032, "WM_SETHOTKEY"},{0x0033, "WM_GETHOTKEY"},{0x0037, "WM_QUERYDRAGICON"},
    {0x0039, "WM_COMPAREITEM"},{0x003D, "WM_GETOBJECT"},{0x0041, "WM_COMPACTING"},{0x0044, "WM_COMMNOTIFY"},
    {0x0046, "WM_WINDOWPOSCHANGING"},{0x0047, "WM_WINDOWPOSCHANGED"},{0x0048, "WM_POWER"},{0x004A, "WM_COPYDATA"},
    {0x004B, "WM_CANCELJOURNAL"},{0x004E, "WM_NOTIFY"},{0x0050, "WM_INPUTLANGCHANGEREQUEST"},{0x0051, "WM_INPUTLANGCHANGE"},
    {0x0052, "WM_TCARD"},{0x0053, "WM_HELP"},{0x0054, "WM_USERCHANGED"},{0x0055, "WM_NOTIFYFORMAT"},
    {0x007B, "WM_CONTEXTMENU"},{0x007C, "WM_STYLECHANGING"},{0x007D, "WM_STYLECHANGED"},{0x007E, "WM_DISPLAYCHANGE"},
    {0x007F, "WM_GETICON"},{0x0080, "WM_SETICON"},{0x0081, "WM_NCCREATE"},{0x0082, "WM_NCDESTROY"},
    {0x0083, "WM_NCCALCSIZE"},{0x0084, "WM_NCHITTEST"},{0x0085, "WM_NCPAINT"},{0x0086, "WM_NCACTIVATE"},
    {0x0087, "WM_GETDLGCODE"},{0x0088, "WM_SYNCPAINT"},{0x00A0, "WM_NCMOUSEMOVE"},{0x00A1, "WM_NCLBUTTONDOWN"},
    {0x00A2, "WM_NCLBUTTONUP"},{0x00A3, "WM_NCLBUTTONDBLCLK"},{0x00A4, "WM_NCRBUTTONDOWN"},{0x00A5, "WM_NCRBUTTONUP"},
    {0x00A6, "WM_NCRBUTTONDBLCLK"},{0x00A7, "WM_NCMBUTTONDOWN"},{0x00A8, "WM_NCMBUTTONUP"},{0x00A9, "WM_NCMBUTTONDBLCLK"},
    {0x0100, "WM_KEYFIRST"},{0x0100, "WM_KEYDOWN"},{0x0101, "WM_KEYUP"},{0x0102, "WM_CHAR"},
    {0x0103, "WM_DEADCHAR"},{0x0104, "WM_SYSKEYDOWN"},{0x0105, "WM_SYSKEYUP"},{0x0106, "WM_SYSCHAR"},
    {0x0107, "WM_SYSDEADCHAR"},{0x0108, "WM_KEYLAST"},{0x010D, "WM_IME_STARTCOMPOSITION"},{0x010E, "WM_IME_ENDCOMPOSITION"},
    {0x010F, "WM_IME_COMPOSITION"},{0x010F, "WM_IME_KEYLAST"},{0x0110, "WM_INITDIALOG"},{0x0111, "WM_COMMAND"},
    {0x0112, "WM_SYSCOMMAND"},{0x0113, "WM_TIMER"},{0x0114, "WM_HSCROLL"},{0x0115, "WM_VSCROLL"},
    {0x0116, "WM_INITMENU"},{0x0117, "WM_INITMENUPOPUP"},{0x011F, "WM_MENUSELECT"},{0x0120, "WM_MENUCHAR"},
    {0x0121, "WM_ENTERIDLE"},{0x0122, "WM_MENURBUTTONUP"},{0x0123, "WM_MENUDRAG"},{0x0124, "WM_MENUGETOBJECT"},
    {0x0125, "WM_UNINITMENUPOPUP"},{0x0126, "WM_MENUCOMMAND"},{0x0132, "WM_CTLCOLORMSGBOX"},{0x0133, "WM_CTLCOLOREDIT"},
    {0x0134, "WM_CTLCOLORLISTBOX"},{0x0135, "WM_CTLCOLORBTN"},{0x0136, "WM_CTLCOLORDLG"},{0x0137, "WM_CTLCOLORSCROLLBAR"},
    {0x0138, "WM_CTLCOLORSTATIC"},{0x0200, "WM_MOUSEFIRST"},{0x0200, "WM_MOUSEMOVE"},{0x0201, "WM_LBUTTONDOWN"},
    {0x0202, "WM_LBUTTONUP"},{0x0203, "WM_LBUTTONDBLCLK"},{0x0204, "WM_RBUTTONDOWN"},{0x0205, "WM_RBUTTONUP"},
    {0x0206, "WM_RBUTTONDBLCLK"},{0x0207, "WM_MBUTTONDOWN"},{0x0208, "WM_MBUTTONUP"},{0x0209, "WM_MBUTTONDBLCLK"},
    {0x020A, "WM_MOUSEWHEEL"},{0x020A, "WM_MOUSELAST"},{0x0209, "WM_MOUSELAST"},{0x0210, "WM_PARENTNOTIFY"},
    {0x0211, "WM_ENTERMENULOOP"},{0x0212, "WM_EXITMENULOOP"},{0x0213, "WM_NEXTMENU"},{0x0214, "WM_SIZING"},
    {0x0215, "WM_CAPTURECHANGED"},{0x0216, "WM_MOVING"},{0x0218, "WM_POWERBROADCAST"},{0x0219, "WM_DEVICECHANGE"},
    {0x0220, "WM_MDICREATE"},{0x0221, "WM_MDIDESTROY"},{0x0222, "WM_MDIACTIVATE"},{0x0223, "WM_MDIRESTORE"},
    {0x0224, "WM_MDINEXT"},{0x0225, "WM_MDIMAXIMIZE"},{0x0226, "WM_MDITILE"},{0x0227, "WM_MDICASCADE"},
    {0x0228, "WM_MDIICONARRANGE"},{0x0229, "WM_MDIGETACTIVE"},{0x0230, "WM_MDISETMENU"},{0x0231, "WM_ENTERSIZEMOVE"},
    {0x0232, "WM_EXITSIZEMOVE"},{0x0233, "WM_DROPFILES"},{0x0234, "WM_MDIREFRESHMENU"},{0x0281, "WM_IME_SETCONTEXT"},
    {0x0282, "WM_IME_NOTIFY"},{0x0283, "WM_IME_CONTROL"},{0x0284, "WM_IME_COMPOSITIONFULL"},{0x0285, "WM_IME_SELECT"},
    {0x0286, "WM_IME_CHAR"},{0x0288, "WM_IME_REQUEST"},{0x0290, "WM_IME_KEYDOWN"},{0x0291, "WM_IME_KEYUP"},
    {0x02A1, "WM_MOUSEHOVER"},{0x02A3, "WM_MOUSELEAVE"},{0x0300, "WM_CUT"},{0x0301, "WM_COPY"},
    {0x0302, "WM_PASTE"},{0x0303, "WM_CLEAR"},{0x0304, "WM_UNDO"},{0x0305, "WM_RENDERFORMAT"},
    {0x0306, "WM_RENDERALLFORMATS"},{0x0307, "WM_DESTROYCLIPBOARD"},{0x0308, "WM_DRAWCLIPBOARD"},{0x0309, "WM_PAINTCLIPBOARD"},
    {0x030A, "WM_VSCROLLCLIPBOARD"},{0x030B, "WM_SIZECLIPBOARD"},{0x030C, "WM_ASKCBFORMATNAME"},{0x030D, "WM_CHANGECBCHAIN"},
    {0x030E, "WM_HSCROLLCLIPBOARD"},{0x030F, "WM_QUERYNEWPALETTE"},{0x0310, "WM_PALETTEISCHANGING"},{0x0311, "WM_PALETTECHANGED"},
    {0x0312, "WM_HOTKEY"},{0x0317, "WM_PRINT"},{0x0318, "WM_PRINTCLIENT"},{0x0358, "WM_HANDHELDFIRST"},
    {0x035F, "WM_HANDHELDLAST"},{0x0360, "WM_AFXFIRST"},{0x037F, "WM_AFXLAST"},{0x0380, "WM_PENWINFIRST"},
    {0x038F, "WM_PENWINLAST"},{0x8000, "WM_APP"},{0x0400, "WM_USER"}
};



NOTIFICATIONS CCLookupTable[] = {
     NM_OUTOFMEMORY,       "NM_OUTOFMEMORY",
     NM_CLICK,             "NM_CLICK",
     NM_DBLCLK,            "NM_DBLCLK",
     NM_RETURN,            "NM_RETURN",
     NM_RCLICK,            "NM_RCLICK",
     NM_RDBLCLK,           "NM_RDBLCLK",
     NM_SETFOCUS,          "NM_SETFOCUS",
     NM_KILLFOCUS,         "NM_KILLFOCUS",
     NM_CUSTOMDRAW,        "NM_CUSTOMDRAW",
     NM_HOVER,             "NM_HOVER",
     LVN_ITEMCHANGING,     "LVN_ITEMCHANGING",
     LVN_ITEMCHANGED,      "LVN_ITEMCHANGED",
     LVN_INSERTITEM,       "LVN_INSERTITEM",
     LVN_DELETEITEM,       "LVN_DELETEITEM",
     LVN_DELETEALLITEMS,   "LVN_DELETEALLITEMS",
     LVN_BEGINLABELEDITA,  "LVN_BEGINLABELEDITA",
     LVN_BEGINLABELEDITW,  "LVN_BEGINLABELEDITW",
     LVN_ENDLABELEDITA,    "LVN_ENDLABELEDITA",
     LVN_ENDLABELEDITW,    "LVN_ENDLABELEDITW",
     LVN_COLUMNCLICK,      "LVN_COLUMNCLICK",
     LVN_BEGINDRAG,        "LVN_BEGINDRAG",
     LVN_BEGINRDRAG,       "LVN_BEGINRDRAG",
     LVN_GETDISPINFOA,     "LVN_GETDISPINFOA",
     LVN_GETDISPINFOW,     "LVN_GETDISPINFOW",
     LVN_SETDISPINFOA,     "LVN_SETDISPINFOA",
     LVN_SETDISPINFOW,     "LVN_SETDISPINFOW",
     LVN_KEYDOWN,          "LVN_KEYDOWN",
     HDN_ITEMCHANGINGA,    "HDN_ITEMCHANGINGA",
     HDN_ITEMCHANGINGW,    "HDN_ITEMCHANGINGW",
     HDN_ITEMCHANGEDA,     "HDN_ITEMCHANGEDA",
     HDN_ITEMCHANGEDW,     "HDN_ITEMCHANGEDW",
     HDN_ITEMCLICKA,       "HDN_ITEMCLICKA",
     HDN_ITEMCLICKW,       "HDN_ITEMCLICKW",
     HDN_ITEMDBLCLICKA,    "HDN_ITEMDBLCLICKA",
     HDN_ITEMDBLCLICKW,    "HDN_ITEMDBLCLICKW",
     HDN_DIVIDERDBLCLICKA, "HDN_DIVIDERDBLCLICKA",
     HDN_DIVIDERDBLCLICKW, "HDN_DIVIDERDBLCLICKW",
     HDN_BEGINTRACKA,      "HDN_BEGINTRACKA",
     HDN_BEGINTRACKW,      "HDN_BEGINTRACKW",
     HDN_ENDTRACKA,        "HDN_ENDTRACKA",
     HDN_ENDTRACKW,        "HDN_ENDTRACKW",
     HDN_TRACKA,           "HDN_TRACKA",
     HDN_TRACKW,           "HDN_TRACKW",
     TVN_SELCHANGINGA,     "TVN_SELCHANGINGA",
     TVN_SELCHANGINGW,     "TVN_SELCHANGINGW",
     TVN_SELCHANGEDA,      "TVN_SELCHANGEDA",
     TVN_SELCHANGEDW,      "TVN_SELCHANGEDW",
     TVN_GETDISPINFOA,     "TVN_GETDISPINFOA",
     TVN_GETDISPINFOW,     "TVN_GETDISPINFOW",
     TVN_SETDISPINFOA,     "TVN_SETDISPINFOA",
     TVN_SETDISPINFOW,     "TVN_SETDISPINFOW",
     TVN_ITEMEXPANDINGA,   "TVN_ITEMEXPANDINGA",
     TVN_ITEMEXPANDINGW,   "TVN_ITEMEXPANDINGW",
     TVN_ITEMEXPANDEDA,    "TVN_ITEMEXPANDEDA",
     TVN_ITEMEXPANDEDW,    "TVN_ITEMEXPANDEDW",
     TVN_BEGINDRAGA,       "TVN_BEGINDRAGA",
     TVN_BEGINDRAGW,       "TVN_BEGINDRAGW",
     TVN_BEGINRDRAGA,      "TVN_BEGINRDRAGA",
     TVN_BEGINRDRAGW,      "TVN_BEGINRDRAGW",
     TVN_DELETEITEMA,      "TVN_DELETEITEMA",
     TVN_DELETEITEMW,      "TVN_DELETEITEMW",
     TVN_BEGINLABELEDITA,  "TVN_BEGINLABELEDITA",
     TVN_BEGINLABELEDITW,  "TVN_BEGINLABELEDITW",
     TVN_ENDLABELEDITA,    "TVN_ENDLABELEDITA",
     TVN_ENDLABELEDITW,    "TVN_ENDLABELEDITW",
     TVN_KEYDOWN,          "TVN_KEYDOWN",
     TTN_NEEDTEXTA,        "TTN_NEEDTEXTA",
     TTN_NEEDTEXTW,        "TTN_NEEDTEXTW",
     TTN_SHOW,             "TTN_SHOW",
     TTN_POP,              "TTN_POP",
     TCN_KEYDOWN,          "TCN_KEYDOWN",
     TCN_SELCHANGE,        "TCN_SELCHANGE",
     TCN_SELCHANGING,      "TCN_SELCHANGING",
     TBN_GETBUTTONINFOA,   "TBN_GETBUTTONINFOA",
     TBN_GETBUTTONINFOW,   "TBN_GETBUTTONINFOW",
     TBN_BEGINDRAG,        "TBN_BEGINDRAG",
     TBN_ENDDRAG,          "TBN_ENDDRAG",
     TBN_BEGINADJUST,      "TBN_BEGINADJUST",
     TBN_ENDADJUST,        "TBN_ENDADJUST",
     TBN_RESET,            "TBN_RESET",
     TBN_QUERYINSERT,      "TBN_QUERYINSERT",
     TBN_QUERYDELETE,      "TBN_QUERYDELETE",
     TBN_TOOLBARCHANGE,    "TBN_TOOLBARCHANGE",
     TBN_CUSTHELP,         "TBN_CUSTHELP",
     UDN_DELTAPOS,         "UDN_DELTAPOS",
     PSN_SETACTIVE,        "PSN_SETACTIVE",
     PSN_KILLACTIVE,       "PSN_KILLACTIVE",
     PSN_APPLY,            "PSN_APPLY",
     PSN_RESET,            "PSN_RESET",
     PSN_HELP,             "PSN_HELP",
     PSN_WIZBACK,          "PSN_WIZBACK",
     PSN_WIZNEXT,          "PSN_WIZNEXT",
     PSN_WIZFINISH,        "PSN_WIZFINISH",
     PSN_QUERYCANCEL,      "PSN_QUERYCANCEL"
};



NOTIFICATIONS OtherLookupTable[] = {
    {0x0127, "WM_CHANGEUISTATE"},{0x0128, "WM_UPDATEUISTATE"},{0x0129, "WM_QUERYUISTATE"}
};



//**************** Function Prototypes ******************
void TEMPDEBUG (char * String, ...);
void Debug_Init (void);
void Open_Debug_File(void);
void Decode_Debug_Error(void);



void TEMPDEBUG(char * String, ...) {
//******************************************************************************
//*
//******************************************************************************
    char    Message[] = "The following line was too big for the buffer";
    va_list Args;
    BOOL    RC;
    DWORD   StringLen, RetStringLen;
    char    StringCRLF[250];
//======================================
    if (fTurnOffDebugQuickly == 'N') {
    if (fDebugFileOpen == 'N') {
        fDebugFileOpen = 'Y';
        Open_Debug_File();
        BufferLen = sizeof(Buffer);
    }

    sprintf (StringCRLF, "%s\r\n", String);
    va_start(Args, String);

    StringLen = _vsnprintf(Buffer, BufferLen, StringCRLF, Args);
    if (StringLen == -1) {                                  //the format string is too big for buffer
        RC = WriteFile(hDebugFile, Message, (DWORD) strlen(Message), &StringLen, NULL);
        if (RC == 0) {   
            Decode_Debug_Error();
            sprintf(cDebugDisplayMessage, "\n\nCould not write to file %s\n\n%s\n\n", DebugFileName, cDebugErrorDisplay); 
            MessageBox(NULL, cDebugDisplayMessage, "SERIOUS ERROR!", MB_ICONSTOP|MB_OK|MB_TOPMOST);
        }
        StringLen = (DWORD) strlen(Buffer);
    } 

    RC = WriteFile(hDebugFile, Buffer, StringLen, &RetStringLen, NULL);
    if (RC == 0) {   
        Decode_Debug_Error();
        sprintf(cDebugDisplayMessage, "\n\nCould not write to file %s\n\n%s\n\n", DebugFileName, cDebugErrorDisplay); 
        MessageBox(NULL, cDebugDisplayMessage, "SERIOUS ERROR!", MB_ICONSTOP|MB_OK|MB_TOPMOST);
    }
    }
}




void TEMPDEBUGNOTIFY(char * Message, UINT nNotifyCode) {
//******************************************************************************
// Converts common control NOTIFY message codes into readable text
//******************************************************************************
    int     NOTIFY_COUNT;
    int     iNotify ;
    char    sMessageFound = 'N';
    BOOL    RC;
    DWORD   StringLen, RetStringLen;
    char    StringCRLF[250];
//================================
    if (fTurnOffDebugQuickly == 'N') {
    if (fDebugFileOpen == 'N') {
        fDebugFileOpen = 'Y';
        Open_Debug_File();
        BufferLen = sizeof(Buffer);
    }

    sprintf(StringCRLF, "%s Notify Code %.4d is Unknown\r\n", Message, nNotifyCode);

    NOTIFY_COUNT = sizeof (WMLookupTable) / sizeof (NOTIFICATIONS);

    iNotify = 0;
    while (iNotify < NOTIFY_COUNT
    &&     sMessageFound == 'N') {
        if (WMLookupTable[iNotify].nCode == nNotifyCode) {
            sprintf(StringCRLF, "%s Notify Code %.4d is %s\r\n", Message, nNotifyCode, WMLookupTable[iNotify].pName);
            sMessageFound = 'Y';
        }
        iNotify++;
    }
 
    if  (sMessageFound == 'N') {
        NOTIFY_COUNT = sizeof (CCLookupTable) / sizeof (NOTIFICATIONS);

        iNotify = 0;
        while (iNotify < NOTIFY_COUNT
        &&     sMessageFound == 'N') {
            if (CCLookupTable[iNotify].nCode == nNotifyCode) {
                sprintf(StringCRLF, "%s Notify Code %.4d is %s\r\n", Message, nNotifyCode, CCLookupTable[iNotify].pName);
                sMessageFound = 'Y';
            }
            iNotify++;
        }
    }


    if  (sMessageFound == 'N') {
        NOTIFY_COUNT = sizeof (OtherLookupTable) / sizeof (NOTIFICATIONS);

        iNotify = 0;
        while (iNotify < NOTIFY_COUNT
        &&     sMessageFound == 'N') {
            if (OtherLookupTable[iNotify].nCode == nNotifyCode) {
                sprintf(StringCRLF, "%s Notify Code %.4d is %s\r\n", Message, nNotifyCode, OtherLookupTable[iNotify].pName);
                sMessageFound = 'Y';
            }
            iNotify++;
        }
    }

    StringLen = strlen(StringCRLF); 

    RC = WriteFile(hDebugFile, StringCRLF, StringLen, &RetStringLen, NULL);
    if (RC == 0) {   
        Decode_Debug_Error();
        sprintf(cDebugDisplayMessage, "\n\nCould not write to file %s\n\n%s\n\n", DebugFileName, cDebugErrorDisplay); 
        MessageBox(NULL, cDebugDisplayMessage, "SERIOUS ERROR!", MB_ICONSTOP|MB_OK|MB_TOPMOST);
    }

    }  // end of quick debug turn off
}



void Open_Debug_File(void) {
//******************************************************************************
//*
//******************************************************************************
//======================================
    hDebugFile = CreateFile(DebugFileName, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);

    if (hDebugFile == INVALID_HANDLE_VALUE) {   
        Decode_Debug_Error();
        sprintf(cDebugDisplayMessage, "\n\nCould not Create or Open file %s\n\n%s\n\n", DebugFileName, cDebugErrorDisplay); 
        MessageBox(NULL, cDebugDisplayMessage, "SERIOUS ERROR!", MB_ICONSTOP|MB_OK|MB_TOPMOST);
        exit (8);
    }
}



void Debug_Finish(void) {
//******************************************************************************
//*
//******************************************************************************
    BOOL RC;
//======================================
    RC = CloseHandle(hDebugFile);

    if (RC == 0) {   
        Decode_Debug_Error();
        sprintf(cDebugDisplayMessage, "\n\nCould not close file %s\n\n%s\n\n", DebugFileName, cDebugErrorDisplay); 
        MessageBox(NULL, cDebugDisplayMessage, "SERIOUS ERROR!", MB_ICONSTOP|MB_OK|MB_TOPMOST);
    }
}



void Decode_Debug_Error(void) {
//******************************************************************************
//
//******************************************************************************
    DWORD   dwError;
    char    cErrorNumber[6];
    char    cErrorMessage[512];
//================================
    dwError = GetLastError(); 
    sprintf(cErrorNumber, "%d", dwError);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, cErrorMessage, sizeof(cErrorMessage), NULL);
    sprintf(cDebugErrorDisplay, "%s: %s", cErrorNumber, cErrorMessage);
 }
