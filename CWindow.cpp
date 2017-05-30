//---------------------------------------------------------------------------
// This class stores the characteristics of a window
//---------------------------------------------------------------------------
//#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE 

#include "CWindow.h"



void Window::Store_Attrs(char * sPassedClassname, HINSTANCE hThisInst, HINSTANCE hPrevInst, HMENU hPassedMenu)
//*****************************************************************************
//* Store Main, parent window definitions
//*****************************************************************************
{   int     Len;
/*--------------------------------*/
//    Len = strlen(sPassedClassname);
    Len = 30;
    strcpy_s(sClassName, Len, sPassedClassname);
    hProgramInstance = hThisInst;
    hPreviousInstance = hPrevInst;
    hMenu = hPassedMenu;
}



void Window::Handle_Store(HWND hMainWindowHandle) {
//*****************************************************************************
//* Store Main, parent window handle after created
//*****************************************************************************
/*--------------------------------*/
    hHandle = hMainWindowHandle;
}


//*****************************************************************************
//     Get Type Functions
//*****************************************************************************
HINSTANCE   Window::Program_Instance(void)  {return hProgramInstance;}

char *      Window::ClassName(void)         {return sClassName;}

HWND        Window::Handle(void)            {return hHandle;}

HMENU       Window::Menu(void)              {return hMenu;}
