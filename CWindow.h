#ifndef AJHMP3TagSetWindow
    #define AJHMP3TagWindow

#define  WIN32_LEAN_AND_MEAN
//#define _CRT_SECURE_NO_DEPRECATE 

#include <windows.h>
#include <commctrl.h>
#include <cstring>
#include <cstdio>

class Window {
private:
    char        sClassName[30];  // name of main window class 
    HINSTANCE   hProgramInstance;
    HINSTANCE   hPreviousInstance;
    HWND        hHandle;   

public:
    void        Attrs_Store (char *, HINSTANCE, HINSTANCE);
    void        Handle_Store (HWND hMainWindowHandle);

    HINSTANCE   Program_Instance (void);
    char *      ClassName (void);
    HWND        Handle(void);

};   //End Class
#endif
