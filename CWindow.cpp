//---------------------------------------------------------------------------
// This class stores the characteristics of a window
//---------------------------------------------------------------------------
//#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE 

#include "CWindow.h"



void Window::Attrs_Store(char * sPassedClassname, HINSTANCE ThisInst, HINSTANCE PrevInst)
//*****************************************************************************
//* Store Main, parent window definitions
//*****************************************************************************
{   int     Len;
/*--------------------------------*/
//    Len = strlen(sPassedClassname);
    Len = 30;
    strcpy_s(sClassName, Len, sPassedClassname);
    hProgramInstance = ThisInst;
    hPreviousInstance = PrevInst;
}



void Window::Handle_Store(HWND hMainWindowHandle)
//*****************************************************************************
//* Store Main, parent window handle after created
//*****************************************************************************
{   
/*--------------------------------*/
    hHandle = hMainWindowHandle;
}



HINSTANCE Window::Program_Instance(void) {
//*****************************************************************************
//*****************************************************************************
/*--------------------------------*/
    return hProgramInstance;
}


char * Window::ClassName(void) {
//*****************************************************************************
//*****************************************************************************
/*--------------------------------*/
    return sClassName;
}


HWND Window::Handle(void) {
//*****************************************************************************
//*****************************************************************************
/*--------------------------------*/
    return hHandle;
}
