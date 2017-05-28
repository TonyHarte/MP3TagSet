//---------------------------------------------------------------------------
// This class stores the characteristics of a window
//---------------------------------------------------------------------------
//#define  WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE 

#include "CError.h"
#include "Windows.h"
#include <cstdio>                  //standard I/O functions
#include <cstdlib> 



CError::CError(int ErrorType, char * SpecificErrorMessage) {
//******************************************************************************
// Gets the last error and converts to a readable english version.  It is put
// into displayable format in Error->Decoded for use in a user variable which
// in turn should be put into Error->Display which go into the message box.
//******************************************************************************
    DWORD   dwError;
    char    cErrorMessage[MESSAGELENGTH];
 //================================
    dwError = GetLastError(); 
    sprintf(cErrorNumber, "%d", dwError);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, cSystemError, MESSAGELENGTH, NULL);

    if (ErrorType = CRITICAL_ERROR) {
        sprintf(cWarningType, "%s", "Error");
        sprintf(cContinuationMessage, "%s", "The program will finish when this message is acknowledged");
        Z_Abend_With_MessageBox(SpecificErrorMessage); }
    else {
        if (ErrorType = DISPLAY_AND_CONTINUE) {
            sprintf(cWarningType, "%s", "Warning");
            sprintf(cContinuationMessage, "%s", "Press OK to continue or thing to finish now");
            Z_Show_MessageBox_And_Continue(SpecificErrorMessage);
        }
    }
}



void CError::Z_Abend_With_MessageBox (char * SpecificErrorMessage) {
//******************************************************************************
// Displays a message box with the formatted message in char field Display
// contained in the LASTERROR struct.
// The program exits with the return code of 8 when message acknowledged.
//******************************************************************************
//================================
    sprintf(cFullMessage, "%s %s:  %s\n%s\n\n", cWarningType, cErrorNumber, SpecificErrorMessage, cContinuationMessage);

    MessageBox(NULL, cFullMessage, "SERIOUS ERROR!", MB_ICONSTOP | MB_OK | MB_TOPMOST);
    exit (8);
 }



void CError::Z_Show_MessageBox_And_Continue (char * SpecificErrorMessage) {
//******************************************************************************
// Displays a message box with the formatted message in char field Display
// contained in the LASTERROR struct.
// The program exits with the return code of 8 when message acknowledged.
//******************************************************************************
//================================
    sprintf(cFullMessage, "%s %s:  %s\n%s\n\n", cWarningType, cErrorNumber, SpecificErrorMessage, cContinuationMessage);

    MessageBox(NULL, cFullMessage, "HORRIBLE!", MB_ICONSTOP | MB_OK | MB_TOPMOST);
 }
