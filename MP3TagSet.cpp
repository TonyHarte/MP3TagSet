//---------------------------------------------------------------------------
// This program sets the ID tags for the current directory - which is usually
// and albun of some sort
//---------------------------------------------------------------------------
#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE


#include <windows.h>
#include <commctrl.h>
#include <Objbase.h>

//****************** TEMP TEMP DELETE *************
#include "TEMPDEBUG.h"
//****************** TEMP TEMP DELETE *************

#define  MP3SetTag
#include "CWindow.h"
#include "MP3TagSet.h"
#include "UD_MainWindowHandler.h" 
#include "UF_DialogHandler.h" 
#include "Resources\resource.h" 
#include "CID3v2Tag.h"
#include "CMP3File.h"
#include "EnforceCCVersion6.h"


int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
//******************************************************************************
//* Main line and initialisation
//******************************************************************************
{   HWND    hMainWindow;
    HWND    hCurrDialog = 0;
    MSG     gMessage;
    HACCEL  hAccel;
    INITCOMMONCONTROLSEX cc;    
/*--------------------------------*/
    A100_Program_Genesis(hThisInst, hPrevInst);

    hAccel = LoadAccelerators(hThisInst, "MyMenu");

    /* Initialize the common controls. */
    cc.dwICC = ICC_LISTVIEW_CLASSES;
    cc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&cc); 
    //DWORD coinit = COINIT_SPEED_OVER_MEMORY;
    CoInitialize(NULL);

    A200_Register_Windows();
    A300_Create_Main_Window();

    hMainWindow = CMainWin.Handle();
    /* Display the window. */
TEMPDEBUG("@@@   before ShowWindow for Main");
    ShowWindow(hMainWindow, nWinMode);
TEMPDEBUG("@@@   before UpdateWindow for main");
    UpdateWindow(hMainWindow);
TEMPDEBUG("@@@   After UpdateWindow from Main");

TEMPDEBUG("@@@   before CreateDialog");
    hCurrDialog = CreateDialog(hThisInst, "MyDialogName", hMainWindow, UF_Dialog_Handler);
TEMPDEBUG("@@@   After CreateDialog");
    CDialogCurrent.Handle_Store(hCurrDialog);

    /* Create the message loop. */
    while (GetMessage(&gMessage, NULL, 0, 0)) {
        if (hCurrDialog == 0
        ||  !IsDialogMessage (hCurrDialog, &gMessage)) {
            if (!TranslateAccelerator(hMainWindow, hAccel, &gMessage)) {
                TranslateMessage(&gMessage);     /* translate keyboard message */
                DispatchMessage(&gMessage);      /* return control to Windows 98 */
            }
        }
    }

    return gMessage.wParam;
}



void A100_Program_Genesis(HINSTANCE hThisInst, HINSTANCE hPrevInst)
//*****************************************************************************
//*
//*****************************************************************************
{/*   unsigned long    lResult;
    unsigned long    lDatatype, lDatasize; */
    HMENU   hMenu;
    DWORD   dDirectoryAttributes;
/*--------------------------------*/
    hMenu = LoadMenu(hThisInst, MAKEINTRESOURCE(MP3TagSet_MENU));
    CMainWin.Store_Attrs("MP3TagSet Main Window", hThisInst, hPrevInst, hMenu);
    Album.Set("E:\\Source\\MP3TagSet Other\\A Little More Sophistication Vol 52");


/*    RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\AJHarte\\Midi Jukebox", 0,
                    "Configuration", 0, KEY_ALL_ACCESS, NULL,
                    &hRegKey, &lResult);

    if (lResult == REG_CREATED_NEW_KEY) {
        gCFG.nMainX = 1;
        gCFG.nMainY = 1;
        gCFG.nMainWidth = 640;
        gCFG.nMainHeight = 480;
        strcpy(gCFG.sLastDirectory, "C:\\");
        gCFG.nMidiDeviceID = 1;
        gCFG.nMSDelay = 1;
        gCFG.nSpeedUp = 0;
                                     
        RegSetValueEx(hRegKey, "Configuration", NULL, REG_BINARY, (LPBYTE)&gCFG, sizeof(gCFG)); }
    else {
        lDatasize = sizeof(gCFG);
        RegQueryValueEx(hRegKey, "Configuration", NULL, &lDatatype, (LPBYTE)&gCFG, &lDatasize);    
    endif    

    RegCloseKey(hRegKey);
    
    nMSDelay = gCFG.nMSDelay;
    nSpeedUp = gCFG.nSpeedUp;
        gCFG.nMainX = 0;
        gCFG.nMainY = 0;
    gCFG.nMainWidth = 800;
    gCFG.nMainHeight = 600;
*/    
}



void A200_Register_Windows(void) 
//*****************************************************************************
//*
//*****************************************************************************
{   WNDCLASSEX  gMainWindow;
    HINSTANCE   hProgramInstance;
    char *      sWindowClassName;
//    char sMessageSkel[200] = "Can't register the %s window - end now before serious errors";
/*--------------------------------*/
    hProgramInstance = CMainWin.Program_Instance();
    sWindowClassName = CMainWin.ClassName();
    gMainWindow.cbSize          = sizeof(WNDCLASSEX); 
    gMainWindow.hInstance       = hProgramInstance;        

    gMainWindow.lpszClassName   = sWindowClassName;  
    gMainWindow.lpfnWndProc     = UD_Main_Handler;  
    gMainWindow.style           = CS_HREDRAW|CS_VREDRAW;            

//   gMainWindow.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
//    gMainWindow.hIconSm         = LoadIcon(NULL, IDI_APPLICATION);
    gMainWindow.hIcon           = LoadIcon(gMainWindow.hInstance, "MP3TagSet");
    gMainWindow.hIconSm         = LoadIcon(gMainWindow.hInstance, "MP3TagSet");
    gMainWindow.hCursor         = LoadCursor(NULL, IDC_ARROW);

    gMainWindow.lpszMenuName    = NULL;
    gMainWindow.cbClsExtra      = 0;                    /* no extra */
    gMainWindow.cbWndExtra      = 0;                    /* information needed */

//    gMainWindow.hbrBackground     = (HBRUSH) GetStockObject(WHITE_BRUSH); 
    gMainWindow.hbrBackground     =  GetSysColorBrush(COLOR_BTNFACE); 

    RegisterClassEx(&gMainWindow);   

/*
    if(!RegisterClassEx(&gMainWindow)) return 0;

    if (RegisterClassEx(&gMainWindow) == 0) {
        wsprintf(sUserMessage, sMessageSkel, "Main Window");
        z_abort(sUserMessage, IMMEDIATE_ABORT);
    }
*/
}                     



void A300_Create_Main_Window(void) 
//*****************************************************************************
//*
//*****************************************************************************
{   HWND        hMainWindow;
    char *      sMainWinClassName;
    HINSTANCE   hProgramInstance;
    DWORD       gWindowExtendedStyle;
    DWORD       gWindowStyle;
    HMENU       hMenu;
/*--------------------------------*/
    sMainWinClassName = CMainWin.ClassName();
    hProgramInstance = CMainWin.Program_Instance();
    hMenu = CMainWin.Menu();
    gWindowExtendedStyle = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;

    gWindowStyle =    WS_VISIBLE  
                    | WS_CLIPCHILDREN
                    | WS_CAPTION 
                    | WS_SYSMENU 
                    | WS_THICKFRAME 
                    | WS_MINIMIZEBOX 
                    | WS_MAXIMIZEBOX
                    ;

    hMainWindow =   CreateWindow(
//                    gWindowExtendedStyle,
                    sMainWinClassName,                      /* name of window class */
                    "MP3 Tag Set             A J Harte",    /* title */
                    gWindowStyle, 
                    CW_USEDEFAULT, CW_USEDEFAULT,           /* X & Y coordinates - let Windows decide */
                    CW_USEDEFAULT, CW_USEDEFAULT,           /* width & Height - let Windows decide */
//              gCFG.nMainX, gCFG.nMainY,
//              gCFG.nMainWidth, gCFG.nMainHeight,
                    HWND_DESKTOP,                           /* no parent window */
                    hMenu,                                   
                    hProgramInstance,                       /* handle of this instance of the program */
                    NULL                                    /* no additional arguments */
  );

/*                 
    if (hMainWindow == NULL) {
        z_abort("Can't create the main window - end now before serious errors", IMMEDIATE_ABORT);
    endif
*/
    CMainWin.Handle_Store(hMainWindow);
}  
 




/*
void Z_Last_Error_Decode(LASTERROR * Error) {
//******************************************************************************
// Gets the last error and converts to a readable english version.  It is put
// into displayable format in Error->Decoded for use in a user variable which
// in turn should be put into Error->Display which go into the message box.
//******************************************************************************
    DWORD   dwError;
    char    cErrorNumber[6];
    char    cErrorMessage[512];
//================================
    dwError = GetLastError(); 
    sprintf(cErrorNumber, "%d", dwError);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, cErrorMessage, sizeof(cErrorMessage), NULL);
    sprintf(Error->Decoded, "Error %s:  %s\n%s\n\n", cErrorNumber, cErrorMessage,
                            "The program will finish when this message is acknowledged");
 }
*/
