#include <windows.h>
#include <cstdlib>
#include <cstdio>
//#include <commctrl.h>
//#include <string.h>

#include "CFile.h"


CFile::CFile() {
//******************************************************************************
//* 
//******************************************************************************
char Filename[_MAX_FNAME];
/*--------------------------------*/
	hFile = NULL;
	hFileMap = NULL;		
    dFileSize = 0;
    pFileContents = NULL;
}



CFile::~CFile() {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
//	if (m_szFileName != NULL)
//        delete m_szFileName;
}



HANDLE CFile::Map_File(char * cFileName) {
//******************************************************************************
//* 
//******************************************************************************
    const DWORD dTimeOut = 1500;
    DWORD dStartTick = 0;
    HANDLE  RC;
/*--------------------------------*/
    //  shell can lock files, simply use brute-force until available
    dStartTick = GetTickCount();
 //   while ((GetTickCount() - dStartTick) < dTimeOut) {
        hFile = CreateFile (cFileName,
		                    GENERIC_READ,
		                    FILE_SHARE_READ,
		                    NULL,
		                    OPEN_EXISTING,
		                    FILE_ATTRIBUTE_READONLY,
		                    NULL);
/*
        if (hFile != INVALID_HANDLE_VALUE) {
            break; }
        else {
            if (GetLastError() != ERROR_SHARING_VIOLATION) {
                break;
            }
        }
*/
//        Sleep(20);
//    }

    if (hFile != INVALID_HANDLE_VALUE) {
        dFileSize = GetFileSize(hFile, NULL);  //will handle file size up to 4,294 MB

        if (dFileSize != 0) {
            RC = Create_File_Mapping();
        }}
    else {
        RC = hFile;
	}

    return RC;
}



HANDLE CFile::Create_File_Mapping(void) {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
    hFileMap = CreateFileMapping (hFile,
	                            NULL,
		                        PAGE_READONLY,
		                        0,
		                        0,
                                NULL);      // give it a unique name
/*
    if (hFileMap != INVALID_HANDLE_VALUE) 
        break;
    else {
        if (GetLastError() != ERROR_SHARING_VIOLATION) {
            break;
        }
    }
*/
//    Get_Pointer_To_Contents();
    return hFileMap;
}



char * CFile::Get_Pointer_To_Contents(void) {
//******************************************************************************
//* 
//******************************************************************************
/*--------------------------------*/
    pFileContents = (char *) MapViewOfFile(hFileMap,
                                FILE_MAP_READ,      // read
                                0,                  // high-order 32 bits of file offset
                                0,                  // low-order 32 bits of file offset
                                dFileSize);         // number of bytes to map

    if (pFileContents == NULL) {
        //ERROR
	}

    return pFileContents;
}
